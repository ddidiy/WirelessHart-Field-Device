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

#ifndef COMMONTABLES_H_
#define COMMONTABLES_H_

#include "../../util/WirelessStackTypes.h"
#include "CommonResponseCode.h"

typedef struct
{
#ifdef WIRELESS_HART_DEVICE
  uint8_t unusedByte;
#endif
  
} EmptyCommand_Req;

typedef struct
{
#ifdef WIRELESS_HART_DEVICE
  uint8_t unusedByte;
#endif
  
} EmptyCommand_Resp;


typedef struct
{
  uint16_t cmdId;
} ComposerContext;

//Table 1. Expanded Device Type Codes
//TODO:[Ovidiu] - complete this enum
typedef enum
{
  ExpandedDeviceTypeCode_3DLevelScannerM_AMP = 0xE085,
  ExpandedDeviceTypeCode_3DLevelScannerS_AMP = 0xE086,
  ExpandedDeviceTypeCode_3DLevelScannerL_AMP = 0xE087,
  ExpandedDeviceTypeCode_R3x_BARTEC = 0xE08A,
  ExpandedDeviceTypeCode_SS_DaehanControl = 0xE084,
  ExpandedDeviceTypeCode_700Bridge_Detcon = 0xE08B,
  ExpandedDeviceTypeCode_ESD3000_ExSaf = 0xE080,
  ExpandedDeviceTypeCode_TORRIX_FAFNIR = 0xC680,
  ExpandedDeviceTypeCode_Vortex84_Foxboro = 0x141E,
  ExpandedDeviceTypeCode_FL4000_GeneralMonitorsInc = 0xDF80,
  ExpandedDeviceTypeCode_BECKMK2_HaroldBeckandSons = 0x68EF,
  ExpandedDeviceTypeCode_EP1000SPS200_Koso = 0xCF80,
  ExpandedDeviceTypeCode_ESK2A_Krohne = 0x45E2,
  ExpandedDeviceTypeCode_Optiflex1300C_Krohne = 0x45E4,
  ExpandedDeviceTypeCode_Optiwave7300C_Krohne = 0x45E5,
  ExpandedDeviceTypeCode_CMM01MASSFLOW_METROVAL = 0xE08D,
  ExpandedDeviceTypeCode_UltimaXLXT_MSA = 0xE08C,
  ExpandedDeviceTypeCode_EX200HPositioner_ORANGEINSTUK = 0xE088,
  ExpandedDeviceTypeCode_IS200HPositioner_ORANGEINSTUK = 0xE089,
  ExpandedDeviceTypeCode_Series642_PyromationInc = 0xB580,
  ExpandedDeviceTypeCode_SDT420_SEOJININSTECH = 0xE082,
  ExpandedDeviceTypeCode_HVP1114_SIC = 0xD485,
  ExpandedDeviceTypeCode_PES_SIC = 0xD480,
  ExpandedDeviceTypeCode_LD400_Smar = 0x3E0A,
  ExpandedDeviceTypeCode_IP8101_SMC = 0x837E,
  ExpandedDeviceTypeCode_RTEx15_TASIFLOW = 0xE083,
  ExpandedDeviceTypeCode_MACXMCRSMUX16ch_PhoenixContact = 0xB003,
  ExpandedDeviceTypeCode_MACXMCRSMUX32ch_PhoenixContact = 0xB004,
  ExpandedDeviceTypeCode_2600T364_ABB = 0x1A05,
  ExpandedDeviceTypeCode_SITRANSFCMASSFLO_Siemens = 0x2A19,
  //HPort
  ExpandedDeviceTypeCode_3300_Mobrey = 0x3B21,
  ExpandedDeviceTypeCode_KFD2_PepperlFuchs = 0x5D01,
  ExpandedDeviceTypeCode_KFD2HMM16_PepperlFuchs = 0x5DEF,
  ExpandedDeviceTypeCode_SVIIIAP_MasoneilanDresser = 0x65CA,
  ExpandedDeviceTypeCode_DeltapilotS_EndressHauser = 0x111A,
  ExpandedDeviceTypeCode_FMU9x_EndressHauser = 0x111B,
  ExpandedDeviceTypeCode_MAGNEW4W_Yamatake = 0x360C,
  ExpandedDeviceTypeCode_APT3100_DuonSystem = 0x866B,
  ExpandedDeviceTypeCode_P_FLOW92_EndressHauser = 0x1161,
  ExpandedDeviceTypeCode_P_FLOW91_EndressHauser = 0x1162,
  ExpandedDeviceTypeCode_DVC6000_FisherControls = 0x1303,
  ExpandedDeviceTypeCode_STT17H_Honeywell = 0x1707,
  ExpandedDeviceTypeCode_THZ2_MooreIndustries = 0x2004,
  ExpandedDeviceTypeCode_HPSM_ElconInstruments = 0x4901,
  ExpandedDeviceTypeCode_APT3200_DuonSystem = 0x8661,
  ExpandedDeviceTypeCode_MSP2002_SiemensMilltronicsPI = 0x54F8,
  ExpandedDeviceTypeCode_MST9500_SiemensMilltronicsPI = 0x54F9,
  ExpandedDeviceTypeCode_ND820T_MetsoAutomation = 0x57EE,
  ExpandedDeviceTypeCode_2211pH_Knick = 0x61EB
} ExpandedDeviceTypeCodes;

//Table 3. Transfer Function Codes
typedef enum
{
  TransferFunctionCodes_Linear = 0,
  TransferFunctionCodes_SquareRoot = 1,
  TransferFunctionCodes_SquareRootThirdPower = 2,
  TransferFunctionCodes_SquareRootFifthPower = 3,
  TransferFunctionCodes_SpecialCurve = 4,
  TransferFunctionCodes_Square = 5,
  TransferFunctionCodes_Discrete = 230,
  TransferFunctionCodes_SquareRootPlusSpecialCurve = 231,
  TransferFunctionCodes_SquareRootThirdPowerPlusSpecialCurve = 232,
  TransferFunctionCodes_SquareRootFifthPowerPlusSpecialCurve = 233,
  //240-249	Enumeration May Be Used For Manufacturer Specific Definitions
  TransferFunctionCodes_None = 251,
  TransferFunctionCodes_Unknown = 252,
  TransferFunctionCodes_Special = 253
} TransferFunctionCodes;

//Table 2. Engineering Unit Codes
typedef enum
{
  UnitsCodes_DegreesCelsius = 32,
  UnitsCodes_DegreesFahrenheit = 33,
  UnitsCodes_DegreesRankine = 34,
  UnitsCodes_Kelvin = 35,
  //UnitsCodes_InchesOfWaterAt68DegreesF = 0x01,
  UnitsCodes_Minutes = 50,  
  UnitsCodes_Seconds = 51,
  UnitsCodes_Hours = 52,
  UnitsCodes_Days = 53,
  UnitsCodes_Milliamperes = 39,
  UnitsCodes_Percent = 57,
  UnitsCodes_Millivolts = 36,
  UnitsCodes_Volts = 58,
  
  // TBD !!!
  
  UnitsCodes_NotUsed = 250,
} UnitsCodes;

//Table 6. Alarm Selection Codes
typedef enum
{
  AlarmSelectionCodes_High = 0,
  AlarmSelectionCodes_Low = 1,
  AlarmSelectionCodes_HoldLastOutputValue = 239,
  //240-249 Enumeration May Be Used For Manufacturer Specific Definitions
  AlarmSelectionCodes_None = 251,
  AlarmSelectionCodes_Unknown = 252,
  AlarmSelectionCodes_Special = 253
} AlarmSelectionCodes;

//Table 7. Write Protect Codes
typedef enum
{
  WriteProtectCodes_NO = 0,
  WriteProtectCodes_Yes = 1,
  WriteProtectCodes_None = 251,   // Not Implemented
  WriteProtectCodes_Unknown = 252,
  WriteProtectCodes_Special = 253
} WriteProtectCodes;

//Table 8. Manufacturer Identification Codes
typedef enum
{
  ManufacturerIdentificationCodes_ACROMAG = 0x0001,
  ManufacturerIdentificationCodes_ALLEN_BRADLEY = 0x0002,
  ManufacturerIdentificationCodes_AMETEK = 0x0003,
  ManufacturerIdentificationCodes_ANALOG_DEVICES = 0x0004
    //CHECKME [george.petrehus] too many - spec183r19.0
} ManufacturerIdentificationCodes;

//Table 9. Burst Mode Control Codes
typedef enum
{
  BurstModeControlCodes_Off = 0,
  BurstModeControlCodes_EnableBurstOnTokenPassingDataLinkLayer = 1,
  BurstModeControlCodes_EnableBurstOnTDMADataLinkLayer = 2,
  BurstModeControlCodes_EnableBurstOnTDMAandTokenPassingDataLinkLayers = 3
} BurstModeControlCodes;

//Table 10. Physical Signaling Codes
typedef enum
{
  PhysicalSignalingCodes_BELL202CURRENT = 0,
  PhysicalSignalingCodes_BELL202VOLTAGE = 1,
  PhysicalSignalingCodes_RS485 = 2,
  PhysicalSignalingCodes_RS232 = 3,
  PhysicalSignalingCodes_WIRELESS = 4,
  PhysicalSignalingCodes_SPECIAL = 6
} PhysicalSignalingCodes;

//Table 11. Flag Assignments
typedef enum
{
  FlagAssignmentsMask_MultiSensorFieldDevice = 0x01,
  FlagAssignmentsMask_EEPromControl = 0x02,
  FlagAssignmentsMask_ProtocolBridge = 0x04,
  FlagAssignmentsMask_OQPSKModulation = 0x08,
  FlagAssignmentsMask_C8pskCapable = 0x40,
  FlagAssignmentsMask_C8pskInMultiDropOnly = 0x80
} FlagAssignmentsMasks;

//Table 15. Analog Channel Number Codes
typedef enum
{
  AnalogChannelNumberCodes_AnalogChannel0 = 0,
  AnalogChannelNumberCodes_AnalogChannel1 = 1,
  AnalogChannelNumberCodes_AnalogChannel2 = 2,
  AnalogChannelNumberCodes_AnalogChannel3 = 3,
  AnalogChannelNumberCodes_AnalogChannel4 = 4
} AnalogChannelNumberCodes;

//Table 16. Loop Current Mode Codes
typedef enum
{
  LoopCurrentModeCodes_Disabled = 0, LoopCurrentModeCodes_Enabled = 1
} LoopCurrentModeCodes;

//Table 17. Extended Device Status Codes
typedef enum
{
  ExtendedDeviceStatusCodesMask_MaintenanceRequired = 0x01,
  ExtendedDeviceStatusCodesMask_DeviceVariableAlert = 0x02,
  ExtendedDeviceStatusCodesMask_CriticalPowerFailure = 0x04
} ExtendedDeviceStatusCodesMasks;

//Table 18. Lock Device Codes
typedef enum
{
  LockDeviceCodes_Unlocked = 0, LockDeviceCodes_LockTemporary = 1, //Device Reset or Power Loss releases the Lock
  LockDeviceCodes_LockPermanent = 2, //Device Reset or Power Loss does not affect the Lock
  LockDeviceCodes_LockAll = 3
    //No changes in the device's configuration, by any master, are allowed.
} LockDeviceCodes;

//Table 19. Write Device Variable Codes
typedef enum
{
  WriteDeviceVariableCodes_Normal = 0, WriteDeviceVariableCodes_FixedValue = 1
} WriteDeviceVariableCodes;

//Table 20. Device Variable Family Codes
typedef enum
{
  DeviceVariableFamilyCodes_Temperature = 4,
  DeviceVariableFamilyCodes_Pressure = 5,
  DeviceVariableFamilyCodes_ValveActuator = 6,
  DeviceVariableFamilyCodes_SimplePIDControl = 7,
  DeviceVariableFamilyCodes_pH = 8,
  DeviceVariableFamilyCodes_Conductivity = 9,
  DeviceVariableFamilyCodes_Totalizer = 10,
  DeviceVariableFamilyCodes_Level = 11,
  DeviceVariableFamilyCodes_VortexFlow = 12,
  DeviceVariableFamilyCodes_MagFlow = 13,
  DeviceVariableFamilyCodes_CoriolisFlow = 14,
  DeviceVariableFamilyCodes_NotUsed = 250
} DeviceVariableFamilyCodes;

//Table 21. Device Variable Classification Codes
typedef enum
{
  DeviceVariableClassificationCodes_DeviceVariableNotClassified = 0,  
  
  DeviceVariableClassificationCodes_Temperature = 64,
  DeviceVariableClassificationCodes_Pressure = 65,
  DeviceVariableClassificationCodes_VolumetricFlow = 66,
  DeviceVariableClassificationCodes_Velocity = 67,
  DeviceVariableClassificationCodes_Volume = 68,
  DeviceVariableClassificationCodes_Length = 69,
  DeviceVariableClassificationCodes_Time = 70,
  DeviceVariableClassificationCodes_Mass = 71,
  DeviceVariableClassificationCodes_MassFlow = 72,
  DeviceVariableClassificationCodes_MassPerVolume = 73,
  DeviceVariableClassificationCodes_Viscosity = 73,
  DeviceVariableClassificationCodes_AngularVelocity = 75,
  DeviceVariableClassificationCodes_Area = 76,
  DeviceVariableClassificationCodes_Energy = 77,
  DeviceVariableClassificationCodes_Force = 78,
  DeviceVariableClassificationCodes_Power = 79,
  DeviceVariableClassificationCodes_Frequency = 80,
  DeviceVariableClassificationCodes_Analytical = 81,
  DeviceVariableClassificationCodes_Capacitance = 82,
  DeviceVariableClassificationCodes_Emf = 83,
  DeviceVariableClassificationCodes_Current = 84,
  DeviceVariableClassificationCodes_Resistance = 85,
  DeviceVariableClassificationCodes_Angle = 86,
  DeviceVariableClassificationCodes_Conductance = 87,
  DeviceVariableClassificationCodes_VolumePerVolume = 88,
  DeviceVariableClassificationCodes_VolumePerMass = 89,
  DeviceVariableClassificationCodes_Concentration = 90,
  DeviceVariableClassificationCodes_ValveActuator = 91,
  DeviceVariableClassificationCodes_Level = 92,
  DeviceVariableClassificationCodes_VortexFlow = 93,
  DeviceVariableClassificationCodes_MagFlow = 94,
  DeviceVariableClassificationCodes_CoriolisFlow = 95,
 
  DeviceVariableClassificationCodes_NotUsed = 250 //spec 127r7.1 page 24
} DeviceVariableClassificationCodes;

//Table 22. Trim Point Codes
typedef enum
{
  TrimPointCodes_Reserved = 0,
  TrimPointCodes_LowerTrimPointSupported = 1,
  TrimPointCodes_UpperTrimPointSupported = 2,
  TrimPointCodes_LowerAndUpperTrimPointSupported = 3
} TrimPointCodes;

//Table 23. Capture Mode Codes
typedef enum
{
  CaptureModeCodes_CaptureModeCodes_Disabled = 0,
  CaptureModeCodes_Enabled_CatchDataFromFieldDevice = 1,
  CaptureModeCodes_Enabled_CatchDataFromBACKMessage = 2
} CaptureModeCodes;

//Table 25. Lock Device Status
typedef enum
{
  LockDeviceStatusMask_DeviceLocked = 0x01,
  LockDeviceStatusMask_LockIsPermanent = 0x02,
  LockDeviceStatusMask_LockedByPrimaryMaster = 0x04,
  LockDeviceStatusMask_ConfigurationCannotBeChangedByAnyApplication = 0x08,
  LockDeviceStatusMask_LockedbyGateway = 0x10
} LockDeviceStatusMasks;

//Table 26. Analog Channel Flags
typedef enum
{
  AnalogChannelFlagsMask_FieldDevice = 0x01
} AnalogChannelFlagsMasks;

//Table 27. Analog Channel Saturated Codes
typedef enum
{
  AnalogChannelSaturatedCodesMask_AnalogChannel1 = 0x01,
  AnalogChannelSaturatedCodesMask_AnalogChannel2 = 0x02,
  AnalogChannelSaturatedCodesMask_AnalogChannel3 = 0x04,
  AnalogChannelSaturatedCodesMask_AnalogChannel4 = 0x08
} AnalogChannelSaturatedCodesMasks;

//Table 28. Analog Channel Fixed Codes
typedef enum
{
  AnalogChannelFixedCodesMask_AnalogChannel1 = 0x01,
  AnalogChannelFixedCodesMask_AnalogChannel2 = 0x02,
  AnalogChannelFixedCodesMask_AnalogChannel3 = 0x04,
  AnalogChannelFixedCodesMask_AnalogChannel4 = 0x08
} AnalogChannelFixedCodesMasks;

//Table 29. Standardized Status 0
typedef enum
{
  StandardizedStatus0Mask_SimulationActive = 0x01,
  StandardizedStatus0Mask_NonVolatileMemoryDefect = 0x02,
  StandardizedStatus0Mask_VolatileMemoryDefect = 0x04,
  StandardizedStatus0Mask_WatchdogResetExecuted = 0x08,
  StandardizedStatus0Mask_VoltageConditionsOutOfRange = 0x10,
  StandardizedStatus0Mask_EnvironmentalConditionsOutOfRange = 0x20,
  StandardizedStatus0Mask_ElectronicDefect = 0x40
} StandardizedStatus0Masks;

//Table 31. Standardized Status 2
typedef enum
{
  StandardizedStatus2Mask_SubDeviceListChanged = 0x01,
  StandardizedStatus2Mask_DuplicateMasterDetected = 0x02
} StandardizedStatus2Masks;

//Table 32. Standardized Status 3
typedef enum
{
  StandardizedStatus3Mask_CapacityDenied = 0x01,
  StandardizedStatus3Mask_DuplicateMasterDetected = 0x02,
  StandardizedStatus3Mask_BandwidthAllocationPending = 0x04,
  StandardizedStatus3Mask_BlockTransferPending = 0x08
} StandardizedStatus3Masks;

//Table 33. Burst Message Trigger Mode
typedef enum
{
  BurstMessageTriggerMode_Continuous = 0,
  BurstMessageTriggerMode_Window = 1,
  BurstMessageTriggerMode_Rising = 2,
  BurstMessageTriggerMode_Falling = 3,
  BurstMessageTriggerMode_OnChange = 4,
} BurstMessageTriggerMode;

//Table 34. Device Variable Code
typedef enum
{
  DeviceVariableCodes_BatteryLife = 243, // (Float in Days)
  DeviceVariableCodes_PercentRange = 244,
  DeviceVariableCodes_LoopCurrent = 245,
  DeviceVariableCodes_PrimaryVariable = 246,
  DeviceVariableCodes_SecondaryVariable = 247,
  DeviceVariableCodes_TertiaryVariable = 248,
  DeviceVariableCodes_QuaternaryVariable = 249,
  
  DeviceVariableCodes_NotUsed = 250,  
} DeviceVariableCodes;

//Table 35. Event Notification Control Code
typedef enum
{
  EventNotificationControlCodes_Off = 0,
  EventNotificationControlCodes_EnableEventNotificationOnTokenPassingDataLinkLayer = 1,
  EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer = 2,
  EventNotificationControlCodes_EnableEventNotificationOnTDMATokenPassingDataLinkLayers = 3
} EventNotificationControlCodes;

//Table 36. Event Status
typedef enum
{
  EventStatusCodesMask_ConfigurationChangedEventPending = 0x01,
  EventStatusCodesMask_DeviceStatusEventPending = 0x02,
  EventStatusCodesMask_MoreStatusAvailableEventPending = 0x04
} EventStatusCodesMasks;

//Table 37. Trend Control Codes
typedef enum
{
  TrendControlCodes_Disable = 0,
  TrendControlCodes_EnableSingleDataPointTrending = 1,
  TrendControlCodes_EnableFilteredTrending = 2,
  TrendControlCodes_EnableAverageTrending = 3
} TrendControlCodes;

//Table 38. Time-set Code
typedef enum
{
  TimeSetCode_ReadReceiveTime = 0, TimeSetCode_WriteDateTime = 1
} TimeSetCodes;

//Table 39. Service Request Flags
typedef enum
{
  ServiceRequestFlagsMask_Source = 0x01,
  ServiceRequestFlagsMask_Sink = 0x02,
  ServiceRequestFlagsMask_Intermittent = 0x04
} ServiceRequestFlagsMasks;

//Table 40. Service Application Domain
typedef enum
{
  ServiceApplicationDomain_Publish = 0,
  ServiceApplicationDomain_Event = 1,
  ServiceApplicationDomain_Maintenance = 2,
  ServiceApplicationDomain_BlockTransfer = 3
} ServiceApplicationDomain;

//Table 41. Synchronous Action Control
typedef enum
{
  SynchronousActionControlMask_Command = 0x01,
  SynchronousActionControlMask_OneShot = 0x10,
  SynchronousActionControlMasks_ActionEnabled = 0x80
} SynchronousActionControlMasks;

//Table 42. Real-Time Clock Flags
typedef enum
{
  RealTimeClockFlagsMask_NonVolatileClock =0x01,
  RealTimeClockFlagsMask_ClockUninitialized =0x02
} RealTimeClockFlagsMasks;

//Table 43. Wireless Timer Code
typedef enum
{
  WirelessTimerCode_Discovery = 0,
  WirelessTimerCode_Advertisment = 1,
  WirelessTimerCode_Keep_Alive = 2,
  WirelessTimerCode_PathFailure = 3,
  WirelessTimerCode_HealthReport = 4,
  WirelessTimerCode_BroadcastReply = 5,
  WirelessTimerCode_MaximumPDUAge = 6,
  WirelessTimerCode_NoOfEntries = 7
} WirelessTimerCode;

//Table 44. Device Power Source
typedef enum
{
  DevicePowerSource_LinePower = 0,
  DevicePowerSource_BatteryPower = 1,
  DevicePowerSource_RechargeableBatteryPowerOrPowerScavenging = 2
} DevicePowerSource;

//Table 45. Link Type
typedef enum
{
  LinkType_Normal = 0,
  LinkType_Discovery = 1,
  LinkType_Broadcast = 2,
  LinkType_Join = 3
} LinkType;

//Table 46. Link Option Flag Codes
typedef enum
{
  LinkOptionFlagCodesMask_Transmit =0x01,
  LinkOptionFlagCodesMask_Receive =0x02,
  LinkOptionFlagCodesMask_Shared =0x04
} LinkOptionFlagCodesMasks;

//Table 47. Superframe Mode Flags
typedef enum
{
  SuperframeModeFlagsMask_Active =0x01,
  SuperframeModeFlagsMask_HandheldSuperframe =0x80
} SuperframeModeFlagsMasks;

//Table 48. Session Type Codes
typedef enum
{
  SessionTypeCode_Unicast = 0,
  SessionTypeCode_Broadcast = 1,
  SessionTypeCode_Join = 2
} SessionTypeCodes;

//Table 49. Service Deletion Reason Codes
typedef enum
{
  ServiceDeletionReasonCodes_RequestedByPeer = 0,
  ServiceDeletionReasonCodes_ServiceCantBeEstablished = 1,
  ServiceDeletionReasonCodes_NetworkFailure = 2
} ServiceDeletionReasonCodes;

//Table 50. Disconnect Cause Codes
typedef enum
{
  DisconnectCauseCodes_User_Initiated = 0,
  DisconnectCauseCodes_Communication_Failure = 1
} DisconnectCauseCodes;

//Table 51. Wireless Operation Mode
typedef enum
{
  WirelessOperationMode_Idle = 0,
  WirelessOperationMode_ActiveSearch =1,
  WirelessOperationMode_Negotiating =2,
  WirelessOperationMode_Quarantined =3,
  WirelessOperationMode_Operational =4,
  WirelessOperationMode_Suspended =5,
  WirelessOperationMode_DeepSleep_UltraLowPower_PassiveSearch =6,
  // 7-15: reserved
} WirelessOperationMode;

//Table 52. Join Process Status
typedef enum
{
  JoinProcessStatusMask_NetworkPacketsHeard = 0x0001,
  JoinProcessStatusMask_ASNAcquired =0x0002,
  JoinProcessStatusMask_SynchronizedToSlotTime =0x0004,
  JoinProcessStatusMask_AdvertisementHeard =0x0008,
  JoinProcessStatusMask_JoinRequested =0x0010,
  JoinProcessStatusMask_JoinRetrying =0x0020,
  JoinProcessStatusMask_JoinFailed =0x0040,
  JoinProcessStatusMask_Authenticated =0x0080,
  JoinProcessStatusMask_NetworkJoined =0x0100,
  JoinProcessStatusMask_NegotiatingNetworkProperties =0x0200,
  JoinProcessStatusMask_NormalOperationCommencing =0x0400
} JoinProcessStatusMasks;

//Table 53. Security Type Codes
typedef enum
{
  SecurityTypeCodes_SessionKeyed = 0, 
  SecurityTypeCodes_JoinKeyed = 1,
  SecurityTypeCodes_HandheldKeyed = 2
} SecurityTypeCodes;

//Table 54. SI Units Control Code
typedef enum
{
  SIUnitsControlCodes_NoRestrictions = 0, SIUnitsControlCodes_UnitCodesLimitedToTheSIUnitsOnly = 1
} SIUnitsControlCodes;

//Table 55. Device List Code
typedef enum
{
  DeviceListCode_ActiveDeviceList = 0,
  DeviceListCode_WhitelistedDevices = 1,
  DeviceListCode_BlacklistedDevices = 2
} DeviceListCode;

//Table 56. Network Access Mode Code
typedef enum
{
  NetworkAccessModeCode_Open = 0,
  NetworkAccessModeCode_UseWhitelist = 1,
  NetworkAccessModeCode_UseBlacklist = 2,
  NetworkAccessModeCode_UseWhitelistAndBlacklist = 3,
  NetworkAccessModeCode_Lockdown = 4
} NetworkAccessModeCode;

//Table 57. Device Profile Code
typedef enum
{
  DeviceProfileCodes_HART_PROCESS_AUTOMATION_DEVICE = 1,
  DeviceProfileCodes_HART_DISCRETE_DEVICE = 2,
  DeviceProfileCodes_HYBRID_PROCAUTOMATION_AND_DISCRETE = 3,
  DeviceProfileCodes_IO_SYSTEM = 4,
  DeviceProfileCodes_WIRELESSHART_PROCESS_AUTOMATION_DEVICE = 129,
  DeviceProfileCodes_WIRELESSHART_DISCRETE_DEVICE = 130,
  DeviceProfileCodes_WIRELESSHART_HYBRID_PROCAUTOMATION_AND_DISCRETE = 131,
  DeviceProfileCodes_WIRELESSHART_GATEWAY = 132,
  DeviceProfileCodes_WIRELESSHART_PROCESS_ADAPTER = 141,
  DeviceProfileCodes_WIRELESSHART_DISCRETE_ADAPTER = 142,
  DeviceProfileCodes_WIRELESSHART_HANDHELD_MAINTENANCE_TOOL = 144
//CHECKME [george.petrehus] too many - spec183r19.0
} DeviceProfileCodes;

//Table 58. Device Power Status
typedef enum
{
  DevicePowerStatus_Nominal = 0,
  DevicePowerStatus_Low = 1,
  DevicePowerStatus_CriticallyLow = 2,
  DevicePowerStatus_RechargingLow = 3,
  DevicePowerStatus_RechargingHigh = 4
} DevicePowerStatus;

//Table 59. NeighbourFlags
typedef enum
{
  NeighbourFlagsMask_TimeSource = 0x01
} NeighbourFlagsMasks;

//Table 60. Notification Mask Codes
typedef enum
{
  NotificationMaskCodesMask_BurstMode = 0x0001,
  NotificationMaskCodesMask_EventNotification = 0x0002,
  NotificationMaskCodesMask_DeviceStatus = 0x0004,
  NotificationMaskCodesMask_DeviceConfiguration = 0x0008,
  NotificationMaskCodesMask_NetworkTopology = 0x0010,
  NotificationMaskCodesMask_NetworkSchedule = 0x0020
} NotificationMaskCodesMasks;

//Table 61. Join Mode Code
typedef enum
{
  JoinModeCode_DontAttemptToJoin = 0,
  JoinModeCode_JoinNow = 1,
  JoinModeCode_AttemptToJoinImmediatelyOnPowerupOrReset = 2,
} JoinModeCode;

//Table 62. Device Scheduling Flags
typedef enum
{
  DeviceSchedulingFlagsMask_TransientDevice = 0x01,
  DeviceSchedulingFlagsMask_NonRoutingDevice =0x02,
  DeviceSchedulingFlagsMask_HandheldDevice =0x04
} DeviceSchedulingFlagsMasks;

//Table 63. Network Optimization Flags
typedef enum
{
  NetworkOptimizationFlagsMask_LowLatency =0x01,
  NetworkOptimizationFlagsMask_UseLinePoweredDevicesAsBackbone = 0x02
} NetworkOptimizationFlagsMasks;

typedef enum
{
  PrivateLabelDistributorCode_RESERVED = 250, PrivateLabelDistributorCode_RESERVED1 = 251
    //CHECKME [george.petrehus] -spec183r19.0 - Table 8
} PrivateLabelDistributorCodes;

typedef struct
{
  uint8_t deviceVariableCode;
  DeviceVariableClassificationCodes deviceVariableClassificationCode;
  UnitsCodes unitCode;
  uint8_t deviceVariableStatus; //TODO [george.petrehus] not found in documentation
  float deviceVariableValue;
} Slot;

typedef struct
{
  uint8_t unitsCode;    // UnitsCodes
  float variable;
} Variable;

typedef enum
{
  CommunicationStatusMask_CommunicationError = 0x80, // This bit must always be set to indicate a communication error
  CommunicationStatusMask_VerticalParityError = 0x40,
  CommunicationStatusMask_OverrunError = 0x20,
  CommunicationStatusMask_FramingError = 0x10,
  CommunicationStatusMask_LongitudinalParityError = 0x08,
  CommunicationStatusMask_BufferOverflow = 0x02
} CommunicationStatusMasks;

typedef enum
{
  FieldDeviceStatusesMask_DeviceMalfunction = 0x80,
  FieldDeviceStatusesMask_ConfigurationChanged = 0x40,
  FieldDeviceStatusesMask_ColdStart = 0x20,
  FieldDeviceStatusesMask_MoreStatusAvailable = 0x10,
  FieldDeviceStatusesMask_LoopCurrentFixed = 0x08,
  FieldDeviceStatusesMask_LoopCurrentSaturated = 0x04,
  FieldDeviceStatusesMask_NonPrimaryVariableOutOfLimits = 0x02,
  FieldDeviceStatusesMask_PrimaryVariableOutOfLimits = 0x01
} FieldDeviceStatusesMasks;

#endif /* COMMONTABLES_H_ */
