WirelessHart - Field-Device Communication Stack
=============================

For now, the source code is compatible with Freescale MC13224-based platforms, and in order to compile it, the following are needed:

- Freescale BeeKit
http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=BEEKIT_WIRELESS_CONNECTIVITY_TOOLKIT&fpsp=1&tab=Design_Tools_Tab

- Freescale libraries (v1.1.4):
    libs/LLC.a
    libs/MC1322x.a

- Freescale library interfaces:
    LibInterface/ADC_Interface.h
    LibInterface/ASM_Interface.h
    LibInterface/Crm.h
    LibInterface/CrmRegs.h
    LibInterface/GPIO_Interface.h
    LibInterface/I2C_Interface.h
    LibInterface/Init_Interface.h
    LibInterface/Interrupt.h
    LibInterface/ITC_Interface.h
    LibInterface/NVM.h
    LibInterface/Platform.h
    LibInterface/Platform_Private.h
    LibInterface/SPI_Interface.h
    LibInterface/Ssi_Interface.h
    LibInterface/Ssi_Regs.h
    LibInterface/Timer.h
    LibInterface/UartLowLevel.h
