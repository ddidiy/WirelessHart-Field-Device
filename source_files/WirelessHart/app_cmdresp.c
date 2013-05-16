/*
* Copyright (C) 2013 Nivis LLC.
* Email:   opensource@nivis.com
* Website: http://www.nivis.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
* 
* Redistribution and use in source and binary forms must retain this
* copyright notice.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/***************************************************************************************************
* Name:         app_cmdresp.c
* Author:       Nivis LLC
* Date:         August 2008
* Description:  Application Layer, HART commands: parse the request, compose the response
* Changes:
* Revisions:
****************************************************************************************************/

#include <string.h>
#include "app_layer.h"
#include "app_cmdreq.h"
#include "app_cmdresp.h"
#include "drm.h"

#include "datalink_layer_dm.h"
#include "network_layer.h"
#include "phy_layer.h"
#include "provision.h"

#define CMD077_MIN_PREAMBLES_NO   5
#define CMD077_MAX_PREAMBLES_NO   20

#define RESET_WIRELESS_TIMEOUT   (20*32768)    // 20s [RTC units]
#define RESET_WIRED_TIMEOUT      (2*32768)    // 2s [RTC units]

// when set => perform a reset
uint8 g_ucDoDeviceInitFromCmd;   
uint32 g_ulDelayInitFromCmd;

void APP_SetDeviceStatusFlag(uint8_t ucBitMask)
{
  uint8 ucValue=0;
  
  for(uint8_t ucIdx = 0; ucIdx < MASTER_NO - 1; ucIdx++) // no handheld(at present)
  {
    if( g_stProv.aDeviceStatus[ucIdx] & ucBitMask )
      continue;
    else
    {
      g_stProv.aDeviceStatus[ucIdx] |= ucBitMask;
      ucValue = 1;
    }  
  }

  if(CONFIG_CHANGED & ucBitMask)
    g_stProv.unConfigChangeCounter++;  
  
  if( !ucValue )
    return;
  
  // NVM -> store the individual record into FLASH (only after the full record !!!) 
  // write aDeviceStatus to FLASH
  if( g_ulNvmAreaOffset != NVM_AREA1_START_ADDR )
    NVM_WriteIndividualRecord( 22, 
                               (uint8*)&g_stProv + c_aProvNvmTable[21].m_unOffset, 
                               c_aProvNvmTable[21].m_ucSize);
}

// ucControl indicates the address type: can be MAINTENANCE_PORT_MASK or 0 if wireless source address
void APP_ResetDeviceStatusFlag(SHORT_ADDR unSrcAddr, uint8_t ucControl, uint8_t ucBitMask)
{
  uint8 ucValue=0;
  
  uint8 ucMask = ucBitMask & (CONFIG_CHANGED | COLD_START | MORE_STATUS_AVAILABLE);
  
  if(ucMask)
  {
    if(!(ucControl & MAINTENANCE_PORT_MASK))
    {
      unSrcAddr = (NM_NICKNAME == unSrcAddr) ? MASTER_NM_IDX : MASTER_GW_IDX; // any other nickname is associated with MASTER_GW_IDX!!!
    }
  
    if( g_stProv.aDeviceStatus[unSrcAddr] & ucMask )
    { 
      g_stProv.aDeviceStatus[unSrcAddr] &= ~ucMask;
      ucValue = 1;  // ??? is it necessary for all of them?
    } 
  }
  
  ucMask = ucBitMask & (DEVICE_MALFUNCTION | LOOP_CURRENT_FIXED | LOOP_CURRENT_SATURATED | NON_PRIMARY_VARIABLE_OUT_OF_LIMITS | PRIMARY_VARIABLE_OUT_OF_LIMITS);
  
  if(ucMask)
  {
    for(uint8_t ucIdx = 0; ucIdx < MASTER_NO - 1; ucIdx++)
    {
      if( g_stProv.aDeviceStatus[ucIdx] & ucMask )
      {
        g_stProv.aDeviceStatus[ucIdx] &= ~ucMask;
        ucValue = 1;  // ??? is it necessary for all of them?
      }
    }
  }
  
  if( !ucValue )
    return;
  
  // NVM -> store the individual record into FLASH 
  // write aDeviceStatus to FLASH  
  NVM_WriteIndividualRecord( 22, 
                             (uint8*)&g_stProv + c_aProvNvmTable[21].m_unOffset, 
                             c_aProvNvmTable[21].m_ucSize);
  
}

uint8_t APP_GetDeviceStatus(uint8_t ucSrvIdx)
{
  if(ucSrvIdx >= MASTER_NO)
    ucSrvIdx = MASTER_GW_IDX;
  
  return g_stProv.aDeviceStatus[ucSrvIdx];
}


uint8_t checkExecTimeAndRespSize(_time40_t tExecTime, uint8_t ucTruncated, uint16_t unRemain)
{
  uint8_t rc = 0;   // delayed = 0;
  
  if(memcmp(tExecTime, "\0\0\0\0\0", 5))
  { // execution time is different from zero (delayed execution is intended)
    // check if execution time is smaller than current ASN (in the past)
    unsigned long long ullExecTime = 0, ullCrtASN;
    InverseMemcpy(&ullExecTime, tExecTime, 5);
    MONITOR_ENTER();
    ullCrtASN = g_sDllASN.m_ullASN;
    MONITOR_EXIT();
    
    if((ullExecTime - ullCrtASN) >= 0x8000000000ULL) // half of the ASN domain
    { 
      rc = RCM_E66_InvalidExecutionTime;
    }
    else
    {
      rc = 1;   // delayed = 1;
    }
  }
//  else
//  {
//    rc = 0;   // delayed = 0
//  }

  if((ucTruncated ? unRemain : (unRemain - 5)) >= 0x8000)
    if(rc <= 1)
      rc = RCS_E60_PayloadTooLong;
  
  return rc;
}  


/*
* Execute functions prototype:
*
* typedef uint16_t (*ExecuteFunction)(void* request, void* response, SHORT_ADDR srcAddress,
*                                     uint8_t control, uint8_t remainingBytes);
*
* Parameters:
* @request: pointer to struct of command's specific request params (input data)
* @response: pointer to struct of command's specific response params (output data)
* @srcAddress: requesting device
* @control: bit0 = 1 -request issued via the maintenance port (wired network) , 0 -via the wireless network (use MAINTENANCE_PORT_MASK)
*           bit7 = 1 -published response, 0 -requested command (use PUBLISHED_RESP_MASK)
* @remainingBytes: number of free bytes left in response buffer
*
* Returned value: Command-specific response code
*/


uint8_t CheckAuthorizedMasterRequest(SHORT_ADDR srcAddr, uint8_t control)

{
  if(srcAddr != NM_NICKNAME && srcAddr != GW_NICKNAME)   // network manager, gateway
  {
    if(!(control & MAINTENANCE_PORT_MASK))
      return RCS_E16_AccessRestricted;
  }
  return RCS_N00_Success;
}


/*************************************************************************/
/*********************** Universal Commands ******************************/
/*************************************************************************/

/*********** CMD 000 *************************/

uint16_t Execute_C000_ReadUniqueIdentifier( C000_ReadUniqueIdentifier_Req* request,
                                            C000_ReadUniqueIdentifier_Resp* response,
                                            SHORT_ADDR srcAddr,
                                            uint8_t control, uint8_t remainingBytes)
{
#ifdef TEST_HCF_TOOL  
//  "06";//delimiter + 
//  xx;//address + 
//  "00";//command + 
//  "18";//data field length + 
//  "00 00";//status + 
//  "FE";//254 first data byte + 

  //memcpy((uint8*)&response->expandedDeviceType, g_stProv.aExtendedDeviceTypeLE, 2); //  xx xx;//expanded device type +
  response->expandedDeviceType = 0x0102;
  response->minReqPreamblesNo = CMD000_MIN_REQUIRED_PREAMBLES_FROM_MASTER;          //  "05";//min preamble bytes +
  response->protocolMajorRevNo = CMD000_HART_PROTOCOL_MAJOR_REV_NO;                 //  "07";//hart protocol major rev number +
  response->deviceRevisionLevel = CMD000_DEVICE_REVISION_LEVEL;                     //  "01";//device revision + 
  response->softwareRevisionLevel = 2;                                              //  "02";//software revison + 
  response->hardwareRevisionLevel = 1;                                              //  0x0C  //  "01";//hardware revision + 
  response->physicalSignalingCode = 4; // CommonTable10
  response->flags = 0x08;                                                              //  "00";//Flags + 
  response->deviceID = 0;
  //memcpy((uint8*)&response->deviceID, g_stProv.aDeviceIdLE, 3);                     //  xx xx xx;//Device ID + 
  response->deviceID = 0x00333435;
  response->minRespPreamblesNo = 5;                                                 //  "05";//min preamble bytes from slave to master + 
  response->maxNoOfDeviceVars = 2;                                                  //  "02";//max nr of device variables + 
  response->configChangeCounter = 0;                                                //  "00 00";//configuration change counter + 
  response->extendedFieldDeviceStatus = 0;                                          //  "00";//extended field status of device + 
  response->manufacturerIDCode = 0x00C0;                                            //  "00 C0";//manuf identification code = Kongsberg Maritime + 
  response->privateLabelDistributorCode = 0x00C0;                                   //  "00 C0";//private distributor label = Kongsberg Maritime + 
  response->deviceProfile = 0x81;                                                   //  "81";//device profile

  
#else
  // normal Cmd0 execution
  #ifdef IS_MACHINE_LITTLE_ENDIAN
  memcpy((uint8*)&response->expandedDeviceType, g_aJoinDevUID_LE + 3, 2);
  response->deviceID = 0;
  memcpy((uint8*)&response->deviceID, g_aJoinDevUID_LE, 3);
  #else // big endian
  memcpy((uint8*)&response->expandedDeviceType, g_aJoinDevUID_BE, 2);
  response->deviceID = 0;
  memcpy(&((uint8*)&response->deviceID)[1], g_aJoinDevUID_BE + 2, 3);
  #endif

  response->minReqPreamblesNo = CMD000_MIN_REQUIRED_PREAMBLES_FROM_MASTER;
  response->protocolMajorRevNo = CMD000_HART_PROTOCOL_MAJOR_REV_NO;
  response->deviceRevisionLevel = CMD000_DEVICE_REVISION_LEVEL;
  response->softwareRevisionLevel = CMD000_SOFTWARE_REVISION_LEVEL;
  response->hardwareRevisionLevel = g_stProv.ucHardwareRevisionLevel;
  response->physicalSignalingCode = CMD000_PHYSICAL_SIGNALING_CODE; // CommonTable10
  response->flags = CMD000_FLAGS;  // CommonTable11
  response->minRespPreamblesNo = g_stProv.ucMinRespPreamblesNo;
  response->maxNoOfDeviceVars = g_stProv.ucMaxNoOfDeviceVars;
  response->configChangeCounter = g_stProv.unConfigChangeCounter;
  response->extendedFieldDeviceStatus = g_stCmd48Data.data.extendedDeviceStatus; // CommonTable17
  response->manufacturerIDCode = g_stProv.unManufacturerIDCode; // CommonTable8
  response->privateLabelDistributorCode = g_stProv.unPrivateLabelDistributorCode; // CommonTable8
  response->deviceProfile = g_stProv.ucDeviceProfile; // CommonTable57
#endif

  return RCS_N00_Success;
} // End - Execute_C000_ReadUniqueIdentifier()


/*********** CMD 001 *************************/

uint16_t Execute_C001_ReadPrimaryVariable( C001_ReadPrimaryVariable_Req* request,
                                           C001_ReadPrimaryVariable_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes)
{
// TODO[Doinel]:
// return RCS_E06_DeviceSpecificCommandError;
// return RCS_E16_AccessRestricted;

  response->primaryVariableUnit = g_stDeviceVariableNvData[c_aVarMap[0]].m_ucUnit;
  response->primaryVariable = g_stDeviceVariables[c_aVarMap[0]].m_fValue;

// TODO[Doinel]: this should be reworked, maybe using a freshness flag? The same for C001, C002, C003, C009, C033, C060, C061, C062, C078, C110
//  if(g_stDeviceVariables[c_aVarMap[0]].m_fValue == g_stDeviceVariables[c_aVarMap[0]].m_fLastValue)  //????
//    return RCM_W08_UpdateFailure;
//  else
    return RCS_N00_Success;

} // End - Execute_C001_ReadPrimaryVariable()


/*********** CMD 002 *************************/

uint16_t Execute_C002_ReadLoopCurrentAndPercentOfRange( C002_ReadLoopCurrentAndPercentOfRange_Req* request,
                                                        C002_ReadLoopCurrentAndPercentOfRange_Resp* response,
                                                        SHORT_ADDR srcAddr,
                                                        uint8_t control, uint8_t remainingBytes)
{
// TODO[Doinel]:
// return RCS_E06_DeviceSpecificCommandError;
// return RCS_E16_AccessRestricted;

// TODO[Doinel]: this should report the loop current, in milli-amps, and the percent of range
  
  response->primaryVariablePercentOfRange = g_stDeviceVariables[PERCENT_OF_RANGE_IDX].m_fValue;
  
#ifdef USE_LOOP_CONTROL 
  // tbd  
  response->primaryVariableLoopCurrent = g_stDeviceVariables[LOOP_CURRENT_IDX].m_fValue;
#else    
  response->primaryVariableLoopCurrent = c_stNoValue.m_fValue; 
#endif
  
// TODO[Doinel]:
//  if(...)
//    return RCM_W08_UpdateFailure;
//  else
    return RCS_N00_Success;

} // End - Execute_C002_ReadLoopCurrentAndPercentOfRange()


/*********** CMD 003 *************************/

uint16_t Execute_C003_ReadDynamicVariablesAndLoopCurrent( C003_ReadDynamicVariablesAndLoopCurrent_Req* request,
                                                          C003_ReadDynamicVariablesAndLoopCurrent_Resp* response,
                                                          SHORT_ADDR srcAddr,
                                                          uint8_t control, uint8_t remainingBytes)
{
// TODO[Doinel]:
// return RCS_E06_DeviceSpecificCommandError;
// return RCS_E16_AccessRestricted;


  uint8 ucIdx=0;
  for(; ucIdx < NoOfDynamicVariables; ucIdx++)
  {
    if( DeviceVariableCodes_NotUsed == g_stDeviceVariableNvData[c_aVarMap[ucIdx]].m_ucCode )
      break;
  }

  if( !ucIdx )
  {
    // ??? TBD -> what happens if the device has no dynamic variable ?
  }
  else
  {
    response->noOfVariables = ucIdx;
  }

  if( remainingBytes < (4 + 5*ucIdx) ) // response length: 4 + 5 * response->noOfVariables
  {
    return RCS_E60_PayloadTooLong;
  }

#ifdef USE_LOOP_CONTROL 
  // tbd
  response->primaryVariableLoopCurrent = c_stNoValue.m_fValue; // must be changed !!
#else    
  response->primaryVariableLoopCurrent = c_stNoValue.m_fValue; 
#endif
  
  for(ucIdx=0; ucIdx < response->noOfVariables; ucIdx++ )
  {
    response->variables[ucIdx].unitsCode = g_stDeviceVariableNvData[c_aVarMap[ucIdx]].m_ucUnit;
    response->variables[ucIdx].variable = g_stDeviceVariables[c_aVarMap[ucIdx]].m_fValue;
  }

// TODO[Doinel]:
//  if(...)
//    return RCM_W08_UpdateFailure;
//  else
  return RCS_N00_Success;

} // End - Execute_C003_ReadDynamicVariablesAndLoopCurrent()


/*********** CMD 006 *************************/

uint16_t Execute_C006_WritePollingAddress( C006_WritePollingAddress_Req* request,
                                           C006_WritePollingAddress_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes)
{
#if(WIHARTDEV_TYPE == WIHARTDEV_TYPE_ACCPOINT)
  return RCS_E64_CommandNotImplemented;
#else    
    
  CK_WP_MODE;       //  return RCS_E07_InWriteProtectMode;
  CK_LOCK_STATUS;   //  return RCS_E16_AccessRestricted;

    
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;
  
  if(request->pollingAddressDevice > 63 )
    return RCS_E02_InvalidSelection; // invalid poll address selection

  // ? can request->ucBackwardCompatMode be used somehow ?
#ifdef USE_LOOP_CONTROL
  if(request->loopCurrentModeCode > LoopCurrentModeCodes_Enabled)
    return RCM_E12_InvalidModeSelection;
#else
  #ifndef HCF_LOOP_CURRENT_WA  
  
  if(request->loopCurrentModeCode != LoopCurrentModeCodes_Disabled)
  {
    APP_SetDeviceStatusFlag(LOOP_CURRENT_FIXED); // set LOOP_CURRENT_FIXED bit in device status
    return RCM_E12_InvalidModeSelection;
  }
  
  #endif //#ifndef HCF_LOOP_CURRENT_WA  
#endif
  
  SET_CONFIG_CHANGED
  
  // write the polling address of the device and the loop current mode
  if(g_stProv.ucDevicePollingAddr != request->pollingAddressDevice ||
     g_stProv.ucLoopCurrentMode != request->loopCurrentModeCode)
  { // write to EEPROM only if necessary
    g_stProv.ucDevicePollingAddr = request->pollingAddressDevice; 
    g_stProv.ucLoopCurrentMode = request->loopCurrentModeCode;
    DLLHART_LOCAL_MANAGEMENT_Request(SET_POOLING_ADDRESS,&g_stProv.ucDevicePollingAddr);
    
    #ifndef HCF_LOOP_CURRENT_WA
    if(request->loopCurrentModeCode == LoopCurrentModeCodes_Disabled)
    {
      APP_SetDeviceStatusFlag(LOOP_CURRENT_FIXED); 
    }
    else
    {
      APP_ResetDeviceStatusFlag(srcAddr, control, LOOP_CURRENT_FIXED); 
    }
    #endif //#ifndef HCF_LOOP_CURRENT_WA
    // NVM -> store the individual record into FLASH 
    // write ucDevicePollingAddr to FLASH
    NVM_WriteIndividualRecord( 1, 
                               (uint8*)&g_stProv + c_aProvNvmTable[0].m_unOffset, 
                               c_aProvNvmTable[0].m_ucSize);
    
    // write ucLoopCurrentMode to FLASH
    NVM_WriteIndividualRecord( 4, 
                               (uint8*)&g_stProv + c_aProvNvmTable[3].m_unOffset, 
                               c_aProvNvmTable[3].m_ucSize);
  }
  
  response->pollingAddressDevice = request->pollingAddressDevice;
  response->loopCurrentModeCode = request->loopCurrentModeCode;
 
  
  return RCS_N00_Success; 
#endif //WIHARTDEV_TYPE_ACCPOINT   
  
} // End - Execute_C006_WritePollingAddress()


/*********** CMD 007 *************************/

uint16_t Execute_C007_ReadLoopConfiguration( C007_ReadLoopConfiguration_Req* request,
                                             C007_ReadLoopConfiguration_Resp* response,
                                             SHORT_ADDR srcAddr,
                                             uint8_t control, uint8_t remainingBytes)
{

// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
  
  response->pollingAddressDevice = g_stProv.ucDevicePollingAddr;
  response->loopCurrentModeCode  = g_stProv.ucLoopCurrentMode;
  
  return RCS_N00_Success;

} // End - Execute_C007_ReadLoopConfiguration()


/*********** CMD 008 *************************/

uint16_t Execute_C008_ReadDynamicVariableClassifications(
                                     C008_ReadDynamicVariableClassifications_Req* request,
                                     C008_ReadDynamicVariableClassifications_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;

  uint8 index;
  uint8* p = (uint8*)response;
  
  for (index = 0; index < NoOfDynamicVariables; index++)
  {
    *(p+index) = g_stDeviceVariableNvData[index].m_ucClassification;
  }
  
  /* max. number of dynamic variables = 4 */
  for (index = NoOfDynamicVariables; index < 4; index++)
  {
    *(p+index) = (uint8)DeviceVariableClassificationCodes_NotUsed; //spec 127r7.1 page 24
  }
  
  return RCS_N00_Success;
  
} // End - Execute_C008_ReadDynamicVariableClassifications()


/*********** CMD 009 *************************/

uint16_t Execute_C009_ReadDeviceVariablesWithStatus( C009_ReadDeviceVariablesWithStatus_Req* request,
                                                     C009_ReadDeviceVariablesWithStatus_Resp* response,
                                                     SHORT_ADDR srcAddr,
                                                     uint8_t control, uint8_t remainingBytes)
{
  NV_DEVICE_VARIABLE_T* p;
  TIME_DATE_T stCrtTimeDate;
  uint16_t ucRetCode = RCS_N00_Success;
     
  //hcf work@
  uint8_t dummyVars[4] = {0xFF, 0xFF, 0xFF, 0xFF};
  if(request->variablesSize && !memcmp(dummyVars, request->slotDVC, request->variablesSize))
    return RCS_E02_InvalidSelection;
  //~hcf work@
  
  if( remainingBytes < 5 + 8*request->variablesSize ) // response length: 5 + 8 * request->variablesSize
  {
    return RCS_E60_PayloadTooLong;
  }
  
  response->variablesSize = request->variablesSize;
  response->extendedFieldDeviceStatus = g_stCmd48Data.data.extendedDeviceStatus; 
 
  uint8 ucIdx=0;
  for(; ucIdx<response->variablesSize; ucIdx++)
  {

#if ( _UAP_TYPE != UAP_TYPE_SIMPLE_API ) 
    /*
    if( !NoOfDeviceVariables ) //this condition does not apply in current implem
    {
      switch(request->slotDVC[ucIdx]){
        case 0: 
          {
            request->slotDVC[ucIdx] = DeviceVariableCodes_PrimaryVariable; 
            ucRetCode = RCM_W14_DynamicVarsReturnedForDeviceVars;
          }
          break;
        case 1: 
          {
            request->slotDVC[ucIdx] = DeviceVariableCodes_SecondaryVariable; 
            ucRetCode = RCM_W14_DynamicVarsReturnedForDeviceVars;
          }
          break;
        case 2: 
          {
            request->slotDVC[ucIdx] = DeviceVariableCodes_TertiaryVariable; 
            ucRetCode = RCM_W14_DynamicVarsReturnedForDeviceVars;
          }
          break;
        case 3: 
          {
            request->slotDVC[ucIdx] = DeviceVariableCodes_QuaternaryVariable; 
            ucRetCode = RCM_W14_DynamicVarsReturnedForDeviceVars;
          }
          break;
        default: break;
      } //end switch 
    }  
    */
#endif
    
    p = APP_ReadChannel(request->slotDVC[ucIdx]);
    response->slots[ucIdx].deviceVariableCode = request->slotDVC[ucIdx];
    if( NULL != p )
    {   
      response->slots[ucIdx].deviceVariableClassificationCode = p->m_ucClassification;
      response->slots[ucIdx].unitCode = p->m_ucUnit;
      response->slots[ucIdx].deviceVariableValue = g_stDeviceVariables[p - g_stDeviceVariableNvData].m_fValue;
      response->slots[ucIdx].deviceVariableStatus = p->m_ucStatus;
    }
    else
    {
      response->slots[ucIdx].deviceVariableClassificationCode = DeviceVariableClassificationCodes_DeviceVariableNotClassified;
      response->slots[ucIdx].unitCode = UnitsCodes_NotUsed; 
      response->slots[ucIdx].deviceVariableValue = c_stNoValue.m_fValue;
      response->slots[ucIdx].deviceVariableStatus = 0x30; // Status="Bad" and Limit="Constant"
    }
  }

  // calculate the current time and date
  APP_TimeToDate(g_sDllASN.m_ullASN, &stCrtTimeDate);
 
  // Cmd9 timestamp = time within the present day  
  response->slot0TimeStamp.u32 = stCrtTimeDate.m_ulTimeOfDay;

  return ucRetCode;
// TODO[Doinel]:
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_W08_UpdateFailure;
//  return RCM_W14_DynamicVarsReturnedForDeviceVars;
//  return RCS_E16_AccessRestricted;
//  return RCM_W30_CommandResponseTruncated;

} // End - Execute_C009_ReadDeviceVariablesWithStatus()


/*********** CMD 011 *************************/

uint16_t Execute_C011_ReadUniqueIdentifierAssociatedWithTag(
                                     C011_ReadUniqueIdentifierAssociatedWithTag_Req* request,
                                     C011_ReadUniqueIdentifierAssociatedWithTag_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  if(memcmp(g_stProv.aTag, request->tag, sizeof(g_stProv.aTag)))
    return RCS_N255_MakeNoResponse; // this request will return no response!!!

  return Execute_C000_ReadUniqueIdentifier(0, response, srcAddr, control, remainingBytes);

} // End - Execute_C011_ReadUniqueIdentifierAssociatedWithTag()


/*********** CMD 012 *************************/

uint16_t Execute_C012_ReadMessage( C012_ReadMessage_Req* request,
                                   C012_ReadMessage_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes)
{

  // reads the message contained within the device
  
  memcpy(response->message, g_stProv.aMessage, 24);
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C012_ReadMessage()


/*********** CMD 013 *************************/

uint16_t Execute_C013_ReadTagDescriptorDate( C013_ReadTagDescriptorDate_Req* request,
                                             C013_ReadTagDescriptorDate_Resp* response,
                                             SHORT_ADDR srcAddr,
                                             uint8_t control, uint8_t remainingBytes)
{
  memcpy(response->tag, g_stProv.aTag, 6);
  memcpy(response->descriptor, g_stProv.aDescriptor, 12);
  memcpy(response->dateCode.date, g_stProv.aDate, 3);
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C013_ReadTagDescriptorDate()


/*********** CMD 014 *************************/

uint16_t Execute_C014_ReadPrimaryVariableTransducerInformation(
                                     C014_ReadPrimaryVariableTransducerInformation_Req* request,
                                     C014_ReadPrimaryVariableTransducerInformation_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  // reads the transducer SN, Limits/Minimum Span Units Code, Upper/Lower Transducer Limit and Min. Span for the PV => TBD !!
  response->transducerSerialNumber = g_stDeviceVariableNvData[PV_IDX].m_ulTransducerSN;
  response->transducerLimitsMinimumSpanUnitsCode = g_stDeviceVariableNvData[PV_IDX].m_ucUnit;
  response->upperTransducerLimit = g_stDeviceVariableNvData[PV_IDX].m_fUpperLimit; 
  response->lowerTransducerLimit = g_stDeviceVariableNvData[PV_IDX].m_fLowerLimit; 
  response->minimumSpan = g_stDeviceVariableNvData[PV_IDX].m_fMinSpan; 
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C014_ReadPrimaryVariableTransducerInformation()


/*********** CMD 015 *************************/

uint16_t Execute_C015_ReadDeviceInformation( C015_ReadDeviceInformation_Req* request,
                                             C015_ReadDeviceInformation_Resp* response,
                                             SHORT_ADDR srcAddr,
                                             uint8_t control, uint8_t remainingBytes)
{
  
  // reads the alarm selection code, transfer function code, range values units code, upper/lower range value for the PV => TBD !!

  // TBD !! 
  response->pvAlarmSelectionCodes = 251; // see Common Table 6
  response->pvTransferFunctionCode = 251; // see Common Table 3 (=0, linear if transfer functions are not supported)
  response->pvUpperLowerRangeValuesUnitsCode = g_stDeviceVariableNvData[PV_IDX].m_ucUnit; 
  response->pvUpperRangeValue = g_stDeviceVariableNvData[PV_IDX].m_fUpperLimit; // float value, tbd 
  response->pvLowerRangeValue = g_stDeviceVariableNvData[PV_IDX].m_fLowerLimit; // float value, tbd
  response->pvDampingValue = g_stDeviceVariableNvData[PV_IDX].m_fDampingValue; // float value, units of seconds, tbd 
  response->writeProtectCode = 251; // see Common Table 7 (=251, none, when write protect is not implemented)
  response->pvAnalogChannelFlags = 0x00; // see Common Table 26
 
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C015_ReadDeviceInformation()


/*********** CMD 016 *************************/

uint16_t Execute_C016_ReadFinalAssemblyNumber( C016_ReadFinalAssemblyNumber_Req* request,
                                               C016_ReadFinalAssemblyNumber_Resp* response,
                                               SHORT_ADDR srcAddr,
                                               uint8_t control, uint8_t remainingBytes)
{
  response->finalAssemblyNumber = g_stProv.ulFinalAssemblyNmb;  // uint24_t

  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C016_ReadFinalAssemblyNumber()


/*********** CMD 017 *************************/

uint16_t Execute_C017_WriteMessage( C017_WriteMessage_Req* request,
                                    C017_WriteMessage_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  SET_CONFIG_CHANGED
    
  *response = *request;
  
  if( memcmp(g_stProv.aMessage, request->messageString, 24) ) // new message ?
  {  
    // writes the message into the device
    memcpy(g_stProv.aMessage, request->messageString, 24);
    
    // write aMessage to FLASH
    NVM_WriteIndividualRecord( 21, 
                               (uint8*)&g_stProv + c_aProvNvmTable[20].m_unOffset, 
                               c_aProvNvmTable[20].m_ucSize);
    
  }
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C017_WriteMessage()


/*********** CMD 018 *************************/

uint16_t Execute_C018_WriteTagDescriptorDate( C018_WriteTagDescriptorDate_Req* request,
                                              C018_WriteTagDescriptorDate_Resp* response,
                                              SHORT_ADDR srcAddr,
                                              uint8_t control, uint8_t remainingBytes)
{  
  CK_WP_MODE
  SET_CONFIG_CHANGED 
  *response = *request;  
  
  // NVM -> store the individual record into FLASH 
  // write aDate to FLASH
  if( memcmp(g_stProv.aDate, request->dateCodeUsedByTheMasterForRecordKeeping.date, 3) )  // new date ?
  {
    memcpy(g_stProv.aDate, request->dateCodeUsedByTheMasterForRecordKeeping.date, 3);
    
    NVM_WriteIndividualRecord( 15, 
                               (uint8*)&g_stProv + c_aProvNvmTable[14].m_unOffset, 
                               c_aProvNvmTable[14].m_ucSize);
    
  }  
  
  // write aTag to FLASH
  if( memcmp(g_stProv.aTag, request->tag, 6) )  // new tag ?
  {
    memcpy(g_stProv.aTag, request->tag, 6);
    
    NVM_WriteIndividualRecord( 16, 
                               (uint8*)&g_stProv + c_aProvNvmTable[15].m_unOffset, 
                               c_aProvNvmTable[15].m_ucSize);
    
  }  
    
  // write aDescriptor to FLASH 
  if( memcmp(g_stProv.aDescriptor, request->descriptorUsedByTheMasterForRecordKeeping, 12) ) // new descriptor ?
  {
    memcpy(g_stProv.aDescriptor, request->descriptorUsedByTheMasterForRecordKeeping, 12);
    
    NVM_WriteIndividualRecord( 17, 
                               (uint8*)&g_stProv + c_aProvNvmTable[16].m_unOffset, 
                               c_aProvNvmTable[16].m_ucSize);
    
  }    
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E09_InvalidDateCodeDetected;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C018_WriteTagDescriptorDate()


/*********** CMD 019 *************************/

uint16_t Execute_C019_WriteFinalAssemblyNumber( C019_WriteFinalAssemblyNumber_Req* request,
                                                C019_WriteFinalAssemblyNumber_Resp* response,
                                                SHORT_ADDR srcAddr,
                                                uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  SET_CONFIG_CHANGED 
  *response = *request;
  
  // NVM -> store the individual record into FLASH 
  // write ulFinalAssemblyNmb to FLASH
  if( g_stProv.ulFinalAssemblyNmb != request->finalAssemblyNumber ) // new number ?
  {
    g_stProv.ulFinalAssemblyNmb = request->finalAssemblyNumber; 
    
    NVM_WriteIndividualRecord( 53, 
                               (uint8*)&g_stProv + c_aProvNvmTable[52].m_unOffset, 
                               c_aProvNvmTable[52].m_ucSize);
  }  
    
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C019_WriteFinalAssemblyNumber()


/*********** CMD 020 *************************/

uint16_t Execute_C020_ReadLongTag( C020_ReadLongTag_Req* request,
                                   C020_ReadLongTag_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes)
{
  if(CheckAuthorizedMasterRequest(srcAddr, control))
    return RCS_E16_AccessRestricted;

// TODO[Doinel]:
//  return RCS_E32_Busy; //???

  memcpy(response->longTag, g_stProv.aLongTag, 32);

  return RCS_N00_Success;
} // End - Execute_C020_ReadLongTag()


/*********** CMD 021 *************************/

uint16_t Execute_C021_ReadUniqueIdentifierAssociatedWithLongTag(
                                     C021_ReadUniqueIdentifierAssociatedWithLongTag_Req* request,
                                     C021_ReadUniqueIdentifierAssociatedWithLongTag_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  if(memcmp(g_stProv.aLongTag, request->longTag, 32))
    return RCS_N255_MakeNoResponse; // this request will return no response!!!

  return Execute_C000_ReadUniqueIdentifier(0, response, srcAddr, control, remainingBytes);

} // End - Execute_C021_ReadUniqueIdentifierAssociatedWithLongTag()


/*********** CMD 022 *************************/

uint16_t Execute_C022_WriteLongTag( C022_WriteLongTag_Req* request,
                                    C022_WriteLongTag_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  SET_CONFIG_CHANGED
  *response = *request;

  // write aLongTag to FLASH
  if( memcmp(g_stProv.aLongTag, request->longTag, 32) ) // new LongTag ?
  { 
    memcpy(g_stProv.aLongTag, request->longTag, 32);
    
    NVM_WriteIndividualRecord( 19, 
                               (uint8*)&g_stProv + c_aProvNvmTable[18].m_unOffset, 
                               c_aProvNvmTable[18].m_ucSize);
  }
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy; // a DR could not be started
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponsDead;
//  return RCS_E36_DelayedResponseConflict;

} // End - Execute_C022_WriteLongTag()


/*************************************************************************/
/*********************** Common Practice Commands ************************/
/*************************************************************************/

/*********** CMD 033 *************************/

uint16_t Execute_C033_ReadDeviceVariables(C033_ReadDeviceVariables_Req* request,
                                     C033_ReadDeviceVariables_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  NV_DEVICE_VARIABLE_T* p;
  
  //hcf work@
  uint8_t dummyVars[4] = {0xFF, 0xFF, 0xFF, 0xFF};
  if(request->variablesSize && !memcmp(dummyVars, request->variables, request->variablesSize))
    return RCS_E02_InvalidSelection;
  //~hcf work@
  
  if( remainingBytes < 6*request->variablesSize ) // 6 bytes / variable
  {
    return RCS_E60_PayloadTooLong;
  }
      
  response->variablesSize = request->variablesSize;

  uint8 ucIdx=0;
  for(; ucIdx < request->variablesSize; ucIdx++)
  {
    response->variables[ucIdx].code = request->variables[ucIdx];
    p=APP_ReadChannel(request->variables[ucIdx]);
    
    if( NULL != p )
    {
      response->variables[ucIdx].unitsCode = p->m_ucUnit;
      response->variables[ucIdx].value = g_stDeviceVariables[p - g_stDeviceVariableNvData].m_fValue;
    }
    else  //dev var is not supported
    {
      //see common practice commands spec, page 33.
      response->variables[ucIdx].unitsCode = DeviceVariableFamilyCodes_NotUsed; // = 250; //not used
      response->variables[ucIdx].value = c_stNoValue.m_fValue;;
    }
  }
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_W08_UpdateFailure;
//  return RCS_E16_AccessRestricted;

} // End - Execute_C033_ReadDeviceVariables()


/*********** CMD 034 *************************/

uint16_t Execute_C034_WritePrimaryVariableDampingValue(C034_WritePrimaryVariableDampingValue_Req* request,
                                     C034_WritePrimaryVariableDampingValue_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C034_WritePrimaryVariableDampingValue()


/*********** CMD 035 *************************/

uint16_t Execute_C035_WritePrimaryVariableRangeValues(C035_WritePrimaryVariableRangeValues_Req* request,
                                     C035_WritePrimaryVariableRangeValues_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;  // (Upper or Lower Range Pushed)
//  return RCM_E09_LowerRangeValueTooHigh;
//  return RCM_E10_LowerRangeValueTooLow;
//  return RCM_E11_UpperRangeValueTooHigh;
//  return RCM_E12_UpperRangeValueTooLow;
//  return RCM_E13_UpperAndLowerRangeValuesOutOfLimits;
//  return RCM_W14_SpanTooSmall;  // (Device Accuracy May Be Impaired)
//  return RCS_E16_AccessRestricted;
//  return RCS_E18_InvalidUnitsCode;
//  return RCM_E29_InvalidSpan;
//  return RCS_E32_Busy;

} // End - Execute_C035_WritePrimaryVariableRangeValues()


/*********** CMD 036 *************************/

uint16_t Execute_C036_SetPrimaryVariableUpperRangeValue(C036_SetPrimaryVariableUpperRangeValue_Req* request,
                                     C036_SetPrimaryVariableUpperRangeValue_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;  // (Upper Range Pushed)
//  return RCM_E09_AppliedProcessTooHigh;
//  return RCM_E10_AppliedProcessTooLow;
//  return RCM_W14_SpanTooSmall;  // (Device Accuracy May Be Impaired)
//  return RCS_E16_AccessRestricted;
//  return RCM_E29_InvalidSpan;
//  return RCS_E32_Busy;

} // End - Execute_C036_SetPrimaryVariableUpperRangeValue()


/*********** CMD 037 *************************/

uint16_t Execute_C037_SetPrimaryVariableLowerRangeValue(C037_SetPrimaryVariableLowerRangeValue_Req* request,
                                     C037_SetPrimaryVariableLowerRangeValue_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E09_AppliedProcessTooHigh;
//  return RCM_E10_AppliedProcessTooLow;
//  return RCM_W14_NewLowerRangeValuePushedUpperRangeOver;
//  return RCS_E16_AccessRestricted;
//  return RCM_E29_InvalidSpan;
//  return RCS_E32_Busy;

} // End - Execute_C037_SetPrimaryVariableLowerRangeValue()


/*********** CMD 038 *************************/
//Exception: this is a universal command
uint16_t Execute_C038_ResetConfigurationChangedFlag(C038_ResetConfigurationChangedFlag_Req* request,
                                     C038_ResetConfigurationChangedFlag_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{ 
  CK_WP_MODE
 
  // compare the configuration change counter
  if( !request->backComp && request->configurationChangeCounter != g_stProv.unConfigChangeCounter )
  {
    return RCM_E09_ConfigurationChangeCounterMismatch;   
  }
  
//  if(!(control & MAINTENANCE_PORT_MASK))
//  {
//    srcAddr = (NM_NICKNAME == srcAddr) ? MASTER_NM_IDX : MASTER_GW_IDX;
//  }
//  
//  // reset BIT6 from g_stProv.ucDeviceStatus
//  g_stProv.aDeviceStatus[srcAddr] &= ~CONFIG_CHANGED;   
  APP_ResetDeviceStatusFlag(srcAddr, control, CONFIG_CHANGED);
  
  // compose the response
  response->configurationChangeCounter = g_stProv.unConfigChangeCounter;
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E09_ConfigurationChangeCounterMismatch;
//  return RCS_E16_AccessRestricted;

} // End - Execute_C038_ResetConfigurationChangedFlag()


/*********** CMD 040 *************************/

uint16_t Execute_C040_EnterExitFixedCurrentMode(C040_EnterExitFixedCurrentMode_Req* request,
                                     C040_EnterExitFixedCurrentMode_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E11_LoopCurrentNotActive;  // (Device in Multidrop Mode)
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C040_EnterExitFixedCurrentMode()


/*********** CMD 041 *************************/

uint16_t Execute_C041_PerformSelfTest(C041_PerformSelfTest_Req* request,
                                     C041_PerformSelfTest_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  // initiates the Self Test function in the device
  g_ucDoSelfTest = 1;
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C041_PerformSelfTest()


/*********** CMD 042 *************************/

uint16_t Execute_C042_PerformDeviceReset(C042_PerformDeviceReset_Req* request,
                                     C042_PerformDeviceReset_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_LOCK_STATUS;       // return RCS_E16_AccessRestricted;
  
  // reset the device
  if((control & MAINTENANCE_PORT_MASK))
  {
    g_ucDoDeviceInitFromCmd = APP_DEVRESET_C42_WIRED;
    g_ulDelayInitFromCmd = RESET_WIRED_TIMEOUT;
  }
  else 
  {
    g_ucDoDeviceInitFromCmd = APP_DEVRESET_C42_WIRELESS;
#if ( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )             
//      g_ulDelayInitFromCmd = RESET_WIRELESS_TIMEOUT;
      g_ulDelayInitFromCmd = RESET_WIRED_TIMEOUT; // however it will send the packets from DLL queue first, then the Disconnect packets 
#else
      g_ulDelayInitFromCmd = RESET_WIRED_TIMEOUT; // AP doesn't need 20s to transmit the response via serial interface
#endif
      g_ucMarkDisconnectHandle = 1;
  }
  g_ulDelayInitFromCmd += RTC_COUNT;
  
  
  return RCS_N00_Success;

//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C042_PerformDeviceReset()


/*********** CMD 043 *************************/

uint16_t Execute_C043_SetPrimaryVariableZero(C043_SetPrimaryVariableZero_Req* request,
                                     C043_SetPrimaryVariableZero_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_E09_AppliedProcessTooHigh;
//  return RCM_E10_AppliedProcessTooLow;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C043_SetPrimaryVariableZero()


/*********** CMD 044 *************************/

uint16_t Execute_C044_WritePrimaryVariableUnits(C044_WritePrimaryVariableUnits_Req* request,
                                     C044_WritePrimaryVariableUnits_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C044_WritePrimaryVariableUnits()


/*********** CMD 045 *************************/

uint16_t Execute_C045_TrimLoopCurrentZero(C045_TrimLoopCurrentZero_Req* request,
                                     C045_TrimLoopCurrentZero_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E09_IncorrectLoopCurrentModeOrValue;
//  return RCM_E11_LoopCurrentNotActive;  // (Device in Multidrop Mode)
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C045_TrimLoopCurrentZero()


/*********** CMD 046 *************************/

uint16_t Execute_C046_TrimLoopCurrentGain(C046_TrimLoopCurrentGain_Req* request,
                                     C046_TrimLoopCurrentGain_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E09_IncorrectLoopCurrentModeOrValue;
//  return RCM_E11_LoopCurrentNotActive;  // (Device in Multidrop Mode)
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C046_TrimLoopCurrentGain()


/*********** CMD 047 *************************/

uint16_t Execute_C047_WritePrimaryVariableTransferFunction(C047_WritePrimaryVariableTransferFunction_Req* request,
                                     C047_WritePrimaryVariableTransferFunction_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C047_WritePrimaryVariableTransferFunction()


/*********** CMD 048 *************************/
//Exception: this is a universal command
uint16_t Execute_C048_ReadAdditionalDeviceStatus(C048_ReadAdditionalDeviceStatus_Req* request,
                                     C048_ReadAdditionalDeviceStatus_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_LOCK_STATUS;   //  return RCS_E16_AccessRestricted;

  // device must return the current values
  memcpy(response, &g_stCmd48Data.data, C048_SIZE);
  response->commandSize = C048_SIZE;
  
  // compare the request data bytes to the current value contained in the device 
  if(C048_SIZE > request->commandSize) // if fewer than expected, no sense to compare; if more, will be truncated
  {
    return RCS_N00_Success;
  }
  
  if(request->commandSize && memcmp(request, &g_stCmd48Data.data, C048_SIZE) )  
  {
    return RCM_W14_StatusBytesMismatch;
  }
  
  //reaching this point means exact match => reset More Status Available bit
  APP_ResetDeviceStatusFlag(srcAddr, control, MORE_STATUS_AVAILABLE);
   
   
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_W08_UpdateInProgress;
//  return RCM_W14_StatusBytesMismatch;
//  return RCS_E16_AccessRestricted;

} // End - Execute_C048_ReadAdditionalDeviceStatus()


/*********** CMD 049 *************************/

uint16_t Execute_C049_WritePrimaryVariableTransducerSerialNumber(C049_WritePrimaryVariableTransducerSerialNumber_Req* request,
                                     C049_WritePrimaryVariableTransducerSerialNumber_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C049_WritePrimaryVariableTransducerSerialNumber()


/*********** CMD 050 *************************/

uint16_t Execute_C050_ReadDynamicVariableAssignments(C050_ReadDynamicVariableAssignments_Req* request,
                                     C050_ReadDynamicVariableAssignments_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;

} // End - Execute_C050_ReadDynamicVariableAssignments()


/*********** CMD 051 *************************/

uint16_t Execute_C051_WriteDynamicVariableAssignments(C051_WriteDynamicVariableAssignments_Req* request,
                                     C051_WriteDynamicVariableAssignments_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C051_WriteDynamicVariableAssignments()


/*********** CMD 052 *************************/

uint16_t Execute_C052_SetDeviceVariableZero(C052_SetDeviceVariableZero_Req* request,
                                     C052_SetDeviceVariableZero_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E09_AppliedProcessTooHigh;
//  return RCM_E10_AppliedProcessTooLow;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C052_SetDeviceVariableZero()


/*********** CMD 053 *************************/

uint16_t Execute_C053_WriteDeviceVariableUnits(C053_WriteDeviceVariableUnits_Req* request,
                                     C053_WriteDeviceVariableUnits_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E11_InvalidDeviceVariableCode;
//  return RCM_E12_InvalidUnitsCode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C053_WriteDeviceVariableUnits()


/*********** CMD 054 *************************/

uint16_t Execute_C054_ReadDeviceVariableInformation(C054_ReadDeviceVariableInformation_Req* request,
                                     C054_ReadDeviceVariableInformation_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  NV_DEVICE_VARIABLE_T* p = APP_ReadChannel(request->variableCode);
  
  if( p != NULL )
  {
    response->variableCode = p->m_ucCode;
    response->serialNo = p->m_ulTransducerSN;
    response->spanUnits = p->m_ucUnit;
    response->upperTransducerLimit = p->m_fUpperLimit;
    response->lowerTransducerLimit = p->m_fLowerLimit;
    response->dampingValue = p->m_fDampingValue;
    response->minimumSpan = p->m_fMinSpan;
    
    response->classificationCode = p->m_ucClassification;
    response->familyCode = p->m_ucFamily;
    
    response->updateTime.u32 = 0;  // init update time
    NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData;
    for(; pCrtBurst < g_stBurstModeTableNvData + MAX_BURST_NO; pCrtBurst++)
    { // parse the burst table
      if( pCrtBurst->m_aucDeviceVariableCodes[0] == p->m_ucCode )
      {
        response->updateTime.u32 = pCrtBurst->m_ulMaxUpdateTime;
        response->updateTime.u32 *= 32 * 10; // [1/32 ms]
        break;
      }    
    }  
  }
  else
  {
    return RCS_E06_DeviceSpecificCommandError;  
  }
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C054_ReadDeviceVariableInformation()


/*********** CMD 055 *************************/

uint16_t Execute_C055_WriteDeviceVariableDampingValue(C055_WriteDeviceVariableDampingValue_Req* request,
                                     C055_WriteDeviceVariableDampingValue_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C055_WriteDeviceVariableDampingValue()


/*********** CMD 056 *************************/

uint16_t Execute_C056_WriteDeviceVariableTransducerSerialNo(C056_WriteDeviceVariableTransducerSerialNo_Req* request,
                                     C056_WriteDeviceVariableTransducerSerialNo_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C056_WriteDeviceVariableTransducerSerialNo()


/*********** CMD 059 *************************/

uint16_t Execute_C059_WriteNumberOfResponsePreambles(C059_WriteNumberOfResponsePreambles_Req* request,
                                     C059_WriteNumberOfResponsePreambles_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
#if(WIHARTDEV_TYPE == WIHARTDEV_TYPE_ACCPOINT)  
  return RCS_E64_CommandNotImplemented;
#else  
  CK_WP_MODE;         // return RCS_E07_InWriteProtectMode;
  CK_LOCK_STATUS;     // return RCS_E16_AccessRestricted;
  
  if( request->noOfPreambles < 5 )
    return RCS_E04_PassedParameterTooSmall;  

  if( request->noOfPreambles > 20 )
    return RCS_E03_PassedParameterTooLarge;     

  SET_CONFIG_CHANGED
    
  if( g_stProv.ucMinRespPreamblesNo != request->noOfPreambles )
  {  
    g_stProv.ucMinRespPreamblesNo = request->noOfPreambles; 
    DLLHART_LOCAL_MANAGEMENT_Request(SET_PREAMBLE,&g_stProv.ucMinRespPreamblesNo); // min preambles to be sent with the response message from Slave to Master
  
    // NVM -> store the individual record into FLASH 
    // write ucMinRespPreamblesNo to FLASH
    NVM_WriteIndividualRecord( 5, 
                               (uint8*)&g_stProv + c_aProvNvmTable[4].m_unOffset, 
                               c_aProvNvmTable[4].m_ucSize);
  }
  response->noOfPreambles = g_stProv.ucMinRespPreamblesNo;
  
  return RCS_N00_Success;

//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;   ??
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
#endif //WIHARTDEV_TYPE_ACCPOINT
} // End - Execute_C059_WriteNumberOfResponsePreambles()


/*********** CMD 060 *************************/

uint16_t Execute_C060_ReadAnalogChannelAndPercentOfRange(C060_ReadAnalogChannelAndPercentOfRange_Req* request,
                                     C060_ReadAnalogChannelAndPercentOfRange_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_W08_UpdateFailure;
//  return RCS_E16_AccessRestricted;

} // End - Execute_C060_ReadAnalogChannelAndPercentOfRange()


/*********** CMD 062 *************************/

uint16_t Execute_C062_ReadAnalogChannels(C062_ReadAnalogChannels_Req* request,
                                     C062_ReadAnalogChannels_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_W08_UpdateFailure;

} // End - Execute_C062_ReadAnalogChannels()


/*********** CMD 063 *************************/

uint16_t Execute_C063_ReadAnalogChannelInformation(C063_ReadAnalogChannelInformation_Req* request,
                                     C063_ReadAnalogChannelInformation_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C063_ReadAnalogChannelInformation()


/*********** CMD 064 *************************/

uint16_t Execute_C064_WriteAnalogChannelAdditionalDampingValue(C064_WriteAnalogChannelAdditionalDampingValue_Req* request,
                                     C064_WriteAnalogChannelAdditionalDampingValue_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
    //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C064_WriteAnalogChannelAdditionalDampingValue()


/*********** CMD 065 *************************/

uint16_t Execute_C065_WriteAnalogChannelRangeValues(C065_WriteAnalogChannelRangeValues_Req* request,
                                     C065_WriteAnalogChannelRangeValues_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
    //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;  // (Upper or Lower Range Pushed)
//  return RCM_E09_LowerRangeValueTooHigh;
//  return RCM_E10_LowerRangeValueTooLow;
//  return RCM_E11_UpperRangeValueTooHigh;
//  return RCM_E12_UpperRangeValueTooLow;
//  return RCM_E13_UpperAndLowerRangeValuesOutOfLimits;
//  return RCM_W14_SpanTooSmall;  // (Device Accuracy May Be Impaired)
//  return RCM_E15_InvalidAnalogChannelCodeNumber;
//  return RCS_E16_AccessRestricted;
//  return RCM_E28_InvalidRangeUnitsCode;
//  return RCM_E29_InvalidSpan;
//  return RCS_E32_Busy;

} // End - Execute_C065_WriteAnalogChannelRangeValues()


/*********** CMD 066 *************************/

uint16_t Execute_C066_EnterExitFixedAnalogChannelMode(C066_EnterExitFixedAnalogChannelMode_Req* request,
                                     C066_EnterExitFixedAnalogChannelMode_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E11_InMultidropMode;
//  return RCM_E12_InvalidUnitsCode;
//  return RCM_E15_InvalidAnalogChannelCodeNumber;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C066_EnterExitFixedAnalogChannelMode()


/*********** CMD 067 *************************/

uint16_t Execute_C067_TrimAnalogChannelZero(C067_TrimAnalogChannelZero_Req* request,
                                     C067_TrimAnalogChannelZero_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  return RCS_E64_CommandNotImplemented;
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E09_NotInProperAnalogChannelMode;
//  return RCM_E11_InMultidropMode;
//  return RCM_E12_InvalidUnitsCode;
//  return RCM_E15_InvalidAnalogChannelCodeNumber;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

} // End - Execute_C067_TrimAnalogChannelZero()


/*********** CMD 068 *************************/

uint16_t Execute_C068_TrimAnalogChannelGain(C068_TrimAnalogChannelGain_Req* request,
                                     C068_TrimAnalogChannelGain_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
    //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E09_NotInProperAnalogChannelMode;
//  return RCM_E11_InMultidropMode; 
//  return RCM_E12_InvalidUnitsCode;
//  return RCM_E15_InvalidAnalogChannelCodeNumber;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
  
} // End - Execute_C068_TrimAnalogChannelGain()


/*********** CMD 069 *************************/

uint16_t Execute_C069_WriteAnalogChannelTransferFunction(C069_WriteAnalogChannelTransferFunction_Req* request,
                                     C069_WriteAnalogChannelTransferFunction_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
    //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_E13_InvalidTransferFunctionCode;
//  return RCM_E15_InvalidAnalogChannelCodeNumber;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
  
} // End - Execute_C069_WriteAnalogChannelTransferFunction()


/*********** CMD 070 *************************/

uint16_t Execute_C070_ReadAnalogChannelEndpointValues(C070_ReadAnalogChannelEndpointValues_Req* request,
                                     C070_ReadAnalogChannelEndpointValues_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
  
} // End - Execute_C070_ReadAnalogChannelEndpointValues()


/*********** CMD 071 *************************/

uint16_t Execute_C071_LockDevice(C071_LockDevice_Req* request,
                                     C071_LockDevice_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_E10_InvalidLockCode;
//  return RCM_E11_CannotLockDevice;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict;  
  
} // End - Execute_C071_LockDevice()


/*********** CMD 072 *************************/

uint16_t Execute_C072_Squawk(C072_Squawk_Req* request,
                                     C072_Squawk_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_E09_UnableToSquawk;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy; 
  
} // End - Execute_C072_Squawk()


/*********** CMD 073 *************************/

uint16_t Execute_C073_FindDevice(C073_FindDevice_Req* request,
                                     C073_FindDevice_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  //TODO:
//  if(..device armed by some HW or Device UserInterface dependant condition)
//  {
//    dis-arm the device;
//    return Execute_C000_ReadUniqueIdentifier(0, response, srcAddr, control, remainingBytes);
//  }
//  else
    return RCS_N255_MakeNoResponse;

} // End - Execute_C073_FindDevice()


/*********** CMD 074 *************************/

uint16_t Execute_C074_ReadIOSystemCapabilities(C074_ReadIOSystemCapabilities_Req* request,
                                     C074_ReadIOSystemCapabilities_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
  
} // End - Execute_C074_ReadIOSystemCapabilities()


/*********** CMD 075 *************************/

uint16_t Execute_C075_PollSubDevice(C075_PollSubDevice_Req* request,
                                     C075_PollSubDevice_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E09_NoSubDeviceFound;  
//  return RCS_E16_AccessRestricted;
//  return RCS_E21_InvalidCardNumber;
//  return RCS_E22_InChannelNumber;
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C075_PollSubDevice()


/*********** CMD 076 *************************/

uint16_t Execute_C076_ReadLockDeviceState(C076_ReadLockDeviceState_Req* request,
                                     C076_ReadLockDeviceState_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
  
} // End - Execute_C076_ReadLockDeviceState()


/*********** CMD 077 *************************/

uint16_t Execute_C077_SendCommandToSubDevice(C077_SendCommandToSubDevice_Req* request,
                                     C077_SendCommandToSubDevice_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E21_InvalidIOCardNumber;  
//  return RCS_E22_InvalidChannelNumber;
//  return RCS_E23_SubDeviceResponseTooLong;
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
} // End - Execute_C077_SendCommandToSubDevice()


/*********** CMD 078 *************************/

uint16_t Execute_C078_ReadAggregatedCommands(C078_ReadAggregatedCommands_Req* request,
                                     C078_ReadAggregatedCommands_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  
  BinaryStream stInputStream;
  STREAM_INIT(&stInputStream, request->cmdData, request->cmdSize);
 
  BinaryStream stOutputStream;
  STREAM_INIT(&stOutputStream, response->cmdData, sizeof(response->cmdData));
  

  
  response->extendedDeviceStatus = g_stCmd48Data.data.extendedDeviceStatus;
  response->noOfCommands = request->noOfCommands;
  if(!request->cmdSize)
  {
    response->cmdSize = 0;
    if (request->noOfCommands == 0)
    {
      return RCS_N00_Success;
    }
    else
    {
      return RCS_E05_TooFewDataBytesReceived;
    }
  }

  uint8_t retCode = APP_ProcessCmdBuf( APP_QUEUE_SIZE,
                                        &response->noOfCommands,
                                        control,
                                        CD_REQUEST,
                                        srcAddr,
                                        &stInputStream,
                                        &stOutputStream
                                      ); 
  if(response->noOfCommands < request->noOfCommands)
    return RCS_E05_TooFewDataBytesReceived;
  response->cmdSize = stOutputStream.nextByte - response->cmdData;
  return retCode;

// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;f
//  return RCS_W08_UpdateFailure:  
//  return RCS_E09_InvalidCommandRequested;
//  return RCS_W30_CommandResponseTruncated;
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C078_ReadAggregatedCommands()


/*********** CMD 079 *************************/

uint16_t Execute_C079_WriteDeviceVariable(C079_WriteDeviceVariable_Req* request,
                                     C079_WriteDeviceVariable_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  NV_DEVICE_VARIABLE_T* p = APP_ReadChannel( request->deviceVariableCode );
  
  if( NULL != p )
  {
    if( request->writeCommandCode == WriteDeviceVariableCodes_FixedValue )
    { 
      // this device variable will be temporary forced to a fixed value 
      p->m_ucStatus = request->deviceVariableStatus;
      g_stDeviceVariables[p - g_stDeviceVariableNvData].m_fValue = request->value;
      g_stDeviceVariables[p - g_stDeviceVariableNvData].m_fLastValue = request->value;
      
      g_stDeviceVariables[p - g_stDeviceVariableNvData].m_ucNormalOperation = 1; // no update more for the engineering value
    }  
  
    response->writeCommandCode = request->writeCommandCode;
    response->deviceVariableCode = request->deviceVariableCode;
    response->unitsCode = request->unitsCode;
    response->value = g_stDeviceVariables[p - g_stDeviceVariableNvData].m_fValue;
    response->deviceVariableStatus = p->m_ucStatus;
  } 
  else
  {
    //return RCS_E10_InvalidWriteDeviceVariableCode; 
  }  
  SET_CONFIG_CHANGED
  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_W08_DeviceFamilyBitNotSet;  
//  return RCS_E10_InvalidWriteDeviceVariableCode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E17_InvalidDeviceVariableIndex;
//  return RCS_E18_InvalidUnitsCode;
//  return RCS_E19_DeviceVariableIndexNotAllowed;  
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C079_WriteDeviceVariable()


/*********** CMD 080 *************************/

uint16_t Execute_C080_ReadDeviceVariableTrimPoints(C080_ReadDeviceVariableTrimPoints_Req* request,
                                     C080_ReadDeviceVariableTrimPoints_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E17_InvalidDeviceVariableIndex;
//  return RCS_E19_DeviceVariableIndexNotAllowed;  
//  return RCS_E32_Busy;
  
} // End - Execute_C080_ReadDeviceVariableTrimPoints()


/*********** CMD 081 *************************/

uint16_t Execute_C081_ReadDeviceVariableTrimGuidelines(C081_ReadDeviceVariableTrimGuidelines_Req* request,
                                     C081_ReadDeviceVariableTrimGuidelines_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E17_InvalidDeviceVariableIndex;
//  return RCS_E19_DeviceVariableIndexNotAllowed;  
//  return RCS_E32_Busy;
  
} // End - Execute_C081_ReadDeviceVariableTrimGuidelines()


/*********** CMD 082 *************************/

uint16_t Execute_C082_WriteDeviceVariableTrimPoint(C082_WriteDeviceVariableTrimPoint_Req* request,
                                     C082_WriteDeviceVariableTrimPoint_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_E09_AppliedProcessTooHigh;  
//  return RCS_E10_AppliedProcessTooHigh;
//  return RCS_E11_TrimError;  
//  return RCS_E13_ComputationError;
//  return RCS_W14_SpanTooSmall;
//  return RCS_E16_AccessRestricted;
//  return RCS_E17_InvalidDeviceVariableIndex;
//  return RCS_E18_InvalidUnitsCode;
//  return RCS_E19_DeviceVariableIndexNotAllowed;  
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C082_WriteDeviceVariableTrimPoint()


/*********** CMD 083 *************************/

uint16_t Execute_C083_ResetDeviceVariableTrim(C083_ResetDeviceVariableTrim_Req* request,
                                     C083_ResetDeviceVariableTrim_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_E16_AccessRestricted;
//  return RCS_E17_InvalidDeviceVariableIndex;
//  return RCS_E19_DeviceVariableIndexNotAllowed;  
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C083_ResetDeviceVariableTrim()


/*********** CMD 084 *************************/

uint16_t Execute_C084_ReadSubDeviceIdentitySummary(C084_ReadSubDeviceIdentitySummary_Req* request,
                                     C084_ReadSubDeviceIdentitySummary_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
  
} // End - Execute_C084_ReadSubDeviceIdentitySummary()


/*********** CMD 085 *************************/

uint16_t Execute_C085_ReadIOChannelStatistics(C085_ReadIOChannelStatistics_Req* request,
                                     C085_ReadIOChannelStatistics_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E21_InvalidCardNumber;  
//  return RCS_E22_InChannelNumber;
//  return RCS_E32_Busy;
  
} // End - Execute_C085_ReadIOChannelStatistics()


/*********** CMD 086 *************************/

uint16_t Execute_C086_ReadSubDeviceStatistics(C086_ReadSubDeviceStatistics_Req* request,
                                     C086_ReadSubDeviceStatistics_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
  
} // End - Execute_C086_ReadSubDeviceStatistics()


/*********** CMD 087 *************************/

uint16_t Execute_C087_WriteIOSystemMasterMode(C087_WriteIOSystemMasterMode_Req* request,
                                     C087_WriteIOSystemMasterMode_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_E16_AccessRestricted;
} // End - Execute_C087_WriteIOSystemMasterMode()


/*********** CMD 088 *************************/

uint16_t Execute_C088_WriteIOSystemRetryCount(C088_WriteIOSystemRetryCount_Req* request,
                                     C088_WriteIOSystemRetryCount_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{ 
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_E16_AccessRestricted;
} // End - Execute_C088_WriteIOSystemRetryCount()


/*********** CMD 089 *************************/

uint16_t Execute_C089_SetRealTimeClock(C089_SetRealTimeClock_Req* request,
                                     C089_SetRealTimeClock_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{ 
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_E16_AccessRestricted;
  
} // End - Execute_C089_SetRealTimeClock()


/*********** CMD 090 *************************/

uint16_t Execute_C090_ReadRealTimeClock(C090_ReadRealTimeClock_Req* request,
                                     C090_ReadRealTimeClock_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{ 
  if( g_stUTCatASN0.m_ucUTCReceived )
  { 
    unsigned long long ullTmp;
    TIME_DATE_T stCurrentUTC;
    
    // convert UTC into 10ms tics
    APP_DateToTime(&g_stUTCatASN0, &ullTmp);
    
    // calculate the current Time and Date
    APP_TimeToDate(ullTmp + g_sDllASN.m_ullASN, &stCurrentUTC);
    // current Time & Date
    response->currentDate.day   = stCurrentUTC.m_ucDay;
    response->currentDate.month = stCurrentUTC.m_ucMonth;
    response->currentDate.year  = stCurrentUTC.m_ucYear;
    response->currentTime.u32   = stCurrentUTC.m_ulTimeOfDay; 
    
    // calculate the last Time and Date when the clock was set
    APP_TimeToDate(ullTmp + g_stUTCatASN0.m_ullASN, &stCurrentUTC);
    // last Time & Date the clock was set
    response->lastDateSet.day   = stCurrentUTC.m_ucDay;
    response->lastDateSet.month = stCurrentUTC.m_ucMonth;
    response->lastDateSet.year  = stCurrentUTC.m_ucYear;
    response->lastTimeSet.u32   = stCurrentUTC.m_ulTimeOfDay; 
    
    // TBD
    response->rtcFlags = 0x01; // non-volatile??? clock (clock was already set but it's not non-volatile !!)
  }
  else
  {
    // calculate the current Time and Date
    APP_TimeToDate(g_sDllASN.m_ullASN, &g_stUTCatASN0);
    
    response->currentDate.day   = g_stUTCatASN0.m_ucDay;
    response->currentDate.month = g_stUTCatASN0.m_ucMonth;
    response->currentDate.year  = g_stUTCatASN0.m_ucYear;
    response->currentTime.u32   = g_stUTCatASN0.m_ulTimeOfDay; 
    
    response->lastDateSet.day   = 0x01;
    response->lastDateSet.month = 0x01;
    response->lastDateSet.year  = 0x00; // year 1900 
    response->lastTimeSet.u32   = 0; // 00:00:00
  
    response->rtcFlags = 0x02; // clock uninitialized
  }
  
  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
  
} // End - Execute_C090_ReadRealTimeClock()


/*********** CMD 091 *************************/

uint16_t Execute_C091_ReadTrendConfiguration(C091_ReadTrendConfiguration_Req* request,
                                     C091_ReadTrendConfiguration_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError; 
//  return RCS_E11_InvalidTrendNumber;
  
} // End - Execute_C091_ReadTrendConfiguration()


/*********** CMD 092 *************************/

uint16_t Execute_C092_WriteTrendConfiguration(C092_WriteTrendConfiguration_Req* request,
                                     C092_WriteTrendConfiguration_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode; 
//  return RCS_W08_SetToNearestPossibleValue;   
//  return RCS_E11_InvalidTrendNumber;  
//  return RCS_E16_AccessRestricted;
//  return RCS_E17_InvalidDeviceVariableIndex;
  
} // End - Execute_C092_WriteTrendConfiguration()


/*********** CMD 093 *************************/

uint16_t Execute_C093_ReadTrend(C093_ReadTrend_Req* request,
                                     C093_ReadTrend_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError; 
//  return RCS_W08_TrendNotActive;   
//  return RCS_E11_InvalidTrendNumber;  
  
} // End - Execute_C093_ReadTrend()


/*********** CMD 094 *************************/

uint16_t Execute_C094_ReadIOSystemClientSideCommunicationStatistics(C094_ReadIOSystemClientSideCommunicationStatistics_Req* request,
                                     C094_ReadIOSystemClientSideCommunicationStatistics_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
  
} // End - Execute_C094_ReadIOSystemClientSideCommunicationStatistics()


/*********** CMD 095 *************************/

uint16_t Execute_C095_ReadDeviceCommunicationsStatistics(C095_ReadDeviceCommunicationsStatistics_Req* request,
                                     C095_ReadDeviceCommunicationsStatistics_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
  
} // End - Execute_C095_ReadDeviceCommunicationsStatistics()


/*********** CMD 096 *************************/

uint16_t Execute_C096_ReadSynchronousAction(C096_ReadSynchronousAction_Req* request,
                                     C096_ReadSynchronousAction_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
  
} // End - Execute_C096_ReadSynchronousAction()


/*********** CMD 097 *************************/

uint16_t Execute_C097_ConfigureSynchronousAction(C097_ConfigureSynchronousAction_Req* request,
                                     C097_ConfigureSynchronousAction_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_W08_SamplingTimeAdjusted; 
//  return RCS_E09_BadTriggerAction;  
//  return RCS_E10_InvalidDate;
//  return RCS_E11_InvalidTime; 
//  return RCS_E12_InvalidDeviceVariable;
//  return RCS_E13_CommandNumberNotSupported;
//  return RCS_E16_AccessRestricted; 
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C097_ConfigureSynchronousAction()


/*********** CMD 098 *************************/

uint16_t Execute_C098_ReadCommandAction(C098_ReadCommandAction_Req* request,
                                     C098_ReadCommandAction_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
  
} // End - Execute_C098_ReadCommandAction()


/*********** CMD 099 *************************/

uint16_t Execute_C099_ConfigureCommandAction(C099_ConfigureCommandAction_Req* request,
                                     C099_ConfigureCommandAction_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_W08_SetToNearest; 
//  return RCS_E09_BadTriggerAction;  
//  return RCS_E10_BadCommandData;
//  return RCS_E13_CommandNumberNotSupported;
//  return RCS_E16_AccessRestricted; 
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C099_ConfigureCommandAction()


/*********** CMD 101 *************************/

uint16_t Execute_C101_ReadSubDeviceToBurstMessageMap(C101_ReadSubDeviceToBurstMessageMap_Req* request,
                                     C101_ReadSubDeviceToBurstMessageMap_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  response->burstMessage = request->burstMessage;
  response->subDeviceIndex = 0; // index 0 indicates the I/O system itself (tbd !!)

  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;  
  
} // End - Execute_C101_ReadSubDeviceToBurstMessageMap()


/*********** CMD 102 *************************/

uint16_t Execute_C102_MapSubDeviceToBurstMessage(C102_MapSubDeviceToBurstMessage_Req* request,
                                     C102_MapSubDeviceToBurstMessage_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE

  *response = *(C102_MapSubDeviceToBurstMessage_Resp *)request; // index 0 indicates the I/O system itself
  
  SET_CONFIG_CHANGED
  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode,
//  return RCM_E09_InvalidSubDeviceIndex,
//  return RCS_E16_AccessRestricted;  
  
} // End - Execute_C102_MapSubDeviceToBurstMessage()


/*********** CMD 103 *************************/
uint16_t Execute_C103_WriteBurstPeriod(C103_WriteBurstPeriod_Req* request,
                                     C103_WriteBurstPeriod_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE

  if( request->burstMessage >= MAX_BURST_NO )
    return RCM_E09_InvalidBurstMessage;

  //if a srvc req for this burst is pending, do not overwrite settings!
  if(REQNM_FindSrvcCmdPending(request->burstMessage + 1))
    return RCS_E32_Busy;
  
  TIME_10MS ulPeriod = request->updatePeriod.u32 / (32 * 10); // [10ms]
  TIME_10MS ulMaxPeriod = request->maxUpdatePeriod.u32 / (32 * 10); // [10ms]
  
  // apply correction
  /*uint8 ucCorrection = */APP_DoBurstEventTimerCorrection(&ulPeriod, &ulMaxPeriod);  
  
  response->burstMessage = request->burstMessage;
  response->updatePeriod.u32 = ulPeriod * (32 * 10);
  response->maxUpdatePeriod.u32 = ulMaxPeriod * (32 * 10);
  //from now on will compare possible existing time settings with response members, since correction was made.
  
  uint8 ucSrvcIdx = FindServiceIndex(request->burstMessage + 1, NL_SEARCH_BY_SERVICE_ID);
  if(NET_INVALID_IDX != ucSrvcIdx)
  { 
    if(g_aServicesTable[ucSrvcIdx].m_tPeriod.u32 != response->updatePeriod.u32)
    {
      if(g_stC769.joinStatus & JoinProcessStatusMask_NormalOperationCommencing)
      {
        DR_SLAVE_BUFFER_STRUCT* pDRBuf = NULL;
        if(g_aServicesTable[ucSrvcIdx].m_tPeriod.u32 > response->updatePeriod.u32) //bandwidth reallocation
        {
          pDRBuf = DRM_GetInactiveDRBuf();
          if( !pDRBuf )
            return RCS_E32_Busy;
        }
        // issue a RequestService(Cmd799) to the NM for this burst/DR buffer
        uint8 ucQIdx;
        REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
        if(!p) // no entry available, cannot send the request to NM
          return RCS_E32_Busy; 
        
        g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_BandwidthAllocationPending;
        APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
        p->m_unCmdId = CMDID_C799_RequestService;
        p->m_pDRBuf = pDRBuf;
        p->m_stC799Req.m_ucServiceRequestFlags = ServiceRequestFlagsMask_Source; 
        p->m_stC799Req.m_ucServiceApplicationDomain = ServiceApplicationDomain_Publish;
        p->m_stC799Req.m_unNicknameOfPeer = GW_NICKNAME;   
        p->m_stC799Req.m_tPeriod.u32 = response->updatePeriod.u32;
        p->m_stC799Req.m_ucServiceId = request->burstMessage + 1;
        if(pDRBuf)
          return RCS_N256_ReturnWithDR;   // will be managed by APP_ProcessCmdBuf()
      }
      else
        return RCS_E32_Busy;  // wait for device to enter NormalOperationCommencing status
    }
  }
  
     
  SET_CONFIG_CHANGED
  NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData + request->burstMessage;

  
  if(  (pCrtBurst->m_ulMinUpdateTime != ulPeriod) 
     ||(pCrtBurst->m_ulMaxUpdateTime != ulMaxPeriod)  )
  {
    pCrtBurst->m_ulMinUpdateTime = ulPeriod; // [10ms]
    pCrtBurst->m_ulMaxUpdateTime = ulMaxPeriod; // [10ms]    
    // NVM -> store the individual record into FLASH 
    uint8 ucId; // data identifier
    ucId = 77 + 5*request->burstMessage;  
    NVM_WriteIndividualRecord( ucId, 
                               (uint8*)g_stBurstModeTableNvData + c_aProvNvmTable[ucId-1].m_unOffset, 
                               c_aProvNvmTable[ucId-1].m_ucSize); 
    

    #ifdef BURST_PIPE_100MS  
    
      if( (pCrtBurst->m_ulMinUpdateTime > 10) && (g_stBurstPipe100ms.m_ucIdx == request->burstMessage) )
      {
        // re-init the 100ms pipe
        memset((void*)&g_stBurstPipe100ms, 0xFF, sizeof(g_stBurstPipe100ms)); 
      }  
      
    #endif
      
    // init min/max update timer
    MinUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] = ulPeriod;
    MaxUpdateTimer[pCrtBurst - g_stBurstModeTableNvData] = ulMaxPeriod; 

  }
  if( (request->updatePeriod.u32 != response->updatePeriod.u32) ||
      (request->maxUpdatePeriod.u32 != response->maxUpdatePeriod.u32))
    return RCM_W08_UpdateTimesAdjusted; 

  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_W08_UpdateTimesAdjusted; 
//  return RCS_E09_InvalidBurstMessage;  
//  return RCS_E16_AccessRestricted; 
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C103_WriteBurstPeriod()


/*********** CMD 104 *************************/

uint16_t Execute_C104_WriteBurstTrigger(C104_WriteBurstTrigger_Req* request,
                                     C104_WriteBurstTrigger_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  uint8 ucLocalStatus=RCS_N00_Success;
  uint8 ucValue;
   
  if( request->burstMessage >= MAX_BURST_NO )
    return RCM_E09_InvalidBurstMessage;
  
  SET_CONFIG_CHANGED
    
  *response = *(C104_WriteBurstTrigger_Resp *)request;  
  
  NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData + request->burstMessage;
  
/*  
  //TODO validate units code 
*/  
  if(  (request->selectionCode != pCrtBurst->m_ucBurstTriggerModeCode)
     ||(request->classificationCode != pCrtBurst->m_ucClassification)
     ||(request->unitsCode != pCrtBurst->m_ucUnit)     
     ||(request->triggerLevel != pCrtBurst->m_fTriggerValue)  )
  {  
    if( request->selectionCode > BurstMessageTriggerMode_OnChange )
    { // unsupported trigger mode -> continuous mode, value=0
      pCrtBurst->m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;
      pCrtBurst->m_fTriggerValue = 0;
      response->selectionCode = BurstMessageTriggerMode_Continuous;
      response->triggerLevel = 0;
      ucLocalStatus = RCM_E13_InvalidBurstTriggerModeSelectionCode;
    }
    else
    {
      pCrtBurst->m_ucBurstTriggerModeCode = request->selectionCode;
      pCrtBurst->m_fTriggerValue = request->triggerLevel;
    }
  
    pCrtBurst->m_ucClassification = request->classificationCode;
    pCrtBurst->m_ucUnit = request->unitsCode;
    //ucValue = request->unitsCode;
//    if( ((ucValue >= 1) && (ucValue <= 169)) || ((ucValue >= 220) && (ucValue <= 239)) )
    //TODO: should be accepted only the unit codes related to the specific variable types supported by the device???
    /*
    if( ((ucValue >= 1) && (ucValue <= 253)) ) //!!!!!!!!!!!!!!!!!!!!!!
      pCrtBurst->m_ucUnit = ucValue;
    else 
      ucLocalStatus = RCM_E12_InvalidUnitsCode;
    */
    
    
    // NVM -> store the individual record into FLASH
    ucValue = 78 + 5*request->burstMessage;  
    NVM_WriteIndividualRecord( ucValue, 
                               (uint8*)g_stBurstModeTableNvData + c_aProvNvmTable[ucValue-1].m_unOffset, 
                               c_aProvNvmTable[ucValue-1].m_ucSize); 
     
  }

  return ucLocalStatus;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_E09_InvalidBurstMessage;  
//  return RCS_E11_InvalidDeviceVariableClassification;  
//  return RCS_E12_InvalidUnitsCode;  
//  return RCS_E13_InvalidBurstTriggerModeSelectionCode;
//  return RCS_E16_AccessRestricted; 
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C104_WriteBurstTrigger()


/*********** CMD 105 *************************/

uint16_t Execute_C105_ReadBurstModeConfiguration( C105_ReadBurstModeConfiguration_Req* request,
                                                 C105_ReadBurstModeConfiguration_Resp* response,
                                                 SHORT_ADDR srcAddr,
                                                 uint8_t control, uint8_t remainingBytes)
{
  
  if( request->burstMessage >= MAX_BURST_NO )
    return RCM_E09_InvalidBurstMessage;
  
  NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData;
  if( !request->requestIsEmpty )
  {
    // request not empty -> search for requested pipe
    pCrtBurst += request->burstMessage;
    response->commandNo = 0x1F; // command number expansion flag
  }
  else
  { // empty request -> read pipe 0
    response->commandNo = (uint8)pCrtBurst->m_unCmdNo;
  }
  
  // read burst pipe configuration

  response->burstMode = pCrtBurst->m_ucBurstModeControlCode;
  memcpy(response->deviceVariableCode, pCrtBurst->m_aucDeviceVariableCodes, 8);
 
  response->burstMessage = pCrtBurst - g_stBurstModeTableNvData;
  response->maxBurstMessagesSupported = MAX_BURST_NO;
  response->extendedCommandNumber = pCrtBurst->m_unCmdNo;
  response->updateTime.u32 = pCrtBurst->m_ulMinUpdateTime;
  response->maxUpdateTime.u32 = pCrtBurst->m_ulMaxUpdateTime;
  response->updateTime.u32 *= 32 * 10; // [1/32 ms]
  response->maxUpdateTime.u32 *= 32 * 10; // [1/32 ms]
  response->burstTriggerMode = pCrtBurst->m_ucBurstTriggerModeCode;
  response->classificationCode = pCrtBurst->m_ucClassification;
  response->unitsCode = pCrtBurst->m_ucUnit;
  response->triggerValue = pCrtBurst->m_fTriggerValue;

  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_BurstConditionConflict;
//  return RCM_E09_InvalidBurstMessage;
  
} // End - Execute_C105_ReadBurstModeConfiguration()


/*********** CMD 106 *************************/

uint16_t Execute_C106_FlushDelayedResponses(C106_FlushDelayedResponses_Req* request,
                                     C106_FlushDelayedResponses_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  //TODO: check if g_aReqNMQueue[ucIdx].m_pDRBuf corresponds to any of the "to flush" drs. maybe should return RCS_E32_Busy?
  uint8 ucRetCode = RCS_N00_Success;
  
  #ifdef DR_USE_MORE_BUFFERS
    for(uint8 ucIdx=0; ucIdx<MAX_DR_SLAVE_BUFFERS; ucIdx++)
    {
      if(g_aDRSlaveBuf[ucIdx].m_unSrcAddr == srcAddr)
      {
        switch(g_aDRSlaveBuf[ucIdx].m_ucDRStatus){
          //case DRSLAVE_INACTIVE:
          //case DRSLAVE_START:
          case DRSLAVE_INITIATE:
          case DRSLAVE_RUNNING:
              ucRetCode = RCM_W08_AllButRunningDelayedResponsesFlushed;
              break;
          //case DRSLAVE_END:
          //case DRSLAVE_DEAD:
          default:
              g_aDRSlaveBuf[ucIdx].m_ucDRStatus = DRSLAVE_INACTIVE;
              break;
        }
      }
    }
  #else 
    switch(g_stDRSlaveBuf.m_ucDRStatus)
    {
      //case DRSLAVE_INACTIVE:
      //case DRSLAVE_START:
      case DRSLAVE_INITIATE:
      case DRSLAVE_RUNNING:
        ucRetCode = RCM_W08_AllButRunningDelayedResponsesFlushed;
        break;
      //case DRSLAVE_END:
      //case DRSLAVE_DEAD:
      default:
        g_stDRSlaveBuf.m_ucDRStatus = DRSLAVE_INACTIVE;
        break;  
    }
  #endif //DR_USE_MORE_BUFFERS  
  
  return ucRetCode;
 
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_W08_AllButRunningDelayedResponsesFlushed;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;

  
} // End - Execute_C106_FlushDelayedResponses()


/*********** CMD 107 *************************/

uint16_t Execute_C107_WriteBurstDeviceVariables(C107_WriteBurstDeviceVariables_Req* request,
                                     C107_WriteBurstDeviceVariables_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
    
  if( request->burstMessage >= MAX_BURST_NO )
    return RCM_E09_InvalidBurstMessage;
  
  uint8 ucIdx;  

  uint8 ucDevVarsNo = sizeof(g_stDeviceVariableNvData)/sizeof(*g_stDeviceVariableNvData);
  for( uint8 ucIdxReq =0; ucIdxReq < request->noOfDeviceVariables; ucIdxReq++ )
  {
    if( request->deviceVariableCode[ucIdxReq] == DeviceVariableCodes_NotUsed )
      continue;    //allow NotUsed, actually prepare burst function will take care of truncation
    
   
#if ( _UAP_TYPE != UAP_TYPE_SIMPLE_API )    
    //search if dev var code instead
    uint8 ucFoundDevVarCode = 0;
       
    for(ucIdx=0; ucIdx<ucDevVarsNo ; ucIdx++)
    {
      //this is for dev var codes 0, 1, 2...NoOfDeviceVariables
      if(request->deviceVariableCode[ucIdxReq] == g_stDeviceVariableNvData[ucIdx].m_ucCode)
        ucFoundDevVarCode = 1;
    }
    if(ucFoundDevVarCode)
      continue; //go to next iteration, found a "good" code
    
    //search if dynamic code instead
    uint8 ucFoundDynamic = 0;
       
    for(ucIdx=0; ucIdx<NoOfDynamicVariables ; ucIdx++)
    {
      //this is for dev var codes 0, 1, 2...NoOfDeviceVariables
      if(request->deviceVariableCode[ucIdxReq] == c_aVarMap[ucIdx])
        ucFoundDynamic = 1;
    }
    
    if(!ucFoundDynamic) //not even dynamic var code, means that a code not supported by dev is found in payload; return!
      return RCS_E02_InvalidSelection;    
    
#else
  if(request->deviceVariableCode[ucIdxReq] < ucDevVarsNo)
    continue; //go to next iteration, found a "good" code    
  
  #if (BOARD_TYPE_HART_DEV_KIT == BOARD_TYPE)

    for(ucIdx = 0; ucIdx < LOCAL_ANALOG_CH_NO ; ucIdx++)
    { 
      if( request->deviceVariableCode[ucIdxReq] == g_aHartDVCodes[ucIdx] )
        break;    
    }
    
    if( ucIdx < LOCAL_ANALOG_CH_NO )
      continue;  //go to next iteration
     
  #elif (BOARD_TYPE_DEVELOPMENT == BOARD_TYPE)    
    if((request->deviceVariableCode[ucIdxReq] >= DeviceVariableCodes_PrimaryVariable) && (request->deviceVariableCode[ucIdxReq] <= DeviceVariableCodes_TertiaryVariable))
      continue;
  #endif 
  //at this point no var found.  
  return RCS_E02_InvalidSelection;  
#endif    
    
  }   
 
  uint8 ucLocalStatus = RCS_N00_Success;

  SET_CONFIG_CHANGED
  NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData + request->burstMessage;
      
  if( memcmp(request->deviceVariableCode, pCrtBurst->m_aucDeviceVariableCodes, 8) )
  {  
    for(ucIdx=0; ucIdx<8; ucIdx++)
    {
      if( ucIdx < request->noOfDeviceVariables )
        pCrtBurst->m_aucDeviceVariableCodes[ucIdx] = request->deviceVariableCode[ucIdx];
      else
        pCrtBurst->m_aucDeviceVariableCodes[ucIdx] = DeviceVariableCodes_NotUsed;
    }
  
    if( pCrtBurst->m_ucBurstTriggerModeCode != BurstMessageTriggerMode_Continuous )
    {
      if( pCrtBurst->m_ucClassification != g_stDeviceVariableNvData[0].m_ucClassification )
      {
        pCrtBurst->m_ucClassification       = g_stDeviceVariableNvData[0].m_ucClassification;
        pCrtBurst->m_ucUnit                 = g_stDeviceVariableNvData[0].m_ucUnit;
        pCrtBurst->m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;

        ucLocalStatus = RCM_W08_BurstConditionConflict;
      }
    }
        
    // NVM -> store the individual record into FLASH 
    uint8 ucId; // data identifier
    ucId = 79 + 5*request->burstMessage;  
    NVM_WriteIndividualRecord( ucId, 
                               (uint8*)g_stBurstModeTableNvData + c_aProvNvmTable[ucId-1].m_unOffset, 
                               c_aProvNvmTable[ucId-1].m_ucSize);
    
  } 
  
  memcpy(response->deviceVariableCode, pCrtBurst->m_aucDeviceVariableCodes, 8);
  response->burstMessage = request->burstMessage;   
    
  return ucLocalStatus;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode; 
//  return RCS_W08_BurstConditionConflict;   
//  return RCS_E09_InvalidBurstMessage; 
  
} // End - Execute_C107_WriteBurstDeviceVariables()


/*********** CMD 108 *************************/

uint16_t Execute_C108_WriteBurstModeCommandNumber(C108_WriteBurstModeCommandNumber_Req* request,
                                     C108_WriteBurstModeCommandNumber_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
    
  if( request->burstMessage >= MAX_BURST_NO )
    return RCM_E09_InvalidBurstMessage;

#ifndef  PUBLISH_CMD784  
  if((1 != request->commandNo) &&
     (2 != request->commandNo) &&
     (3 != request->commandNo) &&
     (9 != request->commandNo) &&
     (48 != request->commandNo) &&
     (33 != request->commandNo))
    return RCS_E02_InvalidSelection;
#endif  
  
  SET_CONFIG_CHANGED
    
  NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData + request->burstMessage;
      
  if( request->commandNo != pCrtBurst->m_unCmdNo )
  {
    pCrtBurst->m_unCmdNo = request->commandNo;
      
    //TODO: add burst conflict condition
    
    // NVM -> store the individual record into FLASH 
    uint8 ucId; // data identifier
    ucId = 80 + 5*request->burstMessage;  
    NVM_WriteIndividualRecord( ucId, 
                               (uint8*)g_stBurstModeTableNvData + c_aProvNvmTable[ucId-1].m_unOffset, 
                               c_aProvNvmTable[ucId-1].m_ucSize);  
    
  }
  *response = *request;
   
  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode; 
//  return RCS_W08_BurstConditionConflict;   
//  return RCS_E09_InvalidBurstMessage;   
  
} // End - Execute_C108_WriteBurstModeCommandNumber()


/*********** CMD 109 *************************/

uint16_t Execute_C109_BurstModeControl(C109_BurstModeControl_Req* request,
                                     C109_BurstModeControl_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  
  uint8 ucId; // data identifier
  // our wireless device/adapter doesn't support burst mode (publish) on wired interface
  // C109_ReqSize is set to 1, for backward compatibility
                              //  return RCS_E05_TooFewDataBytesReceived;
    
  CK_WP_MODE;                 //  return RCS_E07_InWriteProtectMode;
  CK_LOCK_STATUS;             //  return RCS_E16_AccessRestricted;
  
  
  if( request->backComp && (request->burstModeCode > BurstModeControlCodes_EnableBurstOnTokenPassingDataLinkLayer) )
    return RCS_E05_TooFewDataBytesReceived;  // see backward compatibility spec
  
  if( (request->burstMessage >= MAX_BURST_NO) || 
      (request->burstModeCode != BurstModeControlCodes_Off && 
       request->burstModeCode != BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer) )
    return RCS_E02_InvalidSelection;  // invalid Burst Message or trying to publish on wired stack
  
  uint8 ucServiceId = request->burstMessage + 1;  // fixed serviceId allocation
  
  #ifndef DRM_REDESIGN
    if(REQNM_FindSrvcCmdPending(ucServiceId))
          return RCS_E32_Busy;
  #endif
  
  *response = *request;
  
  NV_BURST_MODE_T *pCrtBurst = g_stBurstModeTableNvData + request->burstMessage;
  if(pCrtBurst->m_ucBurstModeControlCode == request->burstModeCode)
  {
    SET_CONFIG_CHANGED
    return RCS_N00_Success;   // nothing to do
  }

  /* if this request has arrived before the function APP_SrvcTask() processes the burst msg config, save the control code into the NVM */
  if (g_stC769.joinStatus <= JoinProcessStatusMask_NegotiatingNetworkProperties)
  {
    pCrtBurst->m_ucBurstModeControlCode = request->burstModeCode;
    // NVM -> store the individual record into FLASH 
    ucId = 81 + 5*request->burstMessage;  
    NVM_WriteIndividualRecord( ucId, 
                             (uint8*)g_stBurstModeTableNvData + c_aProvNvmTable[ucId-1].m_unOffset, 
                             c_aProvNvmTable[ucId-1].m_ucSize);
  }
  else
  {
    /* the device is in NormalOperationCommencing */
    if(request->burstModeCode == BurstModeControlCodes_Off)
    { // stop publishing only if the device is in NormalOperationCommencing state
      // TODO: 
      // -check if there is a pending C799 request to NM for this burst/serviceId and flush it
      // if burst/service tables already contain a service allocated to this burst, start a C801 request to NM to delete it
      
#ifdef DRM_REDESIGN 
     
      REQNM_QEUE_T* p799 = REQNM_FindSrvcCmdPending(ucServiceId);
      if(NULL!=p799)
      {
        if(CMDID_C799_RequestService == p799->m_unCmdId)
        {
          if(NULL != p799->m_pDRBuf)
          {
            BinaryStream stStreamDR;
            STREAM_INIT(&stStreamDR, p799->m_pDRBuf->m_aResp, sizeof(p799->m_pDRBuf->m_aResp));    
            //compose DR response
            ComposeResponseToCommmand(p799->m_pDRBuf->m_unSrcAddr,                 
                                      NULL, 
                                      p799->m_pDRBuf->m_unCmdId, 
                                      NULL, 
                                      RCS_E33_DelayedResponseInitiated,
                                      &stStreamDR
                                     );    
            p799->m_pDRBuf->m_ucRespSize = stStreamDR.nextByte - p799->m_pDRBuf->m_aResp;
            p799->m_pDRBuf->m_ucDRStatus = DRSLAVE_END; //DR will be deleted in the next APP queue task pass
            APP_QueueTask();
            memset(p799->m_pDRBuf, 0, sizeof(DR_SLAVE_BUFFER_STRUCT));
          }
          
          p799->m_ucStatus = REQNM_SUCCESS; //no more retries
        }
      }
#endif      
      
      if(
         (FindServiceIndex(ucServiceId, NL_SEARCH_BY_SERVICE_ID) < g_ucServicesNo))
      {
        
        uint8 ucQIdx;
        REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
        if(!p) // no entry available, cannot send DeleteService request to NM
          return RCS_E32_Busy; 
    
        p->m_unCmdId = CMDID_C801_DeleteService;
        p->m_pDRBuf = NULL;      // no DR buffer used
        p->m_stC801Req.m_ucServiceId = ucServiceId;
        p->m_stC801Req.m_ucReason = ServiceDeletionReasonCodes_RequestedByPeer; // CommonTable 49
      }
      
      pCrtBurst->m_ucBurstModeControlCode = BurstModeControlCodes_Off;
      g_stBurstModeTable[request->burstMessage].m_ucServiceId = 0; // set to the "no service allocated for this burst" value 
      g_stBurstModeTable[request->burstMessage].m_ucFirstBurstSent = 0; // clear burst flag 

      // NVM -> store the individual record into FLASH 
      ucId = 81 + 5*request->burstMessage;  
      NVM_WriteIndividualRecord( ucId, 
                               (uint8*)g_stBurstModeTableNvData + c_aProvNvmTable[ucId-1].m_unOffset, 
                               c_aProvNvmTable[ucId-1].m_ucSize); 
    }
    else
    { /* start publishing only if the device is in NormalOperationCommencing state */
      // before start publishing, has to initiate a service request to NM for this burst
/*  
    // ------------- this is ony temporary, for tests --------------------------
    pCrtBurst->m_ucBurstModeControlCode = BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer;
    g_stBurstModeTable[request->burstMessage].m_ucServiceId = request->burstMessage + 1;
    
    SET_CONFIG_CHANGED;
    return RCS_N00_Success;
    // ------------- this is ony temporary, for tests --------------------------
*/    
    
#ifdef DRM_REDESIGN       
      REQNM_QEUE_T* p801 = REQNM_FindSrvcCmdPending(ucServiceId);
      if(NULL!=p801)
      {
        if(CMDID_C801_DeleteService == p801->m_unCmdId)
        {
          if(REQNM_SEND == p801->m_ucStatus) //req did not get to be sent, so it's safe just to clean it!
            memset(p801, 0, sizeof(REQNM_QEUE_T));
          else 
            p801->m_ucStatus = REQNM_SUCCESS; //no more retries
        }
      }
#endif      
      
      DR_SLAVE_BUFFER_STRUCT* pDRBuf = DRM_GetInactiveDRBuf();
      if( !pDRBuf )
        return RCS_E32_Busy;
  
      // issue a RequestService(Cmd799) to the NM for this burst/DR buffer
      uint8 ucQIdx;
      REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
      if(!p) // no entry available, cannot send the request to NM
        return RCS_E32_Busy; 
      g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_BandwidthAllocationPending;
      APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
      p->m_unCmdId = CMDID_C799_RequestService;
      p->m_pDRBuf = pDRBuf;
      p->m_stC799Req.m_ucServiceRequestFlags = ServiceRequestFlagsMask_Source; 
      p->m_stC799Req.m_ucServiceApplicationDomain = ServiceApplicationDomain_Publish;
      p->m_stC799Req.m_unNicknameOfPeer = GW_NICKNAME;   
      p->m_stC799Req.m_tPeriod.u32 = pCrtBurst->m_ulMinUpdateTime * (32 * 10) ;
      p->m_stC799Req.m_ucServiceId = ucServiceId;
      return RCS_N256_ReturnWithDR;   // will be managed by APP_ProcessCmdBuf()
  
      // The requested burstModeCode will be saved (or not) to g_stBurstModeTable later, by DR Mechanism,
      // after NM will respond to the service allocation request. Then will be finalized the Exec_C109 flow
      // and the final C109 response will be composed.
    }
  }
  
  SET_CONFIG_CHANGED
  return RCS_N00_Success; 


// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_W08_UpdatePeriodIncreased;  
//  return RCS_E09_InsufficientBandwidth;
//  return RCS_E16_AccessRestricted; 
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C109_BurstModeControl()


/*********** CMD 113 *************************/

uint16_t Execute_C113_CatchDeviceVariable(C113_CatchDeviceVariable_Req* request,
                                     C113_CatchDeviceVariable_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
  //TODO:  
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_E10_InvalidCaptureModeCode;
//  return RCM_E11_InvalidSlotNumber;
//  return RCM_E12_CommandNumberNotSupported;
//  return RCS_E16_AccessRestricted;
//  return RCS_E17_InvalidDeviceVariableIndex;
//  return RCS_E19_DeviceVariableIndexNotAllowed;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;
} // End - Execute_C113_CatchDeviceVariable()


/*********** CMD 114 *************************/

uint16_t Execute_C114_ReadCaughtDeviceVariable(C114_ReadCaughtDeviceVariable_Req* request,
                                     C114_ReadCaughtDeviceVariable_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
  //TODO:  
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E17_InvalidDeviceVariableIndex;
//  return RCS_E19_DeviceVariableIndexNotAllowed;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;
} // End - Execute_C114_ReadCaughtDeviceVariable()


/*********** CMD 115 *************************/

uint16_t Execute_C115_ReadEventNotificationSummary(C115_ReadEventNotificationSummary_Req* request,
                                     C115_ReadEventNotificationSummary_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  if(request->eventNo >= MAX_EVENTS_NO)
    return RCS_E02_InvalidSelection;
  
  NV_EVENT_NOTIFICATION_T *pCrtEvent = g_stEventsTableNvData + request->eventNo;
   
  response->eventNo = request->eventNo;
  response->noOfEventsSupported = MAX_EVENTS_NO;
  
  
  if( EVENT_QUEUE_NOT_EMPTY )
  {  
    EVENT_PENDING_T* pEvent = g_stEventQueue; // active event
    
    response->firstUnacknowledged.u32 = pEvent->m_ulTimestamp; // [1/32 ms] 
    
    // get event status
    for(; pEvent < g_pEventQueueEnd; pEvent++)
    {
      response->eventStatus |= pEvent->m_ucEventStatus; 
    }  
  }
  else
  { // no event was triggered
    
    response->firstUnacknowledged.u32 = 0xFFFFFFFF;
    
    response->eventStatus = 0x00; 
  }  
  
  response->eventNotificationControlCode = pCrtEvent->m_ucEventControlCode;  
  response->eventNotificationRetryTime.u32 = pCrtEvent->m_ulRetryTime * 10 * 32; // [1/32 ms]
  response->maximumUpdateTime.u32 = pCrtEvent->m_ulMaxUpdateTime * 10 * 32; // [1/32 ms]
  response->eventDeBounceInterval.u32 = pCrtEvent->m_ulDebounceInterval * 10 * 32; // [1/32 ms]
  response->noOfReadEvents = pCrtEvent->m_ucMaskSize;
  
  uint8 ucIdx;
  for(ucIdx=0; ucIdx < pCrtEvent->m_ucMaskSize; ucIdx++)
    response->eventMask[ucIdx] = pCrtEvent->m_aucEventMasks[ucIdx];  
  
  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
  
} // End - Execute_C115_ReadEventNotificationSummary()


/*********** CMD 116 *************************/

uint16_t Execute_C116_WriteEventNotificationBitMask(C116_WriteEventNotificationBitMask_Req* request,
                                     C116_WriteEventNotificationBitMask_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  
  if(request->eventNo >= MAX_EVENTS_NO)
    return RCS_E02_InvalidSelection;
  
  SET_CONFIG_CHANGED
  *response = *request;
  uint8 ucNofEvents = request->noOfWriteEvents;
  memcpy(response->eventMask, request->eventMask, ucNofEvents);
  //truncated mask means the rest must be set on 0
  if(ucNofEvents < MaxNoOfWriteEvents)
  {
    memset(request->eventMask + ucNofEvents, 0, MaxNoOfWriteEvents-ucNofEvents);
  }   
  NV_EVENT_NOTIFICATION_T *pCrtEvent = g_stEventsTableNvData + request->eventNo;
  
  if( memcmp(request->eventMask, pCrtEvent->m_aucEventMasks, MaxNoOfWriteEvents) ) //compare all mask, not just req size, to include truncation scenario
  { 
    pCrtEvent->m_ucMaskSize = request->noOfWriteEvents;
    memcpy(pCrtEvent->m_aucEventMasks, request->eventMask, MaxNoOfWriteEvents);
    // NVM -> store the individual record into FLASH 
    uint8 ucId; // data identifier
    ucId = 107 + 3*request->eventNo;  
    NVM_WriteIndividualRecord( ucId, 
                               (uint8*)g_stEventsTableNvData + c_aProvNvmTable[ucId-1].m_unOffset, 
                               c_aProvNvmTable[ucId-1].m_ucSize);
    
  }
   
  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_E16_AccessRestricted; 
  
} // End - Execute_C116_WriteEventNotificationBitMask()


/*********** CMD 117 *************************/

uint16_t Execute_C117_WriteEventNotificationTiming(C117_WriteEventNotificationTiming_Req* request,
                                     C117_WriteEventNotificationTiming_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE

  if( request->eventNo >= MAX_EVENTS_NO )
    return RCM_E09_InvalidBurstMessage;
  
  //if a srvc req for this event is pending, do not overwrite settings!
  if(REQNM_FindSrvcCmdPending(request->eventNo + MAX_BURST_NO + 1))
    return RCS_E32_Busy;  
  
  TIME_10MS ulRetryTime = request->eventNotificationRetryTime.u32 / (32 * 10); // [10ms]
  TIME_10MS ulMaxUpdateTime = request->maximumUpdateTime.u32 / (32 * 10); // [10ms]
  TIME_10MS ulDebounceInterval = request->eventDeBounceInterval.u32 / (32 * 10); // [10ms] 
 
  // apply correction
  /*uint8 ucCorrection = */APP_DoBurstEventTimerCorrection(&ulRetryTime, &ulMaxUpdateTime);
   if( ulDebounceInterval > 100*3600 ) // > 3600s not allowed
     ulDebounceInterval = 100*3600;
   else
  /*ucCorrection = */BurstEventCorrection(&ulDebounceInterval);

  response->eventNo = request->eventNo;
  response->eventNotificationRetryTime.u32 = ulRetryTime * (32 * 10);
  response->maximumUpdateTime.u32 = ulMaxUpdateTime * (32 * 10);
  response->eventDeBounceInterval.u32 = ulDebounceInterval * (32 * 10);
  
  uint8 ucSrvcIdx = FindServiceIndex(request->eventNo + MAX_BURST_NO + 1, NL_SEARCH_BY_SERVICE_ID);
  if(NET_INVALID_IDX != ucSrvcIdx)
  { 
    if(g_aServicesTable[ucSrvcIdx].m_tPeriod.u32 != response->eventNotificationRetryTime.u32) 
    {
      if(g_stC769.joinStatus & JoinProcessStatusMask_NormalOperationCommencing)
      {
        DR_SLAVE_BUFFER_STRUCT* pDRBuf = NULL;
        if(g_aServicesTable[ucSrvcIdx].m_tPeriod.u32 > response->eventNotificationRetryTime.u32)
        {
          pDRBuf = DRM_GetInactiveDRBuf();
          if( !pDRBuf )
            return RCS_E32_Busy;
        }
        // issue a RequestService(Cmd799) to the NM for this burst/DR buffer
        uint8 ucQIdx;
        REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
        if(!p) // no entry available, cannot send the request to NM
          return RCS_E32_Busy; 
  
        g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_BandwidthAllocationPending;
        APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
        p->m_unCmdId = CMDID_C799_RequestService;
        p->m_pDRBuf = pDRBuf;
        p->m_stC799Req.m_ucServiceRequestFlags = ServiceRequestFlagsMask_Source/*|ServiceRequestFlagsMask_Sink*/; 
        p->m_stC799Req.m_ucServiceApplicationDomain = ServiceApplicationDomain_Event;
        p->m_stC799Req.m_unNicknameOfPeer = GW_NICKNAME;   
        p->m_stC799Req.m_tPeriod.u32 = response->eventNotificationRetryTime.u32;
        p->m_stC799Req.m_ucServiceId = request->eventNo + MAX_BURST_NO + 1;
        if(pDRBuf)
          return RCS_N256_ReturnWithDR;   // will be managed by APP_ProcessCmdBuf()
      }
      else   
        return RCS_E32_Busy;  // wait for device to enter NormalOperationCommencing status
    }
  }
   
  SET_CONFIG_CHANGED
    
  NV_EVENT_NOTIFICATION_T *pCrtEvent = g_stEventsTableNvData + request->eventNo;

  if(  (pCrtEvent->m_ulRetryTime != ulRetryTime)
     ||(pCrtEvent->m_ulMaxUpdateTime != ulMaxUpdateTime)
     ||(pCrtEvent->m_ulDebounceInterval != ulDebounceInterval)  )  
  {  
    pCrtEvent->m_ulRetryTime = ulRetryTime;
    pCrtEvent->m_ulMaxUpdateTime = ulMaxUpdateTime;
    pCrtEvent->m_ulDebounceInterval = ulDebounceInterval;
  
    // NVM -> store the individual record into FLASH 
    uint8 ucId; // data identifier
    ucId = 108 + 3*request->eventNo;  
    NVM_WriteIndividualRecord( ucId, 
                               (uint8*)g_stEventsTableNvData + c_aProvNvmTable[ucId-1].m_unOffset, 
                               c_aProvNvmTable[ucId-1].m_ucSize); 
    
   
    #ifdef BURST_PIPE_100MS
      
    if( (pCrtEvent->m_ulRetryTime > 10) && (g_stEventPipe100ms.m_ucIdx == request->eventNo) )
    {
      // re-init the 100ms pipe
      memset((void*)&g_stEventPipe100ms, 0xFF, sizeof(g_stEventPipe100ms)); 
    }  
      
    #endif
    
    // init retry/update timer
    RetryTimer[request->eventNo] = ulRetryTime;
    UpdateTimer[request->eventNo] = ulMaxUpdateTime;
  
  }
  
  if( (request->eventNotificationRetryTime.u32 != response->eventNotificationRetryTime.u32) ||
      (request->maximumUpdateTime.u32 != response->maximumUpdateTime.u32) ||
      (request->eventDeBounceInterval.u32 != response->eventDeBounceInterval.u32) )
    return RCM_W08_UpdatePeriodOrDebounceIntervalAdjusted;

  return RCS_N00_Success;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCM_W08_UpdatePeriodOrDebounceIntervalAdjusted;  
//  return RCM_E09_InvalidEventMessage;  !!! not RCM_E09_InvalidBurstMessage !!!
//  return RCS_E16_AccessRestricted; 
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C117_WriteEventNotificationTiming()


/*********** CMD 118 *************************/

uint16_t Execute_C118_EventNotificationControl(C118_EventNotificationControl_Req* request,
                                     C118_EventNotificationControl_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE;                 //  return RCS_E07_InWriteProtectMode;
  CK_LOCK_STATUS;             //  return RCS_E16_AccessRestricted;
  
  if( (request->eventNo >= MAX_EVENTS_NO) || 
      (request->eventNotificationControlCode != EventNotificationControlCodes_Off && 
       request->eventNotificationControlCode != EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer) )
    return RCS_E02_InvalidSelection;  // invalid eventNo or trying to publish on wired stack
    
  uint8 ucServiceId = request->eventNo + MAX_BURST_NO + 1;  // fixed serviceId allocation
#ifndef DRM_REDESIGN
  //if a srvc req for this event is pending, do not overwrite settings!
  if(REQNM_FindSrvcCmdPending(ucServiceId))
    return RCS_E32_Busy; 
#endif
  
  *response = *request;
  
  NV_EVENT_NOTIFICATION_T *pCrtEvent = g_stEventsTableNvData + request->eventNo;
  if( pCrtEvent->m_ucEventControlCode == request->eventNotificationControlCode ) // low nibble
  {
    SET_CONFIG_CHANGED
    return RCS_N00_Success;   // nothing to do
  }
  
  if( request->eventNotificationControlCode == EventNotificationControlCodes_Off)
  { // stop event notification
    // TODO: 
    // -save to NVMMemory, too
    
    if(g_stC769.joinStatus & JoinProcessStatusMask_NormalOperationCommencing)
    {
      // TODO: 
      // -check if there is a pending C799 request to NM for this event/serviceId and flush it
#ifdef DRM_REDESIGN 
      uint8 ucSrvcReqSent = 0;
     
      REQNM_QEUE_T* p799 = REQNM_FindSrvcCmdPending(ucServiceId);
      if(NULL!=p799)
      {
        if(CMDID_C799_RequestService == p799->m_unCmdId)
        {
          if(NULL != p799->m_pDRBuf)
          {
            BinaryStream stStreamDR;
            STREAM_INIT(&stStreamDR, p799->m_pDRBuf->m_aResp, sizeof(p799->m_pDRBuf->m_aResp));    
            //compose DR response
            ComposeResponseToCommmand(p799->m_pDRBuf->m_unSrcAddr,                 
                                      NULL, 
                                      p799->m_pDRBuf->m_unCmdId, 
                                      NULL, 
                                      RCS_E33_DelayedResponseInitiated,
                                      &stStreamDR
                                     );    
            p799->m_pDRBuf->m_ucRespSize = stStreamDR.nextByte - p799->m_pDRBuf->m_aResp;
            p799->m_pDRBuf->m_ucDRStatus = DRSLAVE_END; //DR will be deleted in the next APP queue task pass
            APP_QueueTask();
            memset(p799->m_pDRBuf, 0, sizeof(DR_SLAVE_BUFFER_STRUCT));
          }
          
          if(REQNM_SEND == p799->m_ucStatus) //req did not get to be sent, so it's safe just to clean it!
          {
            memset(p799, 0, sizeof(REQNM_QEUE_T));
          }
          else 
          {
             p799->m_ucStatus = REQNM_SUCCESS; //no more retries
             ucSrvcReqSent = 1;
           }
        }
      }
#endif  
      
      
      
      // if service table already contains a service allocated to this event, start a C801 request to NM to delete it
      if(
#ifdef DRM_REDESIGN          
         ucSrvcReqSent || 
#endif           
         (FindServiceIndex(ucServiceId, NL_SEARCH_BY_SERVICE_ID) < g_ucServicesNo))
      {
        uint8 ucQIdx;
        REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
        if(!p) // no entry availabe, cannot send DeleteService to NM
          return RCS_E32_Busy;
 
        p->m_unCmdId = CMDID_C801_DeleteService;
        p->m_pDRBuf = NULL;  // no DR buffer used
        p->m_stC801Req.m_ucServiceId = ucServiceId;
        p->m_stC801Req.m_ucReason = ServiceDeletionReasonCodes_RequestedByPeer; //CommonTable 49

      }
    }

    pCrtEvent->m_ucEventControlCode = EventNotificationControlCodes_Off;
    g_stEventsTable[request->eventNo].m_ucServiceId = 0; // set to the "no service allocated for this event" value
    
    
      
    // NVM -> store the individual record into FLASH 
    uint8 ucId; // data identifier
    ucId = 109 + 3*request->eventNo;  
    NVM_WriteIndividualRecord( ucId, 
                               (uint8*)g_stEventsTableNvData + c_aProvNvmTable[ucId-1].m_unOffset, 
                               c_aProvNvmTable[ucId-1].m_ucSize);
  }
  else if(g_stC769.joinStatus & JoinProcessStatusMask_NormalOperationCommencing)
  { // before starting event notification, a service request to NM has to be initiated for this event
/*  
    // ------------- this is ony temporary, for tests --------------------------
    pCrtEvent->m_ucEventControlCode = EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer; 
    g_stEventsTable[request->eventNo].m_ucServiceId = 0; // set to the "no service allocated for this event" value
    
    response->eventNo = request->eventNo;
    response->eventNotificationControlCode = request->eventNotificationControlCode;

    SET_CONFIG_CHANGED;
    return RCS_N00_Success;
    // ------------- this is ony temporary, for tests --------------------------
*/
#ifdef DRM_REDESIGN       
      REQNM_QEUE_T* p801 = REQNM_FindSrvcCmdPending(ucServiceId);
      if(NULL!=p801)
      {
        if(CMDID_C801_DeleteService == p801->m_unCmdId)
        {
          if(REQNM_SEND == p801->m_ucStatus) //req did not get to be sent, so it's safe just to clean it!
            memset(p801, 0, sizeof(REQNM_QEUE_T));
          else 
            p801->m_ucStatus = REQNM_SUCCESS; //no more retries
        }
      }
#endif
    DR_SLAVE_BUFFER_STRUCT* pDRBuf = DRM_GetInactiveDRBuf();
    if( !pDRBuf )
      return RCS_E32_Busy;

    // issue a RequestService(Cmd799) to the NM for this burst/DR buffer
    uint8 ucQIdx;
    REQNM_QEUE_T* p = REQNM_GetQueueEntry(&ucQIdx);
    if(!p) // no entry available, cannot send the request to NM
      return RCS_E32_Busy; 
        
    g_stCmd48Data.data.standardizedStatus3 |= StandardizedStatus3Mask_BandwidthAllocationPending;
    APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
    p->m_unCmdId = CMDID_C799_RequestService;
    p->m_pDRBuf = pDRBuf;
    p->m_stC799Req.m_ucServiceRequestFlags = ServiceRequestFlagsMask_Source/*|ServiceRequestFlagsMask_Sink*/; 
    p->m_stC799Req.m_ucServiceApplicationDomain = ServiceApplicationDomain_Event;
    p->m_stC799Req.m_unNicknameOfPeer = GW_NICKNAME;   
    p->m_stC799Req.m_tPeriod.u32 = pCrtEvent->m_ulRetryTime * (32 * 10) ;
    p->m_stC799Req.m_ucServiceId = ucServiceId;
    return RCS_N256_ReturnWithDR;   // will be managed by APP_ProcessCmdBuf()


    
    // TODO:
    //  if( DR buffers full or conflict between C118 and a pending DR command)
    //    return RCS_E32_Busy;
    //  else
    //  {
    //    -save necessary data to DR buffer
    //    -issue a RequestService(Cmd799) to the NM for this event/DR buffer
  
    //return RCS_N256_ReturnWithDR;   // will be managed by APP_ProcessCmdBuf()
    
    // The requested eventNotificationControlCode will be saved (or not) to g_stEventsTable later, by DR Mechanism,
    // after NM will respond to the service allocation request. Then will be finalized the Exec_C118 flow
    // and the final C118 response will be composed.
  }
  else //MP
  { // if event on AND not NormalOperationCommencing -> return RCS_E32_Busy
    //pCrtBurst->m_ucBurstModeControlCode = request->burstModeCode; 
    return RCS_E32_Busy; 
  }
  
  SET_CONFIG_CHANGED
  return RCS_N00_Success; 
  
  // TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCS_W08_UpdateTimesAdjusted;  
//  return RCS_W14_UpdateRateUncertain;
//  return RCS_E16_AccessRestricted; 
//  return RCS_E32_Busy;
//  return RCS_E33_DrInitiated;
//  return RCM_E34_DrRunning;
//  return RCM_E35_DrDead;
//  return RCS_E36_DrConflict; 
  
} // End - Execute_C118_EventNotificationControl()


/*********** CMD 119 *************************/

uint16_t Execute_C119_AcknowledgeEventNotification(C119_AcknowledgeEventNotification_Req* request,
                                     C119_AcknowledgeEventNotification_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{  
  uint8 ucRC = RCS_N00_Success;
  
  if(request->eventNo >= MAX_EVENTS_NO)
    return RCS_E02_InvalidSelection;
   
  if( g_ucIsEventAck )
  { // device received a Cmd119 request 
    
 
    if( (request->firstUnacknowledged.u32 == g_stEventQueue[0].m_ulTimestamp) &&
        (request->configurationChangedCounter == g_stEventQueue[0].m_ucCCCounter) &&
        (request->eventNo == g_stEventQueue[0].m_ucEventNmb) &&  
        (request->deviceStatus == g_stEventQueue[0].m_aData[0]) &&
        !memcmp(request->command48Data, g_stEventQueue[0].m_aData + 1, g_stEventsTableNvData[request->eventNo].m_ucMaskSize-1))
    { // CC counter, device status, Cmd48 data and timestamp match -> event ist acknowledged  
 
      g_stEventQueue[0].m_ucAckReceived = 1; // set the event acknowledged flag
      
      // reset event status bits (see common table 36) -> not necessary here 
      
      // look for other events into the queue
      if( (g_pEventQueueEnd - g_stEventQueue) > 1 )
      {  
        ucRC = RCS_W08_NotAllEventsCleared; 
      }
    }
    else
    { // Cmd119 request != published event -> event remains active and will be sent as response
      
      if( EVENT_QUEUE_NOT_EMPTY )
      {
        response->firstUnacknowledged.u32 = g_stEventQueue[0].m_ulTimestamp;
        response->configurationChangedCounter = g_stEventQueue[0].m_ucCCCounter;
        response->eventNo = g_stEventQueue[0].m_ucEventNmb;
        response->deviceStatus = g_stEventQueue[0].m_aData[0];
        memcpy(response->command48Data, g_stEventQueue[0].m_aData + 1, MaxLengthOf48Command); 
      
        // clear Cmd119 request flag
        g_ucIsEventAck = 0;
        
        return RCS_W08_NotAllEventsCleared; 
      }
    } 

    // clear Cmd119 request flag
    g_ucIsEventAck = 0;
    
    // compose the response
    *response = *request;
  }
  else
  { // send the triggered event
    response->firstUnacknowledged.u32 = g_stEventQueue[0].m_ulTimestamp;
    response->configurationChangedCounter = g_stEventQueue[0].m_ucCCCounter;
    response->eventNo = g_stEventQueue[0].m_ucEventNmb;
    response->deviceStatus = g_stEventQueue[0].m_aData[0];
    memcpy(response->command48Data, g_stEventQueue[0].m_aData + 1, MaxLengthOf48Command);     
  
  }  

  return ucRC;
// TODO[Dorin]:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_W08_NotAllEventsCleared;  
  
} // End - Execute_C119_AcknowledgeEventNotification()



//--------------- device specific commands, begin -----------------//



/*********** CMD 133 *************************/

uint16_t Execute_C133_ReadFirmwareVersion( C133_ReadFirmwareVersion_Req* request,
                                           C133_ReadFirmwareVersion_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control,
                                           uint8_t remainingBytes )
{
  // read the firmware version
  response->MajorVersion = MAJOR_VERSION; 
  response->MinorVersion = MINOR_VERSION;
  response->Build = BUILD;
  response->Letter = BUILD_LETTER; 
  
  return RCS_N00_Success;
} // End - Execute_C133_ReadFirmwareVersion



//--------------- device specific commands, end -------------------//



/*********** CMD 512 *************************/

uint16_t Execute_C512_ReadCountryCode(C512_ReadCountryCode_Req* request,
                                     C512_ReadCountryCode_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  return RCS_E64_CommandNotImplemented;
  //TODO:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError  
} // End - Execute_C512_ReadCountryCode()


/*********** CMD 513 *************************/

uint16_t Execute_C513_WriteCountryCode(C513_WriteCountryCode_Req* request,
                                     C513_WriteCountryCode_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  //SET_CONFIG_CHANGED
  return RCS_E64_CommandNotImplemented;
  //TODO:
//  return RCS_N00_Success;  	    
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;
} // End - Execute_C513_WriteCountryCode()


/*************************************************************************/
/************************ Wireless Commands ******************************/
/*************************************************************************/

/*********** CMD 768 *************************/

uint16_t Execute_C768_WriteJoinKey(C768_WriteJoinKey_Req* request,
                                  C768_WriteJoinKey_Resp* response,
                                  SHORT_ADDR srcAddr,
                                  uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    if(!(control & MAINTENANCE_PORT_MASK) || (g_ucNetJoinState >= NET_JOIN_ADV_NEIGHBOR))
    { // cmd not received through maintenance port or device is connected to network
      return RCS_E16_AccessRestricted;
    }
  }

  CK_WP_MODE

  *response = *(C768_WriteJoinKey_Resp *)request; 
  
  if(memcmp(g_stProv.aJoinSessionKey, (uint8*)request, 16))
  {
    memcpy(g_stProv.aJoinSessionKey, (uint8*)request, 16);
    
    // write aJoinSessionKey to FLASH
    NVM_WriteIndividualRecord( 18, 
                              (uint8*)&g_stProv + c_aProvNvmTable[17].m_unOffset, 
                              c_aProvNvmTable[17].m_ucSize);
    NET_SESSION_ATTRIBUTES* pSession = FindSessionByType(SessionTypeCode_Join, 0xF980);
    if(pSession)
    { // join session exists
      memcpy(pSession->m_aSessionKey, (uint8*)request, 16);
    }
    
  }
  return RCS_N00_Success;

//TODO:  
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;
//  return RCM_E65_KeyChangeFailed;
} // End - Execute_C768_WriteJoinKey()


/*********** CMD 769 *************************/

uint16_t Execute_C769_ReadJoinStatus(C769_ReadJoinStatus_Req* request,
                                    C769_ReadJoinStatus_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{
  g_stC769.noOfAvailableNeighbors = g_ucDllAllNeighborsSize;
  g_stC769.noOfAdvertisingPacketsReceived = g_ucNetAdNumber;
//  g_stC769.noOfJoinAttempts = ; // not need to be set here
  if(g_stC769.joinStatus < JoinProcessStatusMask_JoinRequested)
    g_stC769.joinRetryTimer.u32 = 0;
  else if(g_stC769.joinStatus < JoinProcessStatusMask_JoinFailed)
    g_stC769.joinRetryTimer.u32 = (g_ulJoinRspTimeout - g_ulJoinRspTimer) * (250UL * 32); // time since the last JReq was sent
  
  if(g_ucNetJoinState <= NET_JOIN_SEARCHING)
    g_stC769.networkSearchTimer.u32 = g_ulDllRecCntr * (10UL * 32); // time listening for first advertisement
  
  
  C769_ReadJoinStatus_Resp *p = &g_stC769; 
  *response = *p;
  response->wirelessMode &= 0x0F; // clear the high nibble, according to addendum
    
  return RCS_N00_Success;
//TODO:  
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;  
} // End - Execute_C769_ReadJoinStatus()


/*********** CMD 770 *************************/

// Cmd 770 must be implemented by wireless device as master and slave
uint16_t Execute_C770_RequestActiveAdvertise(C770_RequestActiveAdvertise_Req* request,
                                            C770_RequestActiveAdvertise_Resp* response,
                                            SHORT_ADDR srcAddr,
                                            uint8_t control, uint8_t remainingBytes)
{
  //CK_WP_MODE;   //  return RCS_E07_InWriteProtectMode;

  // TBD

  return RCS_E64_CommandNotImplemented;
  
//TODO:  
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;  
//  return RCM_W08_SetToNearestPossibleValue;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;    
//  return RCM_E65_Declined;  
} // End - Execute_C770_RequestActiveAdvertise()


/*********** CMD 771 *************************/
#define MIN_ACTIVE_SEARCH_TIME  (30*100UL)  // 30 sec in 10ms units
#define MAX_ACTIVE_SEARCH_TIME  (4800*100UL)  // 4800 sec in 10ms units

uint16_t Execute_C771_ForceJoin(C771_ForceJoin_Req* request,
                               C771_ForceJoin_Resp* response,
                               SHORT_ADDR srcAddr,
                               uint8_t control, uint8_t remainingBytes)
{
  if(request->m_ucJoinMode > JoinModeCode_AttemptToJoinImmediatelyOnPowerupOrReset)
    return RCS_E03_PassedParameterTooLarge;
  
  SET_CONFIG_CHANGED
//TODO:  
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_ForceJoinDeclined;  
  
  if(g_ucJoinMode != request->m_ucJoinMode)
  {
    g_ucJoinMode = request->m_ucJoinMode;
    if(g_stProv.ucJoinModeCode != request->m_ucJoinMode)
    {
      g_stProv.ucJoinModeCode = request->m_ucJoinMode;
      // write ucJoinModeCode to FLASH
      NVM_WriteIndividualRecord( 7, 
                               (uint8*)&g_stProv + c_aProvNvmTable[6].m_unOffset, 
                               c_aProvNvmTable[6].m_ucSize);
    }
  }

  uint32 ulASTime = request->m_tJoinShedTime.u32 / (32*10UL);   // 10ms timebase
  if(ulASTime < MIN_ACTIVE_SEARCH_TIME)
    ulASTime = MIN_ACTIVE_SEARCH_TIME;
  else if(ulASTime > MAX_ACTIVE_SEARCH_TIME)
    ulASTime = MAX_ACTIVE_SEARCH_TIME;
  if( ulASTime != g_stProv.ulActiveSearchShedTime )
  {
    g_stProv.ulActiveSearchShedTime = ulASTime;
    // write ulActiveSearchShedTime to FLASH
    NVM_WriteIndividualRecord( 54, 
                               (uint8*)&g_stProv + c_aProvNvmTable[53].m_unOffset, 
                               c_aProvNvmTable[53].m_ucSize);
  }
     
  response->m_ucJoinMode = g_ucJoinMode;
  response->m_tJoinShedTime.u32 = g_stProv.ulActiveSearchShedTime * (32*10UL);

  if(request->m_ucJoinMode <= JoinModeCode_JoinNow)
  {
    if((control & MAINTENANCE_PORT_MASK))
    { // perform a WDG reset
      g_ucDoDeviceInitFromCmd = APP_DEVRESET_C771_WIRED;
      g_ulDelayInitFromCmd = RESET_WIRED_TIMEOUT;
    }
    else
    { // should it transmit Disconnect packets if already joined???
      g_ucDoDeviceInitFromCmd = APP_DEVRESET_C771_WIRELESS;
#if ( WIHARTDEV_TYPE_ACCPOINT != WIHARTDEV_TYPE )             
//      g_ulDelayInitFromCmd = RESET_WIRELESS_TIMEOUT;
      g_ulDelayInitFromCmd = RESET_WIRED_TIMEOUT; // however it will send the packets from DLL queue first, then the Disconnect packets 
#else
      g_ulDelayInitFromCmd = RESET_WIRED_TIMEOUT; // AP doesn't need 20s to transmit the response via serial interface
#endif
      g_ucMarkDisconnectHandle = 1;
    }
    g_ulDelayInitFromCmd += RTC_COUNT;
  }
  
  if(response->m_tJoinShedTime.u32 != request->m_tJoinShedTime.u32)  
    return RCM_W08_SetToNearestPossibleValue;

  return RCS_N00_Success;
  
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCM_W08_SetToNearestPossibleValue;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;    
//  return RCM_E65_ForceJoinDeclined;  
} // End - Execute_C771_ForceJoin()


/*********** CMD 772 *************************/

uint16_t Execute_C772_ReadJoinModeConfiguration(C772_ReadJoinModeConfiguration_Req* request,
                                               C772_ReadJoinModeConfiguration_Resp* response,
                                               SHORT_ADDR srcAddr,
                                               uint8_t control, uint8_t remainingBytes)
{
  response->m_ucJoinMode = g_ucJoinMode; 
  response->m_tJoinShedTime.u32 = g_stProv.ulActiveSearchShedTime * (10 * 32UL); // 1/32 ms tics
  
  return RCS_N00_Success;
//TODO:  
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;  
} // End - Execute_C772_ReadJoinModeConfiguration()


/*********** CMD 773 *************************/

uint16_t Execute_C773_WriteNetworkId( C773_WriteNetworkId_Req* request,
                                     C773_WriteNetworkId_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)                     // the source is not network manager,
    if(!(control & MAINTENANCE_PORT_MASK))  // nor the maintenance port
      return RCS_E16_AccessRestricted;

//  // see HCF_SPEC-075 TDMA DLL 8.1.2, Table 2 // conflicting with TMLxxx tests, which use NetID=ABAB!!!
//  if((request->m_unNetworkId >= 0x9000 && request->m_unNetworkId <= 0xDFFF) || 
//     (request->m_unNetworkId >= 0xF000 /*&& request->m_unNetworkId <= 0xFFFF*/ ) )
//    return RCM_E65_InvalidNetworkID;
  
  CK_WP_MODE;   //  return RCS_E07_InWriteProtectMode;
  
  SET_CONFIG_CHANGED;
  if(g_stProv.unNetworkId != request->m_unNetworkId)
  {
    g_stProv.unNetworkId = request->m_unNetworkId;
   
    // write NetworkId to FLASH
    NVM_WriteIndividualRecord( 35, 
                               (uint8*)&g_stProv + c_aProvNvmTable[34].m_unOffset, 
                               c_aProvNvmTable[34].m_ucSize);
  }
  response->m_unNetworkId = g_stProv.unNetworkId;

//  if(g_ucDllStatus == DLL_IDLE)
  if(g_ucNetJoinState < NET_JOIN_LOOSELY_COUPLED)
  { // device not joined; make the change immediately
    
      // spec Cmd773: "If the device is not connected to the network, the NetworkID is changed immediately. 
      // The device !should! enter power save mode and wait for the ForceJoinMode command before beginning to actively search for the network"
      //
      // ...but for now we don't want to set here JoinModeCode=0 automatically
    
    if(g_ucJoinMode != JoinModeCode_DontAttemptToJoin)
    {
      // this should re-init the WiHART stack (not by a watchdog reset!) and force it to enter again the Active Search state
      // TODO: make sure that the C773 response was sent before re-init!!!
      g_ucNetState = NET_STATE_IDLE; 
    }
    g_unNetworkId = g_stProv.unNetworkId;
    return RCS_N00_Success;
  }
  else
  { // device joined or trying to join
      // The new NetworkID will start to be used since next Join.
      // For now, it was just written into the non-volatile memory.
    return RCM_W08_NetworkIDChangePending;
  }
  
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;    
//  return RCM_W08_NetworkIDChangePending;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;    
//  return RCM_E65_InvalidNetworkID;
} // End - Execute_C773_WriteNetworkId()


/*********** CMD 774 *************************/

uint16_t Execute_C774_ReadNetworkId( C774_ReadNetworkId_Req* request,
                                    C774_ReadNetworkId_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{
//  response->m_unNetworkId = g_stProv.unNetworkId;
  response->m_unNetworkId = g_unNetworkId; // the actual value used by the DLL
  return RCS_N00_Success;
  
// TODO[Doinel]:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;
} // End - Execute_C774_ReadNetworkId()


/*********** CMD 775 *************************/

uint16_t Execute_C775_WriteNetworkTag( C775_WriteNetworkTag_Req* request,
                                      C775_WriteNetworkTag_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  SET_CONFIG_CHANGED;
  
  memcpy(g_stProv.aNetworkTag, request->m_aNetworkTag, 32);  // TODO: non-volatile value ???
  memcpy(response->m_aNetworkTag, g_stProv.aNetworkTag, 32);

  // write aNetworkTag to FLASH
  NVM_WriteIndividualRecord( 20, 
                             (uint8*)&g_stProv + c_aProvNvmTable[19].m_unOffset, 
                             c_aProvNvmTable[19].m_ucSize);
  
  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy; // a DR could not be started
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;
} // End - Execute_C775_WriteNetworkTag()


/*********** CMD 776 *************************/

uint16_t Execute_C776_ReadNetworkTag( C776_ReadNetworkTag_Req* request,
                                     C776_ReadNetworkTag_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{

  memcpy(response->m_aNetworkTag, g_stProv.aNetworkTag, 32);

  return RCS_N00_Success;
// TODO[Doinel]:
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy; //???  
} // End - Execute_C776_ReadNetworkTag()


/*********** CMD 777 *************************/

uint16_t Execute_C777_ReadWirelessDeviceInformation( C777_ReadWirelessDeviceInformation_Req* request,
                                         C777_ReadWirelessDeviceInformation_Resp* response,
                                         SHORT_ADDR srcAddr,
                                         uint8_t control,
                                         uint8_t remainingBytes )
{
  response->m_ucPowerSource = g_stProv.ucPowerSource;
  response->m_fPeakPacketsPerSecond = g_stProv.fPeakPacketsPerSecond;
  response->m_tDurationAtPeakPacketLoadBeforePowerDrained.u32 = g_stProv.tDurationAtPeakPacketLoad;
  response->m_tTimeToRecoverFromPowerDrain.u32 = g_stProv.tTimeToRecoverFromPowerDrain;
  response->m_cRSLThresholdGoodConnection = g_stProv.cRSLThresholdGoodConnection;
  response->m_tMinRequiredKeepAliveTime.u32 = g_stProv.tMinRequiredKeepAliveTime;
  response->m_unMaxNoOfNeighbors = g_stProv.unMaxNoOfNeighbors;
  response->m_unMaxNoOfPacketBuffers = g_stProv.unMaxNoOfPacketBuffers;

  return RCS_N00_Success;
} // End - Execute_C777_ReadWirelessDeviceInformation()


/*********** CMD 778 *************************/

uint16_t Execute_C778_ReadBatteryLife(C778_ReadBatteryLife_Req* request,
                                     C778_ReadBatteryLife_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  
  // battery life remaining in days; if the device has no battery -> return 0xFFFF
  response->m_unBatteryLifeRemaining = 0xFFFF; // no battery device
  
  return RCS_N00_Success;
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;
  
} // End - Execute_C778_ReadBatteryLife()


/*********** CMD 779 *************************/

uint16_t Execute_C779_ReportDeviceHealth( C779_ReportDeviceHealth_Req* request,
                                         C779_ReportDeviceHealth_Resp* response,
                                         SHORT_ADDR srcAddr,
                                         uint8_t control,
                                         uint8_t remainingBytes )
{
  // not need to disable DLL interrupts, because the members updated from interrupt are one-byte only
  memcpy((void*)response, &g_stC779, sizeof(C779_ReportDeviceHealth_Resp));

  return RCS_N00_Success;
} // End - Execute_C779_ReportDeviceHealth()


/*********** CMD 780 *************************/

uint16_t Execute_C780_ReportNeighborHealthList( C780_ReportNeighborHealthList_Req* request,
                                               C780_ReportNeighborHealthList_Resp* response,
                                               SHORT_ADDR srcAddr,
                                               uint8_t control,
                                               uint8_t remainingBytes )
{
  // DLL interrupts must be disabled while working with the neighbor table
  MONITOR_ENTER();
  
  response->m_ucTotalNoOfNeighbors = g_ucDllLkdNeighborsSize;

  if(response->m_ucTotalNoOfNeighbors == 0)
  { // neighbor list is empty (request from maintenance port)
    MONITOR_EXIT();
    return RCS_E02_InvalidSelection;
  }

  uint8 ucStatus = RCS_N00_Success;

  if( request->m_ucNeighborTableIndex >= response->m_ucTotalNoOfNeighbors )
  { // if neighbor index is greater than the nr. of linked neighbors, the last linked neighbor is returned
    response->m_ucNeighborTableIndex = response->m_ucTotalNoOfNeighbors - 1;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    response->m_ucNeighborTableIndex = request->m_ucNeighborTableIndex;
  }

  // if the nr. of requested entries is greater than the nr. of available entries, the maximum nr. of entries is returned
  if( request->m_ucNoOfNeighborEntriesToRead > (response->m_ucTotalNoOfNeighbors - response->m_ucNeighborTableIndex) )
  {
    response->m_ucNoOfNeighborEntriesRead = response->m_ucTotalNoOfNeighbors - response->m_ucNeighborTableIndex;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else if(request->m_ucNoOfNeighborEntriesToRead == 0)
  {
    response->m_ucNoOfNeighborEntriesRead = 1;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    response->m_ucNoOfNeighborEntriesRead  = request->m_ucNoOfNeighborEntriesToRead;
  }

  if(3 + 10 * response->m_ucNoOfNeighborEntriesRead > remainingBytes)
  { // if more entries than buffer capacity -> limit the number of entries to the buffer capacity
    response->m_ucNoOfNeighborEntriesRead = (remainingBytes - 3)/10;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }

  uint8 ucIndex = response->m_ucNeighborTableIndex;

  for( uint8 ucIdx = 0; ucIdx < response->m_ucNoOfNeighborEntriesRead; ucIndex++, ucIdx++ )
  {
    response->m_aNeighbors[ucIdx].nicknameOfNeighbor = g_aDllNeighbors[ucIndex].m_unShortAddr;
    response->m_aNeighbors[ucIdx].neighborFlags = g_aDllNeighbors[ucIndex].m_bTimeSource & NeighbourFlagsMask_TimeSource;
    response->m_aNeighbors[ucIdx].meanRSLSinceLastReport = g_aDllNeighbors[ucIndex].m_cAvgRSL;
    response->m_aNeighbors[ucIdx].noOfPacketsTransmittedToThisNeighbor = g_aDllNeighbors[ucIndex].m_unPacketsTransmitted;
    response->m_aNeighbors[ucIdx].noOfFailedTransmits = g_aDllNeighbors[ucIndex].m_unPacketsMissedACK;
    response->m_aNeighbors[ucIdx].noOfPacketsReceivedFromThisNeighbor =
      g_aDllNeighbors[ucIndex].m_unPacketsReceived; // + g_aDllNeighbors[ucIndex].m_unBroadcastsReceived; // see Issue#64
  }
  MONITOR_EXIT();

  return ucStatus;
//TODO:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCM_W08_SetToNearestPossibleValue;  
} // End - Execute_C780_ReportNeighborHealthList()


/*********** CMD 781 *************************/

uint16_t Execute_C781_ReadDeviceNicknameAddress( C781_ReadDeviceNicknameAddress_Req* request,
                                                C781_ReadDeviceNicknameAddress_Resp* response,
                                                SHORT_ADDR srcAddr,
                                                uint8_t control, uint8_t remainingBytes)
{
  response->Nickname = g_unNickname;
  return RCS_N00_Success;
} // End - Execute_C781_ReadDeviceNicknameAddress()


/*********** CMD 782 *************************/

uint16_t Execute_C782_ReadSessionEntries(C782_ReadSessionEntries_Req* request,
                                        C782_ReadSessionEntries_Resp* response,
                                        SHORT_ADDR srcAddr,
                                        uint8_t control, uint8_t remainingBytes)
{
  //  if(g_ucSessionsNo == 0 || request->m_ucNoOfEntriesToRead == 0) // see TML204B: RC8 when m_ucNoOfEntriesToRead not good
  if(g_ucSessionsNo == 0)
  { // session list is empty
    return RCS_E02_InvalidSelection;
  }

  uint8_t ucSessionIdx;
  uint8_t ucRespCode = RCS_N00_Success;

  // if session index is greater than the nr. of sessions, the last session is returned
  if(request->m_ucSessionIndex >= g_ucSessionsNo)
  {
    ucSessionIdx = g_ucSessionsNo - 1;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    ucSessionIdx = request->m_ucSessionIndex;
  }

  if(request->m_ucNoOfEntriesToRead > (g_ucSessionsNo - ucSessionIdx))
  {
    // if the nr. of requested entries is greater than the nr. of available entries,
    // the maximum nr. of entries is returned
    response->m_ucNoOfEntriesRead = g_ucSessionsNo - ucSessionIdx;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else if(request->m_ucNoOfEntriesToRead == 0)
  {
    // should be coerced to a valid value and RC8 returned
    response->m_ucNoOfEntriesRead = 1;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    response->m_ucNoOfEntriesRead = request->m_ucNoOfEntriesToRead;
  }

//  if( remainingBytes < C782_RespSize ) // already checked
//  {
//    return RCS_E60_PayloadTooLong;
//  }
//  else 
  if((3 + 16 * response->m_ucNoOfEntriesRead) > remainingBytes)
  { // if more entries than buffer capacity -> limit the number of entries to the buffer capacity
    response->m_ucNoOfEntriesRead = (remainingBytes - 3)/16;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }

  response->m_ucSessionIndex = ucSessionIdx;
  response->m_unNoOfActiveSessions = g_ucSessionsNo;

  uint8_t i;
  // find the required start index, skipping any unused session entries
  for( ucSessionIdx = 0, i = 0; ucSessionIdx < MAX_SESSIONS_NO; ucSessionIdx++ )
  {
    if( (g_aSessionsTable[ucSessionIdx].m_ucSessionFlags & UNUSED_ENTRY) == 0 )
    { // this entry is in use
      i++;  // increment number of sessions read

      if( i > response->m_ucSessionIndex )
      { // the desired index is found when response->m_ucSessionIndex + 1 sessions are read
        break;
      }
    }
  }

  // read the desired number of sessions (starting at the previously determined index),
  //  skipping any unused session entries
  for( i = 0; i < response->m_ucNoOfEntriesRead; ucSessionIdx++ )
  {
    if((g_aSessionsTable[ucSessionIdx].m_ucSessionFlags & UNUSED_ENTRY) == 0)
    { // this entry is in use
      response->m_aSessions[i].sessionType = g_aSessionsTable[ucSessionIdx].m_ucSessionType;
      response->m_aSessions[i].peerDeviceNickname = g_aSessionsTable[ucSessionIdx].m_unPeerNickname;
      memcpy( response->m_aSessions[i].peerDeviceUniqueId, g_aSessionsTable[ucSessionIdx].m_aPeerUniqueID, 5 );
      response->m_aSessions[i].peerDeviceNonceCounterVal = g_aSessionsTable[ucSessionIdx].m_ulPeerNonceCounter;
      response->m_aSessions[i].theDeviceNonceCounterVal = g_aSessionsTable[ucSessionIdx].m_ulMyNonceCounter;
      i++;
    }
  }

  return ucRespCode;
//TODO
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCM_W08_SetToNearestPossibleValue;  
} // End - Execute_C782_ReadSessionEntries()


/*********** CMD 783 *************************/

uint16_t Execute_C783_ReadSuperframeList( C783_ReadSuperframeList_Req* request,
                                         C783_ReadSuperframeList_Resp* response,
                                         SHORT_ADDR srcAddr,
                                         uint8_t control, uint8_t remainingBytes)
{
  // DLL interrupts should be disabled while working with the superframe table!!!!!!
  MONITOR_ENTER();

  if(g_ucDllAllSuperframesSize == 0)  // if the Superframe table is empty
  {
    MONITOR_EXIT();
    return RCS_E02_InvalidSelection;
  }

  uint8 ucStatus = RCS_N00_Success;

  // if Superframe index is greater than the nr. of superframes, the last Superframe is returned
  if(request->m_ucSuperframeIndex >= g_ucDllAllSuperframesSize)
  {
     response->m_ucSuperframeIndex = g_ucDllAllSuperframesSize - 1;
     ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
     response->m_ucSuperframeIndex = request->m_ucSuperframeIndex;
  }

  // if the nr. of requested entries is greater than the nr. of available entries, the maximum nr. of entries is returned
  if(request->m_ucNoOfEntriesToRead > g_ucDllAllSuperframesSize - response->m_ucSuperframeIndex)
  {
     response->m_ucNoOfEntriesRead = g_ucDllAllSuperframesSize - response->m_ucSuperframeIndex;
     ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else if(request->m_ucNoOfEntriesToRead == 0)
  {
    // should be coerced to a valid value and RC8 returned
    response->m_ucNoOfEntriesRead = 1;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    response->m_ucNoOfEntriesRead = request->m_ucNoOfEntriesToRead;
  }

  // if the nr. of requested entries is grater than the maximum number of entries that the command can read
  if(response->m_ucNoOfEntriesRead > C783_MAX_SUPERFRAMES_LIST)
  {
    response->m_ucNoOfEntriesRead = C783_MAX_SUPERFRAMES_LIST;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }

//  if( remainingBytes < C783_RespSize ) // already checked
//  {
//    MONITOR_EXIT();
//    return RCS_E60_PayloadTooLong;
//  }
//  else 
  if((3 + 4 * response->m_ucNoOfEntriesRead) > remainingBytes)
  { // if more entries than buffer capacity -> limit the number of entries to the buffer capacity
    response->m_ucNoOfEntriesRead = (remainingBytes - 3)/4;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }

  uint8 ucIndex = response->m_ucSuperframeIndex;
  for( uint8 ucIdx = 0; ucIdx < response->m_ucNoOfEntriesRead; ucIdx++, ucIndex++ )
  {
    response->m_aSuperframes[ucIdx].superframeId          = g_aDllSuperframes[ucIndex].m_ucSuperframeID;
    response->m_aSuperframes[ucIdx].superframeModeFlags   = g_aDllSuperframes[ucIndex].m_eActive ? 0x01 : 0x00;  // Table 47

    // if the superframe is designated for handheld
    if(g_aDllSuperframes[ucIndex].m_bHandheldSF)
      response->m_aSuperframes[ucIdx].superframeModeFlags |= 0x80;  // see CommonTable 47

    response->m_aSuperframes[ucIdx].noOfSlotsInSuperframe = g_aDllSuperframes[ucIndex].m_unNumSlots;
  }

  response->m_ucNoOfActiveSuperframes = g_ucDllAllSuperframesSize;

  MONITOR_EXIT();

  return ucStatus;
// TODO[Doinel]:
///  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;  
//  return RCS_E05_TooFewDataBytesReceived;  
///  return RCM_W08_SetToNearestPossibleValue;    
} // End - Execute_C783_ReadSuperframeList()


/*********** CMD 784 *************************/

uint16_t Execute_C784_ReadLinkList( C784_ReadLinkList_Req* request,
                                   C784_ReadLinkList_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes)
{
  if(g_ucDllAllLinksSize == 0)  // if the Link table is empty
  {
    return RCS_E02_InvalidSelection;
  }

  uint8 ucStatus = RCS_N00_Success;

  // DLL interrupts should be disabled while working with the link table!!!!!!
  MONITOR_ENTER();

  // if link index is greater than the nr. of links, the last link is returned
  if(request->m_unLinkIndex >= g_ucDllAllLinksSize)
  {
     response->m_unLinkIndex = g_ucDllAllLinksSize - 1;
     ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
     response->m_unLinkIndex = request->m_unLinkIndex;
  }

  // if the nr. of requested entries is greater than the nr. of available entries, the maximum nr. of entries is returned
  if(request->m_ucNoOfLinksToRead > (g_ucDllAllLinksSize - response->m_unLinkIndex))
  {
     response->m_ucNoOfLinksRead = g_ucDllAllLinksSize - response->m_unLinkIndex;
     ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else if(request->m_ucNoOfLinksToRead == 0)
  {
    // should be coerced to a valid value and RC8 returned
    response->m_ucNoOfLinksRead = 1;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
     response->m_ucNoOfLinksRead   = request->m_ucNoOfLinksToRead;
  }

  // if the nr. of requested entries is grater than the maximum number of entries that the command can read
  if(response->m_ucNoOfLinksRead > C784_MAX_LINKS_LIST)
  {
    response->m_ucNoOfLinksRead = C784_MAX_LINKS_LIST;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }

//  if( remainingBytes < C784_RespSize ) // already checked
//  {
//    MONITOR_EXIT();
//    return RCS_E60_PayloadTooLong;
//  }
//  else 
  if((5 + 8 * response->m_ucNoOfLinksRead) > remainingBytes)
  { // if more entries than buffer capacity -> limit the number of entries to the buffer capacity
    response->m_ucNoOfLinksRead = (remainingBytes - 5)/8;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }

  uint8 ucIndex = response->m_unLinkIndex;
  for( uint8 ucIdx = 0; ucIdx < response->m_ucNoOfLinksRead; ucIdx++, ucIndex++ )
  {
    response->m_aLinks[ucIdx].superframeId                  =  g_aDllLinks[ucIndex].m_ucSuperframeID;
    response->m_aLinks[ucIdx].channelOffsetForThisLink      =  g_aDllLinks[ucIndex].m_ucChannelOffset;
    response->m_aLinks[ucIdx].slotNoForThisLink             =  g_aDllLinks[ucIndex].m_unSlot;
    response->m_aLinks[ucIdx].nicknameOfNeighborForThisLink =  g_aDllLinks[ucIndex].m_unNeighborShortAddr;
    response->m_aLinks[ucIdx].linkType                      =  g_aDllLinks[ucIndex].m_eLinkType;
    response->m_aLinks[ucIdx].linkOptions                   =  g_aDllLinks[ucIndex].m_bTx ? 0x01 : 0x00;  // Table 46
    if(g_aDllLinks[ucIndex].m_bRx)
      response->m_aLinks[ucIdx].linkOptions |=  0x02; // Table 46
    if(g_aDllLinks[ucIndex].m_bShared)
      response->m_aLinks[ucIdx].linkOptions |=  0x04; // Table 46
  }

  response->m_unNoOfActiveLinks = g_ucDllAllLinksSize;

  MONITOR_EXIT();

  return ucStatus;
// TODO[Doinel]:
///  return RCS_N00_Success;  
///  return RCS_E02_InvalidSelection;   
//  return RCS_E05_TooFewDataBytesReceived;  
///  return RCM_W08_SetToNearestPossibleValue;      
} // End - Execute_C784_ReadLinkList()


/*********** CMD 785 *************************/

uint16_t Execute_C785_ReadGraphList( C785_ReadGraphList_Req* request,
                                    C785_ReadGraphList_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{
  
  if(g_ucDllHashGraphsSize == 0)
    return RCS_E02_InvalidSelection;
  
  uint8 ucStatus = RCS_N00_Success; 
  uint8 ucGraphListIndex;
  uint8 ucIndex1, ucIndex2;
  
  MONITOR_ENTER();
  if(request->m_ucGraphListIndex >= g_ucDllHashGraphsSize)
  {
    ucGraphListIndex = g_ucDllHashGraphsSize - 1;
    ucStatus = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    ucGraphListIndex = request->m_ucGraphListIndex;
  }
  
  for( ucIndex1 = ucIndex2 = 0; ucIndex1 < g_ucDllCrtConnectionsSize; ucIndex1++ )
  {
    if( g_aDllHashGraphs[ucGraphListIndex].m_unGraphID == g_aDllConnections[ucIndex1].m_unGraphID )
    {  
      response->m_aNicknameOfNeighbor[ucIndex2++] =  g_aDllConnections[ucIndex1].m_unNeighborShortAddr;
    }  
  }
  
  response->m_ucNoOfNeighbors   = g_aDllHashGraphs[ucGraphListIndex].m_ucLen;
  response->m_ucGraphListIndex  = ucGraphListIndex;
  response->m_ucTotalNoOfGraphs = g_ucDllHashGraphsSize;
  response->m_unGraphId         = g_aDllHashGraphs[ucGraphListIndex].m_unGraphID;

  MONITOR_EXIT();
  
  return ucStatus;
// TODO[Doinel]:
///  return RCS_N00_Success;  
//  return RCS_E02_InvalidSelection;     
//  return RCS_E05_TooFewDataBytesReceived;  
///  return RCM_W08_SetToNearestPossibleValue;      
} // End - Execute_C785_ReadGraphList()


/*********** CMD 786 *************************/

uint16_t Execute_C786_ReadNeighborPropertyFlag( C786_ReadNeighborPropertyFlag_Req* request,
                                               C786_ReadNeighborPropertyFlag_Resp* response,
                                               SHORT_ADDR srcAddr,
                                               uint8_t control, uint8_t remainingBytes)
{
  // doinel.alban: is it ok even if that neighbor has no link ?

  uint8 rc = RCS_N00_Success;
  
  MONITOR_ENTER();

  uint8 ucIdx = DLL_FindShortNeighbor(request->Nickname);
  if( ucIdx == DLL_INVALID_NEIGH_IDX )  // if neighbor not found
    rc = RCM_E65_UnknownNickname;
  else
  {
    response->Nickname      = request->Nickname;
    response->NeighbourFlag = (g_aDllNeighbors[ucIdx].m_bTimeSource & NeighbourFlagsMask_TimeSource) ? 0x01 : 0x00;  // Table 59
  }
  
  MONITOR_EXIT();
  return rc;
  
// TODO[Doinel]:
///  return RCS_N00_Success;  
//  return RCS_E05_TooFewDataBytesReceived;  
///  return RCM_E65_UnknownNickname;    
} // End - Execute_C786_ReadNeighborPropertyFlag()


/*********** CMD 787 *************************/

uint16_t Execute_C787_ReportNeighborSignalLevels( C787_ReportNeighborSignalLevels_Req* request,
                                                 C787_ReportNeighborSignalLevels_Resp* response,
                                                 SHORT_ADDR srcAddr,
                                                 uint8_t control, uint8_t remainingBytes)
{
  // The response has to be published at "Process Data" level priority DLL_PACKET_PRIORITY_DATA
  // when included in health report

//  if( remainingBytes < C787_RespSize ) // already checked
//  {
//    return RCS_E60_PayloadTooLong;
//  }

  uint8 ucStatus = RCS_N00_Success;

  MONITOR_ENTER();
  do
  {
    uint8 ucAll = g_ucDllAllNeighborsSize;
    uint8 ucLkd = g_ucDllLkdNeighborsSize;
    
    // number of non-linked neighbors
    response->m_ucTotalNoOfNeighbors = (ucAll>ucLkd) ? (ucAll - ucLkd) : 0;
    if(!response->m_ucTotalNoOfNeighbors)
    {
      response->m_ucNoOfNeighborEntriesRead = 0;
      response->m_ucNeighborTableIndex = ucLkd; // first empty entry
      
      if(request->m_ucNoOfNeighborEntriesToRead || (request->m_ucNeighborTableIndex != ucLkd))
      {
        ucStatus = RCM_W08_SetToNearestPossibleValue;
      }
      break;
    }
    
    // if neighbor index is greater than the nr. of neighbors, the last neighbor is returned
    if(request->m_ucNeighborTableIndex >= ucAll)
    {
      response->m_ucNeighborTableIndex = ucAll - 1;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
    else
    { // if the neighbor index is smaller than the nr. of linked neighbors, the first un-linked neighbor is returned
      if(request->m_ucNeighborTableIndex < ucLkd)
      {
        response->m_ucNeighborTableIndex = ucLkd;
        ucStatus = RCM_W08_SetToNearestPossibleValue;
      }
      else
      {
        response->m_ucNeighborTableIndex = request->m_ucNeighborTableIndex;
      }
    }
    
    // if the nr. of requested entries is greater than the nr. of available entries, the available nr. of entries is returned
    if(request->m_ucNoOfNeighborEntriesToRead > (ucAll - response->m_ucNeighborTableIndex))
    {
      response->m_ucNoOfNeighborEntriesRead = ucAll - response->m_ucNeighborTableIndex;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
    else if(0 == request->m_ucNoOfNeighborEntriesToRead)
    { // should be coerced to a valid value and RC8 returned
      response->m_ucNoOfNeighborEntriesRead = 1;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
    else
    {
      response->m_ucNoOfNeighborEntriesRead  = request->m_ucNoOfNeighborEntriesToRead;
    }
    
    if( (3 + 3 * response->m_ucNoOfNeighborEntriesRead) > remainingBytes )
    { // if more entries than buffer capacity -> limit the number of entries to the buffer capacity
      response->m_ucNoOfNeighborEntriesRead = (remainingBytes - 3)/3;
      ucStatus = RCM_W08_SetToNearestPossibleValue;
    }
    
    uint8 ucIndex = response->m_ucNeighborTableIndex;
    for( uint8 ucIdx = 0; ucIdx < response->m_ucNoOfNeighborEntriesRead; ucIndex++, ucIdx++ )
    {
      response->m_aNeighbors[ucIdx].RSLindB  = g_aDllNeighbors[ucIndex].m_cAvgRSL;
      response->m_aNeighbors[ucIdx].nickname = g_aDllNeighbors[ucIndex].m_unShortAddr;
    }
    
  } while(FALSE);
  
  MONITOR_EXIT();
  
  return ucStatus;
  
// TODO[Doinel]:
///  return RCS_N00_Success;  
//  return RCS_E05_TooFewDataBytesReceived;  
///  return RCM_E65_UnknownNickname;   
} // End - Execute_C787_ReportNeighborSignalLevels()


/*********** CMD 788 *************************/

// Not used - this command shall only be published!
//uint16_t Execute_C788_AlarmPathDown( C788_AlarmPathDown_Req* request,
//                                    C788_AlarmPathDown_Resp* response,
//                                    SHORT_ADDR srcAddr,
//                                    uint8_t control, uint8_t remainingBytes)
//{
//
//  //TODO:  The response has to be transmitted at "Alarm" level priority DLL_PACKET_PRIORITY_ALARM
//
//  if(!(control & PUBLISHED_RESP_MASK))
//    return RCS_E16_AccessRestricted;
//
//  //TODO:
//  //  response->Nickname = ;
//
//  return RCS_N00_Success;
//
//  // return RCS_N00_Success;
//  // return RCS_E16_AccessRestricted;
//} // End - Execute_C788_AlarmPathDown()


/*********** CMD 789 *************************/

// Not used - this command shall only be published!
//uint16_t Execute_C789_AlarmSourceRouteFailed(C789_AlarmSourceRouteFailed_Req* request,
//                                            C789_AlarmSourceRouteFailed_Resp* response,
//                                            SHORT_ADDR srcAddr,
//                                            uint8_t control, uint8_t remainingBytes)
//{
//  if(C789_RespSize > remainingBytes)
//    return RCS_E60_PayloadTooLong; //?such resp code not specified
//
//  // TBD
//  // TODO:  The response has to be transmitted at "Alarm" level priority DLL_PACKET_PRIORITY_DATA
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
//} // End - Execute_C789_AlarmSourceRouteFailed()


/*********** CMD 790 *************************/

// Not used - this command shall only be published!
//uint16_t Execute_C790_AlarmGraphRouteFailed(C790_AlarmGraphRouteFailed_Req* request,
//                                           C790_AlarmGraphRouteFailed_Resp* response,
//                                           SHORT_ADDR srcAddr,
//                                           uint8_t control, uint8_t remainingBytes)
//{
//  if(C790_RespSize > remainingBytes)
//    return RCS_E60_PayloadTooLong; //?such resp code not specified
//
//  // TBD
//  // TODO:  The response has to be transmitted at "Alarm" level priority DLL_PACKET_PRIORITY_DATA
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
//} // End - Execute_C790_AlarmGraphRouteFailed()


/*********** CMD 791 *************************/

// Not used - this command shall only be published!
//uint16_t Execute_C791_AlarmTransportLayerFailed(C791_AlarmTransportLayerFailed_Req* request,
//                                               C791_AlarmTransportLayerFailed_Resp* response,
//                                               SHORT_ADDR srcAddr,
//                                               uint8_t control, uint8_t remainingBytes)
//{
//  if(C791_RespSize > remainingBytes)
//    return RCS_E60_PayloadTooLong; //?such resp code not specified
//
//  // TBD
//  // TODO:  The response has to be transmitted at "Alarm" level priority DLL_PACKET_PRIORITY_DATA
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
//} // End - Execute_C791_AlarmTransportLayerFailed()


// Execute_C793_WriteUTCTime
// Execute_C794_ReadUTCTime


/*********** CMD 793 *************************/

uint16_t Execute_C793_WriteUTCTime( C793_WriteUTCTime_Req* request, 
                                   C793_WriteUTCTime_Resp* response,                                       
                                   SHORT_ADDR srcAddr, 
                                   uint8_t control, uint8_t remainingBytes)
{

  // UTC and the current ASN -> g_stUTCatASN0
  
  g_stUTCatASN0.m_ucUTCReceived = 0x01;
  
  g_stUTCatASN0.m_ucDay = request->m_dDate.day; 
  g_stUTCatASN0.m_ucMonth = request->m_dDate.month;
  g_stUTCatASN0.m_ucYear = request->m_dDate.year;
  g_stUTCatASN0.m_ulTimeOfDay = request->m_tTime.u32;  
  
  g_stUTCatASN0.m_ullASN = g_sDllASN.m_ullASN;
  return RCS_N00_Success;
  
// TODO[Dorin]:
//  return RCS_N00_Success;    
//  return RCS_E05_TooFewDataBytesReceived;  
//  return RCS_E16_AccessRestricted;       
} // End - Execute_C793_WriteUTCTime()


/*********** CMD 794 *************************/

uint16_t Execute_C794_ReadUTCTime( C794_ReadUTCTime_Req* request, 
                                  C794_ReadUTCTime_Resp* response,                                       
                                  SHORT_ADDR srcAddr, 
                                  uint8_t control, uint8_t remainingBytes)
{
  
  // return the UTC at ASN0
  response->m_dDate.day   = g_stUTCatASN0.m_ucDay;
  response->m_dDate.month = g_stUTCatASN0.m_ucMonth;
  response->m_dDate.year  = g_stUTCatASN0.m_ucYear;
  response->m_tTime.u32   = g_stUTCatASN0.m_ulTimeOfDay;
  
  return RCS_N00_Success;  

// TODO[Dorin]:
///  return RCS_N00_Success;     
} // End - Execute_C794_ReadUTCTime()


/*********** CMD 795 *************************/

uint16_t Execute_C795_WriteTimerInterval( C795_WriteTimerInterval_Req* request,
                                         C795_WriteTimerInterval_Resp* response,
                                         SHORT_ADDR srcAddr,
                                         uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  if(srcAddr != NM_NICKNAME)    // not network manager,
     return RCS_E16_AccessRestricted;

  response->m_ucTimerType = request->m_ucTimerType;
  response->m_ulTimerInterval = request->m_ulTimerInterval;
  // m_ulTimerInterval in [ms] units -see command description
  return DLL_LM_Request(DLL_SERV_WRITE_TIMEOUT_PERIOD, response);

// TODO:
  // return RCS_N00_Success;
  // return RCS_E03_PassedParameterTooLarge;
  // return RCS_E04_PassedParameterTooSmall;
  // return RCS_E05_TooFewDataBytesReceived;
  // return RCS_E06_DeviceSpecificCommandError;
  // return RCS_E07_InWriteProtectMode;
     // return RCM_W08_SetToNearestPossibleValue;
     // return RCS_E16_AccessRestricted;
  // return RCS_E32_Busy;
  // return RCS_E33_DelayedResponseInitiated;
  // return RCS_E34_DelayedResponseRunning;
  // return RCS_E35_DelayedResponsDead;
  // return RCS_E36_DelayedResponseConflict;
     // return RCM_E65_InvalidTimerType;
} // End - Execute_C795_WriteTimerInterval()


/*********** CMD 796 *************************/

uint16_t Execute_C796_ReadTimerInterval( C796_ReadTimerInterval_Req* request,
                                        C796_ReadTimerInterval_Resp* response,
                                        SHORT_ADDR srcAddr,
                                        uint8_t control, uint8_t remainingBytes)
{
  response->m_ucTimerType = request->m_ucTimerType;
  // m_ulTimerInterval in [ms] units -see command description
  return DLL_LM_Request(DLL_SERV_READ_TIMEOUT_PERIOD, response);

//TODO:
  // return RCS_N00_Success;
  // return RCS_E05_TooFewDataBytesReceived;
  // return RCS_E06_DeviceSpecificCommandError;
  // return RCS_E32_Busy;
  // return RCM_E65_InvalidTimerType;
} // End - Execute_C796_ReadTimerInterval()


/*********** CMD 797 *************************/

uint16_t Execute_C797_WriteRadioPower(C797_WriteRadioPower_Req* request,
                                     C797_WriteRadioPower_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  if(srcAddr != NM_NICKNAME)   // network manager
  {
    if(!(control & MAINTENANCE_PORT_MASK) || g_ucNetState != NET_STATE_IDLE)
      return RCS_E16_AccessRestricted;
  }

  response->outputPower = request->outputPower;
  return PHY_MNG_Request(PHY_MNG_WRITE_TX_PWR_LVL, response);


  // PHY_MNG_Request() must return one of the following values:

//TODO:  
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E04_PassedParameterTooSmall;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;
} // End - Execute_C797_WriteRadioPower()


/*********** CMD 798 *************************/

uint16_t Execute_C798_ReadRadioPower(C798_ReadRadioPower_Req* request,
                                    C798_ReadRadioPower_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{


  return PHY_MNG_Request (PHY_MNG_READ_TX_PWR_LVL, response);
  // PHY_MNG_Request() must return one of the following values:
  
  //TODO:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;
} // End - Execute_C798_ReadRadioPower()


/*********** CMD 799 *************************/

/*  C799 has to be implemented by the Network Manager as slave and field device as master
uint16_t Execute_C799_RequestService(C799_RequestService_Req* request,
                                    C799_RequestService_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{
  // TBD
//return RCS_N00_Success;
//return RCS_E04_PassedParameterTooSmall;
//return RCS_E05_TooFewDataBytesReceived;
//return RCS_E06_DeviceSpecificCommandError;
//return RCM_W08_SetToNearestPossibleValue;    
//return RCM_W14_CommBandwidthLessThanRequested;
//return RCS_E16_AccessRestricted;
//return RCS_E32_Busy;
//return RCS_E33_DelayedResponseInitiated;
//return RCS_E34_DelayedResponseRunning;
//return RCS_E35_DelayedResponseDead;
//return RCS_E36_DelayedResponseConflict;
//return RCM_E65_ServiceRequestDenied;
//return RCM_E66_UnknownServiceFlag;
//return RCM_E67_UnknownApplicationDomain;
//return RCM_E68_UnknownNickname;
//return RCM_E69_InvalidServiceId;
} // End - Execute_C799_RequestService()
*/


/*********** CMD 800 *************************/

uint16_t Execute_C800_ReadServiceList(C800_ReadServiceList_Req* request,
                                     C800_ReadServiceList_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  if(g_ucServicesNo == 0)
  { // service list is empty
    return RCS_E02_InvalidSelection;
  }

  uint8_t ucServiceIdx;
  uint8_t ucRespCode = RCS_N00_Success;

  // if service index is greater than the nr. of services, the last service is returned
  if(request->m_ucServiceIndex >= g_ucServicesNo)
  {
    ucServiceIdx = g_ucServicesNo - 1;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    ucServiceIdx = request->m_ucServiceIndex;
  }

  if(request->m_ucNoOfEntriesToRead > g_ucServicesNo - ucServiceIdx)
  {
    // if the nr. of requested entries is greater than the nr. of available entries,
    // the maximum nr. of entries is returned
    response->m_ucNoOfEntriesRead = g_ucServicesNo - ucServiceIdx;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else if(0 == request->m_ucNoOfEntriesToRead)
  { // should be coerced to a valid value and RC8 returned
    response->m_ucNoOfEntriesRead = 1;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    response->m_ucNoOfEntriesRead = request->m_ucNoOfEntriesToRead;
  }

//  if( remainingBytes < C800_RespSize ) // already checked
//  {
//    return RCS_E60_PayloadTooLong;
//  }
//  else 
  if(3 + 10 * response->m_ucNoOfEntriesRead > remainingBytes)
  { // if more entries than buffer capacity -> limit the number of entries to the buffer capacity
    response->m_ucNoOfEntriesRead = (remainingBytes - 3)/10;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }

  response->m_ucServiceIndex = ucServiceIdx;
  response->m_ucNoOfActiveServices = g_ucServicesNo;

  for(uint8_t i = 0, j = response->m_ucServiceIndex; i < response->m_ucNoOfEntriesRead; i++, j++)
  {
    response->m_aServices[i].serviceId = g_aServicesTable[j].m_ucServiceID;
    response->m_aServices[i].serviceRequestFlags = g_aServicesTable[j].m_ucServiceFlags;
    response->m_aServices[i].serviceApplicationDomain = g_aServicesTable[j].m_ucApplicationDomain;

    // find peer nickname in the associated route
    ucServiceIdx = FindRouteIndex(g_aServicesTable[j].m_ucRouteID);
    response->m_aServices[i].nicknameOfPeer = g_aRoutesTable[ucServiceIdx].m_unDestNickname;

//    response->m_aServices[i].period.u32 = g_aServicesTable[j].m_tPeriod * 320;
    response->m_aServices[i].period.u32 = g_aServicesTable[j].m_tPeriod.u32;  // 1/32ms
    response->m_aServices[i].routeId = g_aServicesTable[j].m_ucRouteID;
  }

  return ucRespCode;
  //TODO:
// return RCS_N00_Success;
//return RCS_E02_InvalidSelection;
//return RCS_E05_TooFewDataBytesReceived;
//return RCS_E06_DeviceSpecificCommandError;
//return RCM_W08_SetToNearestPossibleValue;
} // End - Execute_C800_ReadServiceList()


/*********** CMD 801 *************************/

uint16_t Execute_C801_DeleteService(C801_DeleteService_Req* request,
                                   C801_DeleteService_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
    return RCS_E16_AccessRestricted;
  
  // TODO: recheck command implementation
  // TODO: ??? move implementation to NET_Request
  // TODO: ??? add support for RCM_E68_DeleteNotAllowed response code
  // TODO: ??? add support for RCM_E67_ReasonCodeRejected response code


  uint8_t ucFoundIdx = FindServiceIndex(request->m_ucServiceId, NL_SEARCH_BY_SERVICE_ID);
  if(ucFoundIdx >= g_ucServicesNo)
  { // service not found
    return RCM_E65_EntryNotFound;
  }

  if( request->m_ucReason != ServiceDeletionReasonCodes_RequestedByPeer &&
      request->m_ucReason != ServiceDeletionReasonCodes_ServiceCantBeEstablished &&
      request->m_ucReason != ServiceDeletionReasonCodes_NetworkFailure )
    return RCM_E66_InvalidReasonCode;
  
  if(request->m_ucReason == ServiceDeletionReasonCodes_RequestedByPeer && request->m_ucServiceId < 0x80)
    return RCM_E68_DeleteNotAllowed;  // requestor not Timetable owner
      
  g_ucServicesNo--;
  memcpy( &g_aServicesTable[ucFoundIdx],
          &g_aServicesTable[ucFoundIdx + 1],
          (g_ucServicesNo - ucFoundIdx) * sizeof(NET_SERVICE_ATTRIBUTES) );

  response->m_ucServiceId = request->m_ucServiceId;
  response->m_ucReason = request->m_ucReason;
  response->m_ucNoOfServiceEntriesRemaining = MAX_SERVICES_NO - g_ucServicesNo;
  
  // request->m_ucServiceId cannot be 0 in Nivis stack implementation
  
  uint8_t ucNotifyStatus = 0;
  if(MAX_BURST_NO >= request->m_ucServiceId)
  { // should stop the burst, too?????
    if(g_stBurstModeTable[request->m_ucServiceId - 1].m_ucServiceId)
    {
      g_stBurstModeTable[request->m_ucServiceId - 1].m_ucServiceId = 0;
      ucNotifyStatus = 1;
    }
  }
  else if((MAX_EVENTS_NO + MAX_BURST_NO) >= request->m_ucServiceId)
  {
    if(g_stEventsTable[request->m_ucServiceId - MAX_BURST_NO - 1].m_ucServiceId)
    {
      g_stEventsTable[request->m_ucServiceId - MAX_BURST_NO - 1].m_ucServiceId = 0;
      ucNotifyStatus = 1;
    }
  }
//  else if(...)
//  {
//    // block transfer data mode table, etc
//  }
  else if(request->m_ucReason != ServiceDeletionReasonCodes_RequestedByPeer)
  {
    ucNotifyStatus = 1;
  }
  
  if(ucNotifyStatus)
  {
    g_stCmd48Data.data.standardizedStatus3 &= StandardizedStatus3Mask_CapacityDenied;
    APP_SetDeviceStatusFlag(MORE_STATUS_AVAILABLE);
  }
  return RCS_N00_Success;
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_EntryNotFound;
//  return RCM_E66_InvalidReasonCode;
//  return RCM_E67_ReasonCodeRejected;  // Service not deleted
//  return RCM_E68_DeleteNotAllowed;  
} // End - Execute_C801_DeleteService()


/*********** CMD 802 *************************/

uint16_t Execute_C802_ReadRouteList(C802_ReadRouteList_Req* request,
                                   C802_ReadRouteList_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes)
{
  if(g_ucRoutesNo == 0)
  { // route list is empty or no entries requested
    return RCS_E02_InvalidSelection;
  }

  uint8_t ucRouteIdx;
  uint8_t ucRespCode = RCS_N00_Success;

  // if route index is greater than the nr. of routes, the last route is returned
  if(request->m_ucRouteIndex >= g_ucRoutesNo)
  {
    ucRouteIdx = g_ucRoutesNo - 1;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    ucRouteIdx = request->m_ucRouteIndex;
  }

  if(request->m_ucNoOfEntriesToRead > g_ucRoutesNo - ucRouteIdx)
  {
    // if the nr. of requested entries is greater than the nr. of available entries,
    // the maximum nr. of entries is returned
    response->m_ucNoOfEntriesRead = g_ucRoutesNo - ucRouteIdx;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else if(0 == request->m_ucNoOfEntriesToRead)
  { // should be coerced to a valid value and RC8 returned
    response->m_ucNoOfEntriesRead = 1;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    response->m_ucNoOfEntriesRead = request->m_ucNoOfEntriesToRead;
  }

//  if( remainingBytes < C802_RespSize ) // already checked
//  {
//    return RCS_E60_PayloadTooLong;
//  }
//  else 
  if(4 + 6 * response->m_ucNoOfEntriesRead > remainingBytes)
  { // if more entries than buffer capacity -> limit the number of entries to the buffer capacity
    response->m_ucNoOfEntriesRead = (remainingBytes - 4)/6;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }

  response->m_ucRouteIndex = ucRouteIdx;
  response->m_ucNoOfActiveRoutes = g_ucRoutesNo;                                                    // ???
//  response->m_ucNoOfRoutesRemaining = g_ucRoutesNo - ucRouteIdx - response->m_ucNoOfEntriesRead;    // ???
  response->m_ucNoOfRoutesRemaining = MAX_ROUTES_NO - g_ucRoutesNo;    

  for(uint8_t i = 0, j = response->m_ucRouteIndex; i < response->m_ucNoOfEntriesRead; i++, j++)
  {
    response->m_aRoutes[i].routeId = g_aRoutesTable[j].m_ucRouteId;
    response->m_aRoutes[i].destinationNickname = g_aRoutesTable[j].m_unDestNickname;
    response->m_aRoutes[i].graphId = g_aRoutesTable[j].m_unGraphId;
    response->m_aRoutes[i].sourceRouteAttached = g_aRoutesTable[j].m_bSourceRouteAttached;
  }

  return ucRespCode;
//TODO:
//    return RCS_N00_Success;
//    return RCS_E02_InvalidSelection;
//    return RCS_E05_TooFewDataBytesReceived;
//    return RCM_W08_SetToNearestPossibleValue;  
} // End - Execute_C802_ReadRouteList()


/*********** CMD 803 *************************/

uint16_t Execute_C803_ReadSourceRoute(C803_ReadSourceRoute_Req* request,
                                     C803_ReadSourceRoute_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  uint8_t ucFoundIdx = FindSrcRouteIndex(request->m_ucRouteId);
  if(ucFoundIdx >= g_ucSrcRoutesNo)
  { // source route not found
    return RCM_E65_EntryNotFound;
  }

  response->m_ucRouteId = request->m_ucRouteId;
  response->m_ucNoOfHops = g_aSrcRoutesTable[ucFoundIdx].m_ucNoOfHops;

  // the unused entries in the m_aSrcRouteHops array contain the destination address

  if(2 + 2 * response->m_ucNoOfHops > remainingBytes)
    return RCS_E60_PayloadTooLong;

  uint8_t i, j, ucHops = response->m_ucNoOfHops;
  for(i = 0, j = 0; j < ucHops; j++)
  {
    if( g_aSrcRoutesTable[ucFoundIdx].m_aSrcRouteHops[j] != 0xFFFF )
    {
      response->m_aHopNicknames[i] = g_aSrcRoutesTable[ucFoundIdx].m_aSrcRouteHops[j];
      i++;
    }
    else
    { // eliminate any FFFF entries in the source route
      response->m_ucNoOfHops--;
    }
  }

  return RCS_N00_Success;
  //TODO:
//      return RCS_N00_Success;
//      return RCS_E05_TooFewDataBytesReceived;
//      return RCM_E65_EntryNotFound;
} // End - Execute_C803_ReadSourceRoute()


/*********** CMD 804 *************************/

uint16_t Execute_C804_ReadRadioCCAMode(C804_ReadRadioCCAMode_Req* request,
                                      C804_ReadRadioCCAMode_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes)
{
  if(g_sDllSharedParams.m_ucUseCCA)
    response->CCAMode = 1;
  else
    response->CCAMode = 0;

  return RCS_N00_Success;
//TODO:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;  
} // End - Execute_C804_ReadRadioCCAMode()


/*********** CMD 805 *************************/

uint16_t Execute_C805_WriteRadioCCAMode(C805_WriteRadioCCAMode_Req* request,
                                       C805_WriteRadioCCAMode_Resp* response,
                                       SHORT_ADDR srcAddr,
                                       uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)   // not network manager
    return RCS_E16_AccessRestricted;

  CK_WP_MODE

  if(request->CCAMode)
  {
    g_sDllSharedParams.m_ucUseCCA = 1;
    response->CCAMode = 1;

    if(request->CCAMode != 1) // wasn't a valid value
      return RCM_W08_SetToNearestPossibleValue;
  }
  else
  {
    g_sDllSharedParams.m_ucUseCCA = 0;
    response->CCAMode = 0;
  }

  return RCS_N00_Success;
  //TODO:
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCM_W08_SetToNearestPossibleValue;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;  
} // End - Execute_C805_WriteRadioCCAMode()


/*********** CMD 806 *************************/

uint16_t Execute_C806_ReadHandheldSuperframe( C806_ReadHandheldSuperframe_Req* request,
                                                   C806_ReadHandheldSuperframe_Resp* response,
                                                   SHORT_ADDR srcAddr,
                                                   uint8_t control, uint8_t remainingBytes)
{
  uint8_t rc = RCM_E09_NoHandheldSuperframe;
  
  MONITOR_ENTER();

  for(uint8 ucIndex = 0; ucIndex < g_ucDllAllSuperframesSize; ucIndex++)
  {
    if(g_aDllSuperframes[ucIndex].m_bHandheldSF)
    {
      response->m_ucSuperframeId          = g_aDllSuperframes[ucIndex].m_ucSuperframeID;
      response->m_ucSuperframeModeFlags   = (g_aDllSuperframes[ucIndex].m_eActive ? \
        (SuperframeModeFlagsMask_HandheldSuperframe | SuperframeModeFlagsMask_Active) : \
          SuperframeModeFlagsMask_HandheldSuperframe); // Table 47
      response->m_unNoOfSlotsInSuperframe = g_aDllSuperframes[ucIndex].m_unNumSlots;
      
      rc = RCS_N00_Success;
      break;
    }
  }
  MONITOR_EXIT();

  return rc;  
} // End - Execute_C806_ReadHandheldSuperframe()


/*********** CMD 807 *************************/

uint16_t Execute_C807_RequestHandheldSuperframeMode( C807_RequestHandheldSuperframeMode_Req* request,
                                               C807_RequestHandheldSuperframeMode_Resp* response,
                                               SHORT_ADDR srcAddr,
                                               uint8_t control, uint8_t remainingBytes)
{
  uint8_t rc = RCS_E02_InvalidSelection;
  
  MONITOR_ENTER();

  // spec155r1.1 - C807 must return "Invalid Selection" when Superframe mode does not designate it is for a handheld
  if(g_ucDllAllSuperframesSize && (request->m_ucSuperframeModeFlags & SuperframeModeFlagsMask_HandheldSuperframe))
  {
    uint8 ucIndex = DLL_FindSuperframe( request->m_ucSuperframeId );
    if(ucIndex != DLL_INVALID_SF_IDX && g_aDllSuperframes[ucIndex].m_bHandheldSF)
    {
      DLL_SetSuperframe(ucIndex, request->m_ucSuperframeModeFlags & SuperframeModeFlagsMask_Active);
      response->m_ucSuperframeId        = request->m_ucSuperframeId;
      response->m_ucSuperframeModeFlags = request->m_ucSuperframeModeFlags;

      rc = RCS_N00_Success;
    }
  }

  MONITOR_EXIT();

  return rc;

//TODO:  
     // return RCS_N00_Success;
     // return RCS_E02_InvalidSelection;
  // return RCS_E05_TooFewDataBytesReceived;
  // return RCS_E16_AccessRestricted;
} // End - Execute_C807_RequestHandheldSuperframeMode()


/*********** CMD 808 *************************/

uint16_t Execute_C808_ReadTimeToLive(C808_ReadTimeToLive_Req* request,
                                    C808_ReadTimeToLive_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{

  return NET_Request(NL_READ_TTL, &response->m_ucTimeToLive);
//TODO:
//  return RCS_N00_Success;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;    
} // End - Execute_C808_ReadTimeToLive()


/*********** CMD 809 *************************/

uint16_t Execute_C809_WriteTimeToLive(C809_WriteTimeToLive_Req* request,
                                     C809_WriteTimeToLive_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    if(!(control & MAINTENANCE_PORT_MASK) || g_ucNetState >= NET_STATE_QUARANTINED)
    { // cmd not received through maintenance port or device is connected to network
      return RCS_E16_AccessRestricted;
    }
  }

  CK_WP_MODE

  uint8_t ucRespCode = RCS_N00_Success;
  uint8_t ucTTL = request->m_ucTimeToLive;
  
  //TTL must be at least 8; if requested TTL is less, TTL must be set to 8 and RCM_W08_SetToNearestPossibleValue returned
  if(ucTTL < 8)
  {
    ucTTL = 8;
    ucRespCode = RCM_W08_SetToNearestPossibleValue;
  }
//  else if(ucTTL > 0x80)
//  {
//    ucTTL = 0x80;
//    ucRespCode = RCM_W08_SetToNearestPossibleValue;
//  }
  
  NET_Request(NL_WRITE_TTL, &ucTTL);

  response->m_ucTimeToLiveSet = ucTTL;

  return ucRespCode;
  //TODO:
//   return RCS_N00_Success;
//   return RCS_E05_TooFewDataBytesReceived;
//   return RCS_E06_DeviceSpecificCommandError;
//   return RCS_E07_InWriteProtectMode;
//   return RCM_W08_SetToNearestPossibleValue;
//   return RCS_E16_AccessRestricted;
//   return RCS_E32_Busy;
//   return RCS_E33_DelayedResponseInitiated;
//   return RCS_E34_DelayedResponseRunning;
//   return RCS_E35_DelayedResponseDead;
//   return RCS_E36_DelayedResponseConflict; 
} // End - Execute_C809_WriteTimeToLive()


/*********** CMD 810 *************************/

uint16_t Execute_C810_ReadJoinPriority( C810_ReadJoinPriority_Req* request,
                                       C810_ReadJoinPriority_Resp* response,
                                       SHORT_ADDR srcAddr,
                                       uint8_t control, uint8_t remainingBytes)
{
  return DLL_LM_Request(DLL_SERV_READ_JOIN_PRIORITY, response);
//TODO:  
// return RCS_N00_Success;  
} // End - Execute_C810_ReadJoinPriority()


/*********** CMD 811 *************************/

uint16_t Execute_C811_WriteJoinPriority( C811_WriteJoinPriority_Req* request,
                                        C811_WriteJoinPriority_Resp* response,
                                        SHORT_ADDR srcAddr,
                                        uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)   // not network manager
     return RCS_E16_AccessRestricted;

  response->JoinPriority = request->JoinPriority;
  return DLL_LM_Request(DLL_SERV_WRITE_JOIN_PRIORITY, response);
//TODO:  
// return RCS_N00_Success;
// return RCS_E05_TooFewDataBytesReceived;
// return RCM_W08_SetToNearestPossibleValue;
// return RCS_E16_AccessRestricted;
} // End - Execute_C811_WriteJoinPriority()


/*********** CMD 812 *************************/

uint16_t Execute_C812_ReadPacketReceivePriority( C812_ReadPacketReceivePriority_Req* request,
                                         C812_ReadPacketReceivePriority_Resp* response,
                                         SHORT_ADDR srcAddr,
                                         uint8_t control, uint8_t remainingBytes)
{
  return DLL_LM_Request(DLL_SERV_READ_PRIORITY_THRESHOLD, response);
//TODO:  
// return RCS_N00_Success;   
} // End - Execute_C812_ReadPacketReceivePriority()


/*********** CMD 813 *************************/

uint16_t Execute_C813_WritePacketReceivePriority( C813_WritePacketReceivePriority_Req* request,
                                          C813_WritePacketReceivePriority_Resp* response,
                                          SHORT_ADDR srcAddr,
                                          uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)   // not network manager
     return RCS_E16_AccessRestricted;

  response->PacketRecPriority = request->PacketRecPriority;
  return DLL_LM_Request(DLL_SERV_WRITE_PRIORITY_THRESHOLD, response);
//TODO:  
// return RCS_N00_Success;
// return RCS_E05_TooFewDataBytesReceived;
// return RCM_W08_SetToNearestPossibleValue;
// return RCS_E16_AccessRestricted;  
} // End - Execute_C813_WritePacketReceivePriority()


/*********** CMD 814 *************************/

uint16_t Execute_C814_ReadDeviceListEntries(C814_ReadDeviceListEntries_Req* request,
                                           C814_ReadDeviceListEntries_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes)
{
  // TODO: RCS_E60_PayloadTooLong

  // TBD
  return RCS_E64_CommandNotImplemented;
//TODO  
//  return RCS_N00_Success;
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCM_W08_SetToNearestPossibleValue;
} // End - Execute_C814_ReadDeviceListEntries()


/*********** CMD 815 *************************/

uint16_t Execute_C815_AddDeviceListTableEntry(C815_AddDeviceListTableEntry_Req* request,
                                             C815_AddDeviceListTableEntry_Resp* response,
                                             SHORT_ADDR srcAddr,
                                             uint8_t control, uint8_t remainingBytes)
{
//  if(C815_RespSize > remainingBytes)
//    return RCS_E60_PayloadTooLong;

  // TBD
//  SET_CONFIG_CHANGED;
  return RCS_E64_CommandNotImplemented;
//TODO:  
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCM_W08_DeviceAlreadyInList;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_NoMoreEntriesAvailable;
//  return RCM_E66_DeviceListConflict;
} // End - Execute_C815_AddDeviceListTableEntry()


/*********** CMD 816 *************************/

uint16_t Execute_C816_DeleteDeviceListTableEntry(C816_DeleteDeviceListTableEntry_Req* request,
                                                C816_DeleteDeviceListTableEntry_Resp* response,
                                                SHORT_ADDR srcAddr,
                                                uint8_t control, uint8_t remainingBytes)
{
//  if(C816_RespSize > remainingBytes)
//    return RCS_E60_PayloadTooLong;

  // TBD
//  SET_CONFIG_CHANGED;
  return RCS_E64_CommandNotImplemented;
//TODO:  
//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E66_DeviceListConflict;
} // End - Execute_C816_DeleteDeviceListTableEntry()


/*********** CMD 817 *************************/

uint16_t Execute_C817_ReadChannelBlacklist(C817_ReadChannelBlacklist_Req* request,
                                          C817_ReadChannelBlacklist_Resp* response,
                                          SHORT_ADDR srcAddr,
                                          uint8_t control, uint8_t remainingBytes)
{
  response->m_unCurrentChannelMapArray = g_unDllChannelMap;
  response->m_ucNoOfBitsInCurrentChannelMapArray = g_ucDllTotalChannels;

  // TODO: pending channel map???
  response->m_unPendingChannelMapArray = g_unDllChannelMap;

  return RCS_N00_Success;
} // End - Execute_C817_ReadChannelBlacklist()


/*********** CMD 818 *************************/

//  C818 has to be implemented only by the Gateway - AccessPoint 
uint16_t Execute_C818_WriteChannelBlacklist(C818_WriteChannelBlacklist_Req* request,
                                           C818_WriteChannelBlacklist_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes)
{
  // C818 will be sent by NM to AccessPoint Transceiver only, after JoinResponse,
  // and before the Sf & links setting
  // The effect will be immediately, not after reset -no need for NVM for pending ChMap
  
  if(srcAddr != NM_NICKNAME)   
  { // not network manager
     return RCS_E16_AccessRestricted;
  }
  SET_CONFIG_CHANGED
  uint8 ucRet;
  
  MONITOR_ENTER();
  if( !(ucRet = DLL_CheckAndSetChannelMap(0, request->m_ucNoOfBitsInNewChannelMapArray, request->m_unPendingChannelMapArray)))
  {
    response->m_ucNoOfBitsInNewChannelMapArray = g_ucDllTotalChannels;
    response->m_unPendingChannelMapArray = g_unDllChannelMap;
    
    //SET_CONFIG_CHANGED;
  }
  
  MONITOR_EXIT();
  
  return ucRet; // RCS_N00_Success                     // ... or
                // RCS_E03_PassedParameterTooLarge     // more than 16 bit ch map ... or
                // RCM_E65_IllegalFrequencyChannelBits // ch16 not allowed for 802.15.4 PHY -seeC818 description
      
//  return RCS_N00_Success;
//  return RCS_E03_PassedParameterTooLarge;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_IllegalFrequencyChannelBits;
} // End - Execute_C818_WriteChannelBlacklist()


/*********** CMD 819 *************************/

uint16_t Execute_C819_ReadBackOffExponent( C819_ReadBackOffExponent_Req* request,
                                          C819_ReadBackOffExponent_Resp* response,
                                          SHORT_ADDR srcAddr,
                                          uint8_t control, uint8_t remainingBytes)
{
  return DLL_LM_Request(DLL_SERV_READ_MAX_BACKOFF_EXPONENT, response);
//TODO:
// return RCS_N00_Success;
// return RCS_E06_DeviceSpecificCommandError;
// return RCS_E32_Busy;
} // End - Execute_C819_ReadBackOffExponent()


/*********** CMD 820 *************************/

uint16_t Execute_C820_WriteBackOffExponent( C820_WriteBackOffExponent_Req* request,
                                           C820_WriteBackOffExponent_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)   // not network manager
     return RCS_E16_AccessRestricted;

  CK_WP_MODE
    
  response->MaxBackOffExp   = request->MaxBackOffExp;
  return DLL_LM_Request(DLL_SERV_WRITE_MAX_BACKOFF_EXPONENT, response);

//TODO:  
     //  return RCS_N00_Success;
 //  return RCS_E03_PassedParameterTooLarge;
 //  return RCS_E04_PassedParameterTooSmall;
 //  return RCS_E05_TooFewDataBytesReceived;
 //  return RCS_E06_DeviceSpecificCommandError;
 //  return RCS_E07_InWriteProtectMode;
     //  return RCM_W08_SetToNearestPossibleValue;
     //  return RCS_E16_AccessRestricted;
 //  return RCS_E32_Busy;
 //  return RCS_E33_DelayedResponseInitiated;
 //  return RCS_E34_DelayedResponseRunning;
 //  return RCS_E35_DelayedResponsDead;
 //  return RCS_E36_DelayedResponseConflict;
} // End - Execute_C820_WriteBackOffExponent()


/*********** CMD 821 *************************/

/*  C821 has to be implemented only by the Gateway
uint16_t Execute_C821_WriteNetworkAccessMode(C821_WriteNetworkAccessMode_Req* request,
                                            C821_WriteNetworkAccessMode_Resp* response,
                                            SHORT_ADDR srcAddr,
                                            uint8_t control, uint8_t remainingBytes)
{
  CK_WP_MODE
  // TBD
  SET_CONFIG_CHANGED;
//TODO:
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E07_InWriteProtectMode;
//  return RCS_E16_AccessRestricted;
//  return RCS_E32_Busy;
//  return RCS_E33_DelayedResponseInitiated;
//  return RCS_E34_DelayedResponseRunning;
//  return RCS_E35_DelayedResponseDead;
//  return RCS_E36_DelayedResponseConflict;   
} // End - Execute_C821_WriteNetworkAccessMode()
*/


/*********** CMD 822 *************************/

/*  C822 has to be implemented only by the Gateway
uint16_t Execute_C822_ReadNetworkAccessMode(C822_ReadNetworkAccessMode_Req* request,
                                           C822_ReadNetworkAccessMode_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes)
{
  // TBD
  return RCS_N00_Success;
//TODO:
//  return RCS_E06_DeviceSpecificCommandError;
//  return RCS_E32_Busy;
} // End - Execute_C822_ReadNetworkAccessMode()
*/


/*********** CMD 823 *************************/

/*  C823 has to be implemented only by the Network Manager
uint16_t Execute_C823_RequestSession(C823_RequestSession_Req* request,
                                    C823_RequestSession_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{
  // TBD
  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_UnknownNickname;
//  return RCM_E66_PeerHasInsufficientCapacityForAnotherSession;
} // End - Execute_C823_RequestSession()
*/


/*********************************************/
/*********** GW and NM Commands **************/

//Execute_C832_ReadNetworkDeviceIdentity
//Execute_C833_ReadNetworkDeviceNeighbourHealth
//Execute_C834_ReadNetworkTopologyInformation
//Execute_C835_ReadBurstMessageList
//Execute_C836_FlushCacheResponses
//Execute_C837_WriteUpdateNotificationBitMask
//Execute_C838_ReadUpdateNotificationBitMask
//Execute_C839_ChangeNotification
//Execute_C840_ReadDeviceStatistics
//Execute_C841_ReadNetworkDeviceIdentityByNickname
//Execute_C842_WriteDeviceSchedulingFlags
//Execute_C843_ReadDeviceSchedulingFlags
//Execute_C844_ReadNetworkConstraints
//Execute_C845_WriteNetworkConstraints


/*********************************************/
/***** Network Management Config Commands ****/


/*********** CMD 960 *************************/

uint16_t Execute_C960_DisconnectDevice(C960_DisconnectDevice_Req* request,
                                      C960_DisconnectDevice_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes)
{
  #ifndef TEST_DISCONNECT
  if(NM_NICKNAME != srcAddr)
    return RCS_E16_AccessRestricted; 
  #endif

  response->m_eReason = request->m_eReason;
  DLL_LM_Request(DLL_SERV_DISCONNECT, NULL);
  g_ucMarkDisconnectHandle = 1;
  return RCS_N00_Success;

//TODO:  
//  return RCS_N00_Success;
//  return RCS_E16_AccessRestricted;
} // End - Execute_C960_DisconnectDevice()


/*********** CMD 961 *************************/

uint16_t Execute_C961_WriteNetworkKey(C961_WriteNetworkKey_Req* request,
                                     C961_WriteNetworkKey_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  if((g_ucJoinCmdFlags & BIT0) && request->m_ucTruncated) // maybe should check if ExecTime == 0 instead of Truncated ??
    return RCM_E66_InvalidExecutionTime;  // C961 can only be truncated first time during the join
  
//  uint8_t delayed = 0;
//  if(request->m_tExecutionTime[0] || request->m_tExecutionTime[1] ||
//     request->m_tExecutionTime[2] || request->m_tExecutionTime[3] ||
//     request->m_tExecutionTime[4])
//  { // execution time is different from zero (delayed execution)
//    uint8_t aASN[5];
//    DLL_GetASN(aASN);
//    if(memcmp(request->m_tExecutionTime, aASN, 5) < 0)
//    { // execution time is smaller than current ASN (in the past)
//      return RCM_E66_InvalidExecutionTime;
//    }
//    delayed = 1;
//  }

  uint8_t delayed = checkExecTimeAndRespSize(request->m_tExecutionTime, request->m_ucTruncated, (uint16_t)remainingBytes - C961_RespSize);
  if(delayed > 1)
    return delayed; // RCM_E66_InvalidExecutionTime or RCS_E60_PayloadTooLong

  // request and response need to have the same structure
  memcpy(response, request, sizeof(C961_WriteNetworkKey_Resp));

  if(delayed)
  {
    MONITOR_ENTER();
    memcpy(&g_sPendingCmdsData.m_sC961_Data, response, sizeof(C961_WriteNetworkKey_Resp));
    g_sPendingCmdsData.m_bC961 = 1;
    MONITOR_EXIT();

    return RCS_N00_Success;
  }

  // TODO: ??? RCM_E65_KeyChangeFailed ???
  DLL_LM_Request(DLL_SERV_WRITE_NETWORK_KEY, response);

  if (NET_JOIN_REQ_ADMISSION == g_ucNetJoinState)
  {
    g_ucJoinCmdFlags |= BIT0;
    g_stC769.joinRetryTimer.u32 = (g_ulJoinRspTimeout - g_ulJoinRspTimer) * (250UL * 32); // save the time since the last JReq was sent for future C769 requests
  }

  return RCS_N00_Success;
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_KeyChangeFailed;
//  return RCM_E66_InvalidExecutionTime;
} // End - Execute_C961_WriteNetworkKey()


/*********** CMD 962 *************************/

uint16_t Execute_C962_WriteDeviceNicknameAddress(C962_WriteDeviceNicknameAddress_Req* request,
                                                C962_WriteDeviceNicknameAddress_Resp* response,
                                                SHORT_ADDR srcAddr,
                                                uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  if(request->m_unNickname == NM_NICKNAME || request->m_unNickname == GW_NICKNAME || request->m_unNickname == 0xFFFF || request->m_unNickname == 0x0000)
  { // network manager, gateway, broadcast address
    return RCM_E65_InvalidNickname;
  }

  g_unNickname = request->m_unNickname;
  response->m_unNickname = request->m_unNickname;

  if (NET_JOIN_REQ_ADMISSION == g_ucNetJoinState)
    g_ucJoinCmdFlags |= BIT1;

  return RCS_N00_Success;
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_InvalidNickname;
} // End - Execute_C962_WriteDeviceNicknameAddress()


/*********** CMD 963 *************************/

uint16_t Execute_C963_WriteSession(C963_WriteSession_Req* request,
                                  C963_WriteSession_Resp* response,
                                  SHORT_ADDR srcAddr,
                                  uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  if( request->m_eSessionType > SessionTypeCode_Join )
    return RCM_E67_SessionTypeInvalid;

  if((request->m_unPeerNickname == 0xFFFF) || (request->m_unPeerNickname == 0x0000)
//      || (request->m_unPeerNickname == 0xABCD) // not in C963 spec, but requested by TML204B!!! don't know for what reason????
      || (!memcmp(request->m_aPeerUniqueID, "\0\0\0\0\0", 5))
    )
  {
    return RCS_E02_InvalidSelection; // not in C963 spec, but requested by TML204B
  }

#ifdef HCF_EXECTIME_DEBUG  // just for debugging
  if(request->m_tExecutionTime[0] || request->m_tExecutionTime[1] ||
     request->m_tExecutionTime[2] || request->m_tExecutionTime[3] ||
     request->m_tExecutionTime[4])
  {
      response->m_eSessionType = 0x55;
  }
#endif
  
//  uint8_t delayed = 0;
////  if(request->m_tExecutionTime[0] || request->m_tExecutionTime[1] ||
////     request->m_tExecutionTime[2] || request->m_tExecutionTime[3] ||
////     request->m_tExecutionTime[4])
//  if(memcmp(request->m_tExecutionTime, "\0\0\0\0\0", 5))
//  { // execution time is different from zero (delayed execution is intended)
//    // check if execution time is smaller than current ASN (in the past)
////    uint8_t aASN[5];
////    DLL_GetASN(aASN);
////    if(memcmp(request->m_tExecutionTime, aASN, 5) < 0)  // incorrect since ASN can rollover
//    
//    unsigned long long ullExecTime, ullCrtASN;
//    InverseMemcpy(&ullExecTime, request->m_tExecutionTime, 5);
//    MONITOR_ENTER();
//    ullCrtASN = g_sDllASN.m_ullASN;
//    MONITOR_EXIT();
//    if((ullExecTime - ullCrtASN) >= 0x8000000000ULL) // half of the ASN domain
//    { 
//      return RCM_E66_InvalidExecutionTime;
//    }
//    delayed = 1;
//  }
//  
//  if(((request->m_ucTruncated) ? C963_RespSize : (C963_RespSize + 5)) > remainingBytes)
//    return RCS_E60_PayloadTooLong;

  uint8_t delayed = checkExecTimeAndRespSize(request->m_tExecutionTime, request->m_ucTruncated, (uint16_t)remainingBytes - C963_RespSize);
  if(delayed > 1)
    return delayed; // RCM_E66_InvalidExecutionTime or RCS_E60_PayloadTooLong

  response->m_eSessionType = request->m_eSessionType;
  response->m_unPeerNickname = request->m_unPeerNickname;

  response->m_unPeerExpandedDeviceTypeCode = ((uint16_t)request->m_aPeerUniqueID[0] << 8) |
                                             ((uint16_t)request->m_aPeerUniqueID[1]);
  response->m_ulPeerDeviceID = ((uint32_t)request->m_aPeerUniqueID[2] << 16) |
                               ((uint32_t)request->m_aPeerUniqueID[3] << 8) |
                               ((uint32_t)request->m_aPeerUniqueID[4]);
  memcpy(response->m_aKeyValue, request->m_aKeyValue, 16);
  response->m_ucTruncated = request->m_ucTruncated;
  memcpy(response->m_tExecutionTime, request->m_tExecutionTime, 5);
  response->m_ulPeerNonceCounterValue = request->m_ulPeerNonceCounterValue;

  //
  //TODO: interrupts must be disabled if g_sPendingCmdsData.m_aC963_Data will be processed from an interrupt!!!!!!!!
  //
  
  /*
    for a m_aC963_Data entry:
      if m_ucRemainingSessionsNo = FF -> the command will create a new session
      if m_ucRemainingSessionsNo =  0 -> the command will modify an existing session
  */

  NET_SESSION_ATTRIBUTES* pSession = FindSessionByType(request->m_eSessionType, request->m_unPeerNickname);
  uint8_t ucSessions = g_ucSessionsNo;
  uint8_t i;

  if(delayed)
  {
    uint8_t ucAlreadyPending = 0;
    uint8_t ucIdx;

    for(i = 0 ; i < g_sPendingCmdsData.m_ucNoOfSessions; i++)
    {
      if(g_sPendingCmdsData.m_aC963_Data[i].m_ucRemainingSessionsNo == 0xFF)
      { // count sessions waiting to be created (which will not modify existing sessions)
        ucSessions++;
      }
      if(g_sPendingCmdsData.m_aC963_Data[i].m_eSessionType == request->m_eSessionType &&
         g_sPendingCmdsData.m_aC963_Data[i].m_unPeerNickname == request->m_unPeerNickname)
      { // modification of this session already pending (need to update command data)
        ucIdx = i;  // save index; entry is overwritten later
        ucAlreadyPending = 1;
      }
    }

    if(!ucAlreadyPending)
    { // no modifications pending for this session
      if(ucSessions >= MAX_SESSIONS_NO)
      { // session table is full (existing sessions from session table + sessions waiting to be created)
        return RCM_E65_NoMoreEntriesAvailable;
      }

      // i == g_sPendingCmdsData.m_ucNoOfSessions
      // write a new entry
      memcpy(&g_sPendingCmdsData.m_aC963_Data[i], response, sizeof(C963_WriteSession_Resp));
      if(pSession == NULL)
      {
        g_sPendingCmdsData.m_aC963_Data[i].m_ucRemainingSessionsNo = 0xFF;
        ucSessions++;
      }
      else
      {
        g_sPendingCmdsData.m_aC963_Data[i].m_ucRemainingSessionsNo = 0x00;
      }

      g_sPendingCmdsData.m_aC963_Idx[i] = i;
      g_sPendingCmdsData.m_ucNoOfSessions++;
    }
    else
    {
      i = g_sPendingCmdsData.m_aC963_Data[ucIdx].m_ucRemainingSessionsNo; // save old value
      // overwrite pending modifications for this session
      memcpy(&g_sPendingCmdsData.m_aC963_Data[ucIdx], response, sizeof(C963_WriteSession_Resp));
      g_sPendingCmdsData.m_aC963_Data[ucIdx].m_ucRemainingSessionsNo = i; // restore old value
    }
    g_sPendingCmdsData.m_bC963 = 1;

    if(pSession != NULL)
    { // if session exists, set response nonce to the value currently in use
      response->m_ulPeerNonceCounterValue = pSession->m_ulPeerNonceCounter;
    }
    response->m_ucRemainingSessionsNo = MAX_SESSIONS_NO - ucSessions;

    // sort m_aC963_Idx desc. by exec time (from m_aC963_Data)
    uint8_t j;
    for(i = 0; i < g_sPendingCmdsData.m_ucNoOfSessions - 1; i++)
    {
      for(j = i + 1; j < g_sPendingCmdsData.m_ucNoOfSessions; j++)
      {
        if(g_sPendingCmdsData.m_aC963_Data[g_sPendingCmdsData.m_aC963_Idx[i]].m_tExecutionTime <
           g_sPendingCmdsData.m_aC963_Data[g_sPendingCmdsData.m_aC963_Idx[j]].m_tExecutionTime)
        {
          uint8_t ucIdx = g_sPendingCmdsData.m_aC963_Idx[i];
          g_sPendingCmdsData.m_aC963_Idx[i] = g_sPendingCmdsData.m_aC963_Idx[j];
          g_sPendingCmdsData.m_aC963_Idx[j] = ucIdx;
        }
      }
    }

    return RCS_N00_Success;
  }


  // immediate execution - just possible for a new created session??? (see Addendum)

  for(i = 0 ; i < g_sPendingCmdsData.m_ucNoOfSessions; i++)
  {
    if(g_sPendingCmdsData.m_aC963_Data[i].m_ucRemainingSessionsNo == 0xFF)
    { // count sessions waiting to be created (which will not modify existing sessions)
      ucSessions++;
    }
  }

  if(pSession == NULL)
  { // new session
    if(ucSessions >= MAX_SESSIONS_NO)
    { // session table is full (existing sessions from session table + sessions waiting to be created)
      return RCM_E65_NoMoreEntriesAvailable;
    }
    ucSessions++;
  }
  else
  { // if session exists, set response nonce to the value currently in use
    response->m_ulPeerNonceCounterValue = pSession->m_ulPeerNonceCounter;
  }

  // TODO: old key has to be retained 2 * max PDU age (NET_Request)
  NET_Request(NL_WRITE_SESSION_KEY, response);

  response->m_ucRemainingSessionsNo = MAX_SESSIONS_NO - ucSessions;

//  if (NET_JOIN_REQ_ADMISSION == g_ucNetJoinState)
//    g_ucJoinCmdFlags |= BIT2;   // moved in NET_Request()

  return RCS_N00_Success;
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_NoMoreEntriesAvailable;
//  return RCM_E66_InvalidExecutionTime;
//  return RCM_E67_SessionTypeInvalid;
} // End - Execute_C963_WriteSession()


/*********** CMD 964 *************************/

uint16_t Execute_C964_DeleteSession(C964_DeleteSession_Req* request,
                                   C964_DeleteSession_Resp* response,
                                   SHORT_ADDR srcAddr,
                                   uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }
  if(request->m_eSessionType > SessionTypeCode_Join)
  { // see HCFTracker  Bug#3454 - invalid session type
    return RCS_E02_InvalidSelection;
  }
    
  response->m_eSessionType = request->m_eSessionType;
  response->m_unPeerNickname = request->m_unPeerNickname;
  // response->m_ucRemainingSessionsNo is set by NET_Request

  return NET_Request(NL_DEL_SESSION, response);

//  return RCS_N00_Success;
//  return RCS_E02_InvalidSelection;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_SessionWithGivenPeerDoesntExist;
} // End - Execute_C964_DeleteSession()


/*********** CMD 965 *************************/

uint16_t Execute_C965_WriteSuperframe(C965_WriteSuperframe_Req* request,
                                     C965_WriteSuperframe_Resp* response,
                                     SHORT_ADDR srcAddr,
                                     uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  // bits 6 to 1 in SuperframeMode must be 0 -see TML203H
  if(request->m_ucSuperframeMode & (~0x81))
    return RCM_E68_InvalidSuperframeMode;
    
  if(request->m_unSuperframeSlotsNo == 0)
  {
    return RCM_E67_InvalidNoOfSlots;
  }
  
  uint16_t rc = RCS_N00_Success;
  MONITOR_ENTER();
  do
  {
    uint8_t ucSFIdx = DLL_FindSuperframe(request->m_ucSuperframeID);
    if(ucSFIdx == DLL_INVALID_SF_IDX)
    { // new superframe
      if(g_ucDllAllSuperframesSize >= DLL_MAX_SUPERFRAMES)
      { // superframe table is full
        rc = RCM_E65_NoMoreEntriesAvailable;
        break;
      }
    }
    else if(request->m_ucSuperframeMode & 0x01)
    { // superframe already exists and active flag is set
      if((g_aDllSuperframes[ucSFIdx].m_bSetEnabled || g_aDllSuperframes[ucSFIdx].m_eActive) &&
         (g_aDllSuperframes[ucSFIdx].m_bModifySlots == 0 &&
          g_aDllSuperframes[ucSFIdx].m_unNumSlots != request->m_unSuperframeSlotsNo ||
            g_aDllSuperframes[ucSFIdx].m_bModifySlots == 1 &&
              g_aDllSuperframes[ucSFIdx].m_unNewSlots != request->m_unSuperframeSlotsNo))
      {
//        rc = RCM_E68_InvalidSuperframeMode;  // slot number can't be modified while superframe is active
        rc = RCM_E67_InvalidNoOfSlots;  // slot number can't be modified while superframe is active
        break;
      }
    }
    
    if(request->m_ucSuperframeMode & 0x80)  // handheld superframe flag is set
    {
      for(uint8_t i = 0; i < g_ucDllAllSuperframesSize; i++)
      {
        if(g_aDllSuperframes[i].m_ucSuperframeID != request->m_ucSuperframeID)
        {
          if(g_aDllSuperframes[i].m_bHandheldSF)
          {
            // handheld superframe flag is already set in another superframe
            rc = RCM_E68_InvalidSuperframeMode;
            break;
          }
        }
      }
      if(rc)
        break;
    }
    
    uint8_t delayed = checkExecTimeAndRespSize(request->m_tExecutionTime, request->m_ucTruncated, (uint16_t)remainingBytes - C965_RespSize);
    if(delayed > 1)
    {
      rc = delayed; // RCM_E66_InvalidExecutionTime or RCS_E60_PayloadTooLong
      break;
    }
    
    response->m_ucSuperframeID = request->m_ucSuperframeID;
    response->m_unSuperframeSlotsNo = request->m_unSuperframeSlotsNo;
    response->m_ucSuperframeMode = request->m_ucSuperframeMode;
    response->m_ucTruncated = request->m_ucTruncated;
    memcpy(response->m_tExecutionTime, request->m_tExecutionTime, 5);
    
    
    /*
    for a m_aC965_Data entry:
    if m_ucRemainingSuperframesNo = FF -> the command will create a new superframe
    if m_ucRemainingSuperframesNo =  0 -> the command will modify an existing superframe
    */
    
    uint8_t ucSuperframes = g_ucDllAllSuperframesSize;
    uint8_t i;
    
    if(delayed)
    {
      uint8_t ucAlreadyPending = 0;
      uint8_t ucIdx;
      
      for(i = 0 ; i < g_sPendingCmdsData.m_ucNoOfSuperframes; i++)
      {
        if(g_sPendingCmdsData.m_aC965_Data[i].m_ucRemainingSuperframesNo == 0xFF)
        { // count superframes waiting to be created (which will not modify existing superframes)
          ucSuperframes++;
        }
        if(g_sPendingCmdsData.m_aC965_Data[i].m_ucSuperframeID == request->m_ucSuperframeID)
        { // modification of this superframe already pending (need to update command data)
          ucIdx = i;  // save index; entry is overwritten later
          ucAlreadyPending = 1;
        }
      }
      
      if(!ucAlreadyPending)
      { // no modifications pending for this superframe
        if(ucSuperframes >= DLL_MAX_SUPERFRAMES)
        { // superframe table is full (existing superframes from superframe table + superframes waiting to be created)
          rc = RCM_E65_NoMoreEntriesAvailable;
          break;
        }
        
        // i == g_sPendingCmdsData.m_ucNoOfSuperframes
        // write a new entry
        memcpy(&g_sPendingCmdsData.m_aC965_Data[i], response, sizeof(C965_WriteSuperframe_Resp));
        if(ucSFIdx == DLL_INVALID_SF_IDX)
        {
          g_sPendingCmdsData.m_aC965_Data[i].m_ucRemainingSuperframesNo = 0xFF;
          ucSuperframes++;
        }
        else
        {
          g_sPendingCmdsData.m_aC965_Data[i].m_ucRemainingSuperframesNo = 0x00;
        }
        
        g_sPendingCmdsData.m_ucNoOfSuperframes++;
      }
      else
      {
        i = g_sPendingCmdsData.m_aC965_Data[ucIdx].m_ucRemainingSuperframesNo; // save old value
        // overwrite pending modifications for this superframe
        memcpy(&g_sPendingCmdsData.m_aC965_Data[ucIdx], response, sizeof(C965_WriteSuperframe_Resp));
        g_sPendingCmdsData.m_aC965_Data[ucIdx].m_ucRemainingSuperframesNo = i; // restore old value
      }
      g_sPendingCmdsData.m_bC965 = 1;
      
      response->m_ucRemainingSuperframesNo = DLL_MAX_SUPERFRAMES - ucSuperframes;
      
      // sort m_aC965_Data desc. by exec time
      uint8_t j;
      for(i = 0; i < g_sPendingCmdsData.m_ucNoOfSuperframes - 1; i++)
      {
        for(j = i + 1; j < g_sPendingCmdsData.m_ucNoOfSuperframes; j++)
        {
          if(g_sPendingCmdsData.m_aC965_Data[i].m_tExecutionTime < g_sPendingCmdsData.m_aC965_Data[j].m_tExecutionTime)
          {
            C965_WriteSuperframe_Resp sSuperframe;
            memcpy(&sSuperframe, &g_sPendingCmdsData.m_aC965_Data[i], sizeof(C965_WriteSuperframe_Resp));
            memcpy(&g_sPendingCmdsData.m_aC965_Data[i], &g_sPendingCmdsData.m_aC965_Data[j],
                   sizeof(C965_WriteSuperframe_Resp));
            memcpy(&g_sPendingCmdsData.m_aC965_Data[j], &sSuperframe, sizeof(C965_WriteSuperframe_Resp));
          }
        }
      }
      
      //rc = RCS_N00_Success;
      break;
    }
    
    
    // immediate execution
    
    for(i = 0 ; i < g_sPendingCmdsData.m_ucNoOfSuperframes; i++)
    {
      if(g_sPendingCmdsData.m_aC965_Data[i].m_ucRemainingSuperframesNo == 0xFF)
      { // count superframes waiting to be created (which will not modify existing superframes)
        ucSuperframes++;
      }
    }
    
    if(ucSFIdx == DLL_INVALID_SF_IDX)
    { // new superframe
      if(ucSuperframes >= DLL_MAX_SUPERFRAMES)
      { // superframe table is full (existing superframes from superframe table + superframes waiting to be created)
        rc = RCM_E65_NoMoreEntriesAvailable;
        break;
      }
      ucSuperframes++;
      
      //    // test only !!!!!!!!!!!! -to permit creation of an active superframe
      //    DLL_LM_Request(DLL_SERV_WRITE_SUPERFRAME, response);
    }
    
    DLL_LM_Request(DLL_SERV_WRITE_SUPERFRAME, response);
    
    // response->RemainingSuperframeEntriesNo is set by DLL_LM_Request
    
    //  if ((NET_JOIN_LOOSELY_COUPLED == g_ucNetJoinState) && (request->m_ucSuperframeMode & SuperframeModeFlagsMask_Active))
    //    g_ucJoinCmdFlags |= BIT3;   // moved in DLL_LM_Request()
    
    //rc = RCS_N00_Success;
    break;
  } while (FALSE);

  MONITOR_EXIT();
  
  return rc;
  
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_NoMoreEntriesAvailable;
//  return RCM_E66_InvalidExecutionTime;
//  return RCM_E67_InvalidNoOfSlots;
//  return RCM_E68_InvalidSuperframeMode;
} // End - Execute_C965_WriteSuperframe()


/*********** CMD 966 *************************/

uint16_t Execute_C966_DeleteSuperframe(C966_DeleteSuperframe_Req* request,
                                      C966_DeleteSuperframe_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  response->m_ucSuperframeID = request->m_ucSuperframeID;
  // response->RemainingSuperframeEntriesNo is set by DLL_LM_Request

  return DLL_LM_Request( DLL_SERV_DEL_SUPERFRAME, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_EntryNotFound;
} // End - Execute_C966_DeleteSuperframe()


/*********** CMD 967 *************************/

uint16_t Execute_C967_WriteLink(C967_WriteLink_Req* request,
                               C967_WriteLink_Resp* response,
                               SHORT_ADDR srcAddr,
                               uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  // avoid a link to itself - not in spec r7.2, but requested by TML201B
  if(((request->m_unNeighborNickname != 0xFFFF) && (request->m_unNeighborNickname == g_unNickname)) ||
        (request->m_unNeighborNickname == 0x0000))
    return RCS_E02_InvalidSelection;  // link to itself or to 0000

  if(request->m_eLinkType > LinkType_Join)
    return RCM_E71_InvalidLinkType;

  response->m_ucSuperframeID = request->m_ucSuperframeID;
  response->m_unSlotNumber = request->m_unSlotNumber;
  response->m_ucChannelOffset = request->m_ucChannelOffset;
  response->m_unNeighborNickname = request->m_unNeighborNickname;
  response->m_ucLinkOptions= request->m_ucLinkOptions;
  response->m_eLinkType = request->m_eLinkType;
  // response->m_unRemainingLinksNo is set by DLL_LM_Request

  return DLL_LM_Request( DLL_SERV_ADD_LINK, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_NoMoreEntriesAvailable;
//  return RCM_E66_LinkAlreadyExists;
//  return RCM_E67_UnknownSuperframeID;
//  return RCM_E68_InvalidSlotNumber;
//  return RCM_E69_InvalidLinkOptions;
//  return RCM_E70_InvalidChannelOffset;
//  return RCM_E71_InvalidLinkType;
//  return RCM_E72_NoMoreNeighborsAvailable;
} // End - Execute_C967_WriteLink()


/*********** CMD 968 *************************/

uint16_t Execute_C968_DeleteLink(C968_DeleteLink_Req* request,
                                C968_DeleteLink_Resp* response,
                                SHORT_ADDR srcAddr,
                                uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  response->m_ucSuperframeID = request->m_ucSuperframeID;
  response->m_unSlotNumber = request->m_unSlotNumber;
  response->m_unNeighborNickname = request->m_unNeighborNickname;
  // response->m_unRemainingLinksNo is set by DLL_LM_Request

  return DLL_LM_Request( DLL_SERV_DEL_LINK, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_LinkNotFound;  
} // End - Execute_C968_DeleteLink()


/*********** CMD 969 *************************/

uint16_t Execute_C969_WriteGraphNeighbourPair(C969_WriteGraphNeighbourPair_Req* request,
                                             C969_WriteGraphNeighbourPair_Resp* response,
                                             SHORT_ADDR srcAddr,
                                             uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  if((request->m_unNeighborNickname == 0x0000) || (request->m_unGraphID == 0xFFFF))
    return RCS_E02_InvalidSelection;
  
  response->m_unGraphID = request->m_unGraphID;
  response->m_unNeighborNickname = request->m_unNeighborNickname;
  // response->m_ucRemainingConnectionsNo is set by DLL_LM_Request

  return DLL_LM_Request( DLL_SERV_ADD_CONNECTION, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_NoMoreEntriesAvailable;
//  return RCM_E66_UnknownNickname;
//  return RCM_E67_CantAddEdgeWhenSuperframeUsedAsGraphEquiv;
} // End - Execute_C969_WriteGraphNeighbourPair()


/*********** CMD 970 *************************/

uint16_t Execute_C970_DeleteGraphConnection(C970_DeleteGraphConnection_Req* request,
                                           C970_DeleteGraphConnection_Resp* response,
                                           SHORT_ADDR srcAddr,
                                           uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  if(request->m_unGraphID < 0x100)
    return RCM_E65_EntryNotFound; // see test spec TML205E

  if(request->m_unNeighborNickname == 0x0000)
    return RCS_E02_InvalidSelection;

  response->m_unGraphID = request->m_unGraphID;
  response->m_unNeighborNickname = request->m_unNeighborNickname;
  // response->m_ucRemainingConnectionsNo is set by DLL_LM_Request

  return DLL_LM_Request( DLL_SERV_DEL_CONNECTION, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_EntryNotFound;  
} // End - Execute_C970_DeleteGraphConnection()


/*********** CMD 971 *************************/

uint16_t Execute_C971_WriteNeighbourPropertyFlag(C971_WriteNeighbourPropertyFlag_Req* request,
                                                C971_WriteNeighbourPropertyFlag_Resp* response,
                                                SHORT_ADDR srcAddr,
                                                uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  uint8_t ucRetVal = RCS_N00_Success;
  
  MONITOR_ENTER();
  
  do
  {
    uint8_t ucNeighIdx = DLL_FindShortNeighbor( request->m_unNeighborNickname );
    
#ifndef BACKBONE_SUPPORT   
    
    if( ucNeighIdx == DLL_INVALID_NEIGH_IDX )
    { // neighbor not found
      ucRetVal = RCM_E65_UnknownNickname;
      break;
    }
    
    if(request->m_ucNeighborFlags & 0xFE)	// at least one of bits 7 to 1 is set
    {
      ucRetVal =  RCM_E66_InvalidNeighborProperty; // what about future standard extension?
      break;
    }
    g_aDllNeighbors[ucNeighIdx].m_bTimeSource = request->m_ucNeighborFlags & 0x01;    // bit 0 (TimeSource flag)
    
#else
    
    if( (ucNeighIdx == DLL_INVALID_NEIGH_IDX) && (request->m_ucNeighborFlags <= 1) ) 
    { // ONLY for 0x00 and 0x01, see below
      ucRetVal = RCM_E65_UnknownNickname;
      break;
    }
    
    switch(request->m_ucNeighborFlags)
    {
    case 0x00:
      g_aDllNeighbors[ucNeighIdx].m_bTimeSource &= ~0x01;     // clear bit 0 (TimeSource flag)
      break;
    case 0x01:
      g_aDllNeighbors[ucNeighIdx].m_bTimeSource |= 0x01;      // set bit 0 (TimeSource flag)
      break;
    case 0x02:
      g_aDllNeighbors[ucNeighIdx].m_bLinked &= ~LinkOptionFlagCodesMask_Receive; // 0x02 
      g_aDllNeighbors[ucNeighIdx].m_bTimeSource &= ~0x80;     // clear bit 7 (ActiveNeighbor flag -Nivis custom, on TR only)  
      break;
    case 0x04:
      if( ucNeighIdx == DLL_INVALID_NEIGH_IDX )
      { // neighbor not found, add it
        if( DLL_AddShortNeighbor( request->m_unNeighborNickname ) )
        { // error: no more neighbors available
          ucRetVal = RCM_E72_NoMoreNeighborsAvailable;
          break;
        }
        ucNeighIdx = DLL_FindShortNeighbor( request->m_unNeighborNickname );
      }
      g_aDllNeighbors[ucNeighIdx].m_bLinked = LinkOptionFlagCodesMask_Receive; // 0x02  
      g_aDllNeighbors[ucNeighIdx].m_bTimeSource |= 0x80;      // set bit 7 (ActiveNeighbor flag -Nivis custom, on TR only)
      break;
      
    default:	// at least one of bits 7 to 3 is set
      ucRetVal =  RCM_E66_InvalidNeighborProperty; // what about future standard extension?
      break;
    }
    
#endif
    
  }while(0);  // do-while(0) - loop used only to allow break on error conditions
  
  MONITOR_EXIT();

  response->m_unNeighborNickname = request->m_unNeighborNickname;
  response->m_ucNeighborFlags = request->m_ucNeighborFlags;

  return   ucRetVal;

//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_UnknownNickname;
//  return RCM_E66_InvalidNeighborProperty;
} // End - Execute_C971_WriteNeighbourPropertyFlag()


/*********** CMD 972 *************************/

uint16_t Execute_C972_SuspendDevices(C972_SuspendDevices_Req* request,
                                    C972_SuspendDevices_Resp* response,
                                    SHORT_ADDR srcAddr,
                                    uint8_t control, uint8_t remainingBytes)
{
  #ifndef TEST_DISCONNECT
  if(NM_NICKNAME != srcAddr)
    return RCS_E16_AccessRestricted;
  #endif
  
  uint8_t aASN[5];
  DLL_GetASN(aASN);
  if(memcmp(request->m_tTimeToSuspend, aASN, 5) < 0 ) // what if ASN rolls over?
    return RCM_E65_InvalidSuspendTime; // suspend time is smaller than current ASN (in the past)
  if(memcmp(request->m_tTimeToResume, request->m_tTimeToSuspend, 5) < 0 )
    return RCM_E66_InvalidResumeTime; // resume time is smaller than suspend time
  memcpy(response, request, sizeof(C972_SuspendDevices_Resp));
  MONITOR_ENTER();
  memcpy(&g_sPendingCmdsData.m_sC972_Data, response, sizeof(C972_SuspendDevices_Resp));
  g_sPendingCmdsData.m_bC972 = 1;
  g_sPendingCmdsData.m_bWaitingToSuspend = 1;
  DLL_LM_Request(DLL_SERV_DISCONNECT, response);
  g_ucMarkDisconnectHandle = 1;
  MONITOR_EXIT();
  return RCS_N00_Success;
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_InvalidSuspendTime;
//  return RCM_E66_InvalidResumeTime;
} // End - Execute_C972_SuspendDevices()


/*********** CMD 973 *************************/

uint16_t Execute_C973_WriteService(C973_WriteService_Req* request,
                                  C973_WriteService_Resp* response,
                                  SHORT_ADDR srcAddr,
                                  uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  if(request->m_ucServiceID < 0x80) 
  { // serviceID range reserved for services requested by device
    // check if awaiting for a response to Cmd799 for the same serviceID
    for(uint8 ucIdx=0; ucIdx<REQNM_QUEUE_SIZE; ucIdx++)
    {
      if((CMDID_C799_RequestService == g_aReqNMQueue[ucIdx].m_unCmdId) &&
         (request->m_ucServiceID == g_aReqNMQueue[ucIdx].m_stC799Req.m_ucServiceId))
      {
        return RCM_E67_OpenTransactionPending;
      }
    }
    
    if(NET_INVALID_IDX == FindServiceIndex( request->m_ucServiceID, NL_SEARCH_BY_SERVICE_ID ))
      return RCM_E66_InvalidServiceID; // NM cannot create a new service having ID < 0x80
  }

  if(request->m_eApplicationDomain > ServiceApplicationDomain_BlockTransfer)
    return RCM_E68_InvalidApplicationDomain;

  // TODO: when reducing the existing bandwidth - "Capacity denied", "More status available"

  // bits 7 to 3 from RequestFlags are discarded in the parse/compose functions

  response->m_unPeerNickname = request->m_unPeerNickname;
  response->m_ucServiceID = request->m_ucServiceID;
  response->m_ucRequestFlags = request->m_ucRequestFlags;
  response->m_eApplicationDomain = request->m_eApplicationDomain;
  response->m_ucRouteID = request->m_ucRouteID;
  response->m_tPeriod = request->m_tPeriod;
  // response->m_ucRemainingServicesNo is set in NET_Request;

  return NET_Request( NL_WRITE_SERVICE, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_NoMoreEntriesAvailable;
//  return RCM_E66_InvalidServiceID;
//  return RCM_E67_OpenTransactionPending;
//  return RCM_E68_InvalidApplicationDomain;
//  return RCM_E69_UnknownCorrespNickname;
//  return RCM_E70_InvalidRouteID;
//  return RCM_E71_CorrespNicknameAndRouteCorrespMismatch;
} // End - Execute_C973_WriteService()


/*********** CMD 974 *************************/

uint16_t Execute_C974_WriteRoute(C974_WriteRoute_Req* request,
                                C974_WriteRoute_Resp* response,
                                SHORT_ADDR srcAddr,
                                uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  response->m_ucRouteID = request->m_ucRouteID;
  response->m_unGraphID = request->m_unGraphID;
  response->m_unPeerNickname = request->m_unPeerNickname;
  // response->m_ucRemainingRoutesNo is set by NET_Request

  return NET_Request( NL_ADD_ROUTE, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_NoMoreEntriesAvailable;
//  return RCM_E66_InvalidNickname;
//  return RCM_E67_InvalidGraphID;
} // End - Execute_C974_WriteRoute()


/*********** CMD 975 *************************/

uint16_t Execute_C975_DeleteRoute(C975_DeleteRoute_Req* request,
                                 C975_DeleteRoute_Resp* response,
                                 SHORT_ADDR srcAddr,
                                 uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  response->m_ucRouteID = request->m_ucRouteID;
  // response->m_ucRemainingRoutesNo is set by NET_Request

  return NET_Request( NL_DEL_ROUTE, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_InvalidRouteID;
} // End - Execute_C975_DeleteRoute()


/*********** CMD 976 *************************/

uint16_t Execute_C976_WriteSourceRoute(C976_WriteSourceRoute_Req* request,
                                      C976_WriteSourceRoute_Resp* response,
                                      SHORT_ADDR srcAddr,
                                      uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  if( request->m_ucHopsNo < 1 || request->m_ucHopsNo > 8 )
    return RCM_E68_InvalidNoOfHops;

  if(3 + 2 * request->m_ucHopsNo > remainingBytes)
    return RCS_E60_PayloadTooLong;

  response->m_ucRouteID = request->m_ucRouteID;
  response->m_ucHopsNo = request->m_ucHopsNo;
  memcpy(response->m_aNicknameHopEntries, request->m_aNicknameHopEntries, request->m_ucHopsNo * 2);

  return NET_Request( NL_ADD_SRC_ROUTE, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCM_W08_BroadcastAddrDeletedInResp;
//  return RCS_E16_AccessRestricted;
//  return RCM_E65_NoMoreEntriesAvailable;
//  return RCM_E66_InvalidRouteID;
//  return RCM_E67_InvalidNickname;
//  return RCM_E68_InvalidNoOfHops;  
} // End - Execute_C976_WriteSourceRoute()


/*********** CMD 977 *************************/

uint16_t Execute_C977_DeleteSourceRoute(C977_DeleteSourceRoute_Req* request,
                                       C977_DeleteSourceRoute_Resp* response,
                                       SHORT_ADDR srcAddr,
                                       uint8_t control, uint8_t remainingBytes)
{
  if(srcAddr != NM_NICKNAME)
  { // not network manager
    return RCS_E16_AccessRestricted;
  }

  response->m_ucRouteID = request->m_ucRouteID;

  return NET_Request( NL_DEL_SRC_ROUTE, response );
//TODO:
//  return RCS_N00_Success;
//  return RCS_E05_TooFewDataBytesReceived;
//  return RCS_E16_AccessRestricted;
//  return RCM_E66_InvalidRouteID;
} // End - Execute_C977_DeleteSourceRoute()


/*********************************************/
/** Device Specific Wireless-only Commands ***/


/*********** CMD 64512 *************************/

uint16_t Execute_C64512_ReadWirelessModuleRevision( C64512_ReadWirelessModuleRevision_Req* request,
                                         C64512_ReadWirelessModuleRevision_Resp* response,
                                         SHORT_ADDR srcAddr,
                                         uint8_t control,
                                         uint8_t remainingBytes )
{
  memcpy((uint8*)&response->expandedDeviceType, g_aJoinDevUID_LE + 3, 2); // the same as for device? (CommonTable1)
  response->manufacturerIDCode = g_stProv.unManufacturerIDCode; // the same as for device? (CommonTable8)
  response->deviceRevisionLevel = CMD64512_DEVICE_REVISION_LEVEL;
  response->softwareRevisionLevel = CMD64512_SOFTWARE_REVISION_LEVEL;
  response->hardwareRevisionLevel = g_stProv.ucHardwareRevisionLevel; // the same as for device?
  
  return RCS_N00_Success;
} // End - Execute_C64512_ReadWirelessModuleRevision()



