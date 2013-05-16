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

#ifdef UT_ACTIVED  // unit testing support actived

#include "crm.h"
#include "system.h"
#include "typedef.h"

/*customize according to your UT !!!*/

#if( UT_ACTIVED == UT_SIZEOF_ONLY )
  #include "UT_defines.h"
#else
#error " UT_ACTIVED must be UT_SIZEOF_ONLY (=21)"
#endif


void UTSPECIFIC_Test1()
{
  //TBD
  CK(1);
}

TEST_FCT g_UTSPECIFIC_Test[] = {
    UTSPECIFIC_Test1
};


int main()
{  
    GlobalDisableIRQ();
  
    WDT_INIT();  

    Digitals_Init();
  
    
  {  
    uint8 ucIdx;
    for(ucIdx = 0; ucIdx < sizeof(g_UTSPECIFIC_Test)/sizeof(*g_UTSPECIFIC_Test); ucIdx++ )
    {        
        FEED_WDT();
        g_UTSPECIFIC_Test[ucIdx]();
    }
    CK(1);
  }
}

#endif // UT_ACTIVED  // unit testing support end

