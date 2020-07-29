/*-----------------------------------------------------------------------------
 * TITLE: CORE_INIT.C - initialization functions used during a reset.
 *
 * Main processing loop (interrupt driven) for the keyboard controller/scanner.
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

//-----------------------------------------------------------------------------
// Reset_Initialization - Initialize the registers and data variables.
//
//-----------------------------------------------------------------------------
void Reset_Initialization(void)
{
    //Init_ClearRam();      // Dino move to startup.a51
    Init_GPIO();
    
    #ifdef HSPI
	GPIO_HSPI_INIT();
    LoadSPIFucnToRam(HSPI_RamCode);	// Load function to ram
    #endif
    
    #ifdef PECI_Support
    Init_PECI();
    #endif
    
    Init_Regs();
    Init_Timers();
    Init_Kbd();
    Cold_Reset();
    Common_Reset();
    Init_PS2();
    Init_Cache();
    OEM_ColdResetInit();

    #ifdef ITE_EVBoard
    CLEAR_BIT(KBHISR,2);
    SET_BIT(GPIOD,3); // = 1;    //PD.3 SCI
    SET_BIT(GPIOB,5); // = 1;    //PB.5 A20
    SET_BIT(GPIOB,6); // = 1;    //PB.6 KB_RST
   // InitShareMemory();
    InitSio();

    GPIO_F0_CTRL = 0x00;
    GPIO_F1_CTRL = 0x00;
    GPIO_F2_CTRL = 0x00;
    GPIO_F3_CTRL = 0x00;
    GPIO_F4_CTRL = 0x00;
    GPIO_F5_CTRL = 0x00;

    //Oem_SysOnInit();

    #endif
    //- - - - - - - - - - - - - - - - -
}

//-----------------------------------------------------------------------------
// The function of enabling interrupts
//
//-----------------------------------------------------------------------------
void vInitEnableInterrupt(void)
{
    EX1 = 1;                // enable external 1 interrupt
    EnableAllInterrupt();   // enable global interrupt
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// The function of init. registers
//
//-----------------------------------------------------------------------------
const struct REG_INIT_DEF code reg_init_table[] =
{
    // HOST interface
    { &SPCTRL1,     _Init_I2EC  },  // Enable I2EC R/W
    { &BADRSEL,     _Init_BADR  },
    { &RSTS,        0x84        },

    // KBC and PM interface
    { &KBIRQR,      0x00        },  // Disable KBC IRQ
    { &KBHICR,      BIT3+BIT6   },  // KBC port control IBFCIE+PMICIE
    { &PM1CTL,      BIT0+BIT6   },  // EC port control  IBFCIE+SCI low active
    { &PM2CTL,      BIT0+BIT6   },  // EC port2 control IBFCIE+SCI low active

    // PS2
    { &PSCTL1,      PS2_InhibitMode     },  // Disable Debounce , Inhibit CLK
    { &PSCTL2,      PS2_InhibitMode     },  // Disable Debounce , Inhibit CLK
    { &PSCTL3,      PS2_InhibitMode     },  // Disable Debounce , Inhibit CLK
    { &PSINT1,      0x06        },  // Transation Done Interrupt
    { &PSINT2,      0x06        },  // Transation Done Interrupt
    { &PSINT3,      0x06        },  // Transation Done Interrupt

    // Keyboard Matrix Scan
    { &KSOCTRL,     BIT0        },  //
    { &KSICTRL,     BIT2        },  //

    // ADC Initial&Calibration
    { &ADCSTS,      BIT3        },   // Enable Analog accuracy initialization
    { &ADCSTS,      0           },   // Stop adc accuracy initialization
    { &ADCCFG,      0           },   //
    { &KDCTL,       BIT7        },   // Enable Hardware Callibration
    { &ADCCFG,      BIT0 + BIT5 },   // Enable Digital Filter enable

    // DAC
    { &DACCTRL,     _Init_DAC   },  //
    { &DACPDREG,    0x00        },  //

};
//-----------------------------------------------------------------------------
void Init_Regs(void)
{
    BYTE index = 0x00;

    while( index < (sizeof(reg_init_table)/3) )
    {
        Tmp_XPntr  = reg_init_table[index].address;
        *Tmp_XPntr = reg_init_table[index].initdata;
        index ++;
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: cold_reset - Initialization when power goes on
//
//-----------------------------------------------------------------------------
void Cold_Reset(void)
{
    //-------------------------------------------------------------------------
    Ext_Cb0 = 0xFF;
    Ccb42   = 0x00;
    //-------------------------------------------------------------------------
    Ext_Cb2.byte = EXT_CB2_INIT;    /* Update Ext_Cb3 a little further down */
    Led_Data = 0x00;
    Gen_Info = 0;       /* Clear general control. */
    Pass_On  = 0;       /* Password enable send code. */
    Pass_Off = 0;       /* Password disable send code. */
    Pass_Make1 = 0;     /* Reject make code 1. */
    Pass_Make2 = 0;     /* Reject make code 2. */
    scan.kbf_tail = 0;
    scan.kbf_head = 0;
    F_Service_UNLOCK = 1;       /* Post unlock service request. */
    Enable_Timer_A();           /* Load Timer A if not already enabled. */
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION:   general_reset1, general_reset2
// Initialize things common to both Cold/Warm reset.
//-----------------------------------------------------------------------------
static void Common_Reset(void)
{
    Service  = 0;       /* Clear service requests */
    Service1 = 0;

    SCI_Event_In_Index  = 0;
    SCI_Event_Out_Index = 0;
    SMI_Event_In_Index  = 0;
    SMI_Event_Out_Index = 0;

    Timer_A.byte  = 0;  /* Clear Timer A events */
    Timer_B.byte  = 0;  /* Clear Timer B events */

    Scanner_State = 0;  /* Clear scanner state */

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: Init_PS2 - Initialize the PS2 Port variables.
//-----------------------------------------------------------------------------
void Init_PS2(void)
{
    PS2_IRQ_Channel = 0xFF; 
    PS2_SSIRQ_Channel = 0xFF; 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// The function of DMA for scratch sram
//
//-----------------------------------------------------------------------------
const sDMAScratchSRAM code asDMAScratchSRAM[]=
{
    { &SCAR1H, &SCAR1M, &SCAR1L },  //1024bytes(externl ram 0x800~BFF)
    { &SCAR2H, &SCAR2M, &SCAR2L },  // 512bytes(externl ram 0xC00~DFF)
    { &SCAR3H, &SCAR3M, &SCAR3L },  // 256bytes(externl ram 0xE00~EFF)
    { &SCAR4H, &SCAR4M, &SCAR4L }   // 256bytes(externl ram 0xF00~FFF)

};
//-----------------------------------------------------------------------------
void  CacheDma( BYTE sramnum, WORD addr )
{
    *asDMAScratchSRAM[sramnum].scarh = 0x80;
    /* high byte of function address.   */
    *asDMAScratchSRAM[sramnum].scarm = (WORD) addr >> 8;
    /* low byte of function address.    */
    *asDMAScratchSRAM[sramnum].scarl = (WORD) addr & 0xff;
    /* start cache dma                  */
    *asDMAScratchSRAM[sramnum].scarh = 0x00;

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Init. DMA for scratch sram
//
// Segment 4: Don' use 0xF00 ~ 0xFFF (used by C compiler)
//-----------------------------------------------------------------------------
void Init_Cache(void)
{
    // Dino for 8500
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// INIT END HERE ......
//*****************************************************************************

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/


