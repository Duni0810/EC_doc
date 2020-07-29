/*-----------------------------------------------------------------------------
 * Filename: ITE_BOOT.C     For Chipset: ITE.IT85XX
 *
 * Function: Cold-Reset Leader
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
// Common Header Include
//-----------------------------------------------------------------------------
#include    "BBK_COMM.H"

//-----------------------------------------------------------------------------
// Externel Link Header
//-----------------------------------------------------------------------------
#include    "BBK_OEM.H"

//=============================================================================
// Local Header
//=============================================================================
#include    "ITE_BOOT.H"

/* ************************************************************************* */
/* CODE START                                                                */
/* ************************************************************************* */
void main(void)
{
    FUNCT_PTR_V_V code GoMainBoot = 0x1000;

    SP = 0xD0;
    EA = 0x00;
    TMOD  = 0x11;
    TF0 = TR0 = 0;  // Stop timer 0 and clear interrupt
    TF1 = TR1 = 0;  // Stop timer 1 and clear interrupt

    //- - - - - - - - - - - - - - - - -
    //Version Information Load...
    CORE_Version = KernelCode_Version;
    Oem_SubVer   = EC_SUB_VER;
    Oem_BBKVer   = EC_BBK_VER;
    Oem_CodeVer  = EC_OEM_VER;
    Oem_ModelVer = EC_MODEL_VER;
    Oem_PCBVer   = EC_PCB_VER;
    Oem_TestVer  = EC_TEST_VER;
    //- - - - - - - - - - - - - - - - -

    #if !SUPPORTED_EC_BOOT_BLOCK
    GoMainBoot();
    #endif

    if( BB_CheckEntry() )   // Check Entry Condition
    {
        uBoot_InitGPIO();
        uBoot_InitRegs();
        BB_Main();          //BL51 Locate: ?PR?BB_MAIN?CORE_BBLOCK(0x0200)
    }

    GoMainBoot();

}

//-----------------------------------------------------------------------------
// The table of GPIO register init.
//-----------------------------------------------------------------------------
const sInitGPIOReg code uBootInitGPIOReg[] =
{

    { &GCR,             0x00            },  //

    { &GPIOA,           0x00            },  // Port A Data port Init

    { &GPIO_A0_CTRL,    INPUT           },
    { &GPIO_A1_CTRL,    INPUT           },
    { &GPIO_A2_CTRL,    INPUT           },
    { &GPIO_A3_CTRL,    INPUT           },
    { &GPIO_A4_CTRL,    INPUT           },
    { &GPIO_A5_CTRL,    INPUT           },
    { &GPIO_A6_CTRL,    INPUT           },
    { &GPIO_A7_CTRL,    INPUT           },

    { &GPIOB,           0x00            },  // Port B Data port Init

    { &GPIO_B0_CTRL,    INPUT           },
    { &GPIO_B1_CTRL,    INPUT           },
    { &GPIO_B2_CTRL,    INPUT           },
    { &GPIO_B3_CTRL,    INPUT           },
    { &GPIO_B4_CTRL,    INPUT           },
    { &GPIO_B5_CTRL,    INPUT           },
    { &GPIO_B6_CTRL,    INPUT           },
    { &GPIO_B7_CTRL,    INPUT           },

    { &GPIOC,           0x00            },  // Port C Data port Init

    { &GPIO_C0_CTRL,    INPUT           },
    { &GPIO_C1_CTRL,    INPUT           },
    { &GPIO_C2_CTRL,    INPUT           },
    { &GPIO_C3_CTRL,    INPUT           },
    { &GPIO_C4_CTRL,    INPUT           },
    { &GPIO_C5_CTRL,    INPUT           },
    { &GPIO_C6_CTRL,    INPUT           },
    { &GPIO_C7_CTRL,    INPUT           },

    { &GPIOD,           0x00            },  // Port D Data port Init

    { &GPIO_D0_CTRL,    INPUT           },
    { &GPIO_D1_CTRL,    INPUT           },
    { &GPIO_D2_CTRL,    INPUT           },
    { &GPIO_D3_CTRL,    INPUT           },
    { &GPIO_D4_CTRL,    INPUT           },
    { &GPIO_D5_CTRL,    INPUT           },
    { &GPIO_D6_CTRL,    INPUT           },
    { &GPIO_D7_CTRL,    INPUT           },

    { &GPIOE,           0x00            },  // Port E Data port Init

    { &GPIO_E0_CTRL,    INPUT           },
    { &GPIO_E1_CTRL,    INPUT           },
    { &GPIO_E2_CTRL,    INPUT           },
    { &GPIO_E3_CTRL,    INPUT           },
    { &GPIO_E4_CTRL,    INPUT           },
    { &GPIO_E5_CTRL,    INPUT           },
    { &GPIO_E6_CTRL,    INPUT           },
    { &GPIO_E7_CTRL,    INPUT           },

    { &GPIOF,           0x00            },  // Port F Data port Init

    { &GPIO_F0_CTRL,    INPUT           },
    { &GPIO_F1_CTRL,    INPUT           },
    { &GPIO_F2_CTRL,    INPUT           },
    { &GPIO_F3_CTRL,    INPUT           },
    { &GPIO_F4_CTRL,    INPUT           },
    { &GPIO_F5_CTRL,    INPUT           },
    { &GPIO_F6_CTRL,    INPUT           },
    { &GPIO_F7_CTRL,    INPUT           },

    { &GPIOG,           0x00            },  // Port G Data port Init

    { &GPIO_G0_CTRL,    INPUT           },
    { &GPIO_G1_CTRL,    INPUT           },
    { &GPIO_G2_CTRL,    INPUT           },
    { &GPIO_G3_CTRL,    INPUT           },
    { &GPIO_G4_CTRL,    INPUT           },
    { &GPIO_G5_CTRL,    INPUT           },
    { &GPIO_G6_CTRL,    INPUT           },
    { &GPIO_G7_CTRL,    INPUT           },

    { &GPIOH,           0x00            },  // Port H Data port Init

    { &GPIO_H0_CTRL,    INPUT           },
    { &GPIO_H1_CTRL,    INPUT           },
    { &GPIO_H2_CTRL,    INPUT           },
    { &GPIO_H3_CTRL,    INPUT           },
    { &GPIO_H4_CTRL,    INPUT           },
    { &GPIO_H5_CTRL,    INPUT           },
    { &GPIO_H6_CTRL,    INPUT           },
    { &GPIO_H7_CTRL,    INPUT           },

    { &GPIOI,           0x00            },  // Port I Data port Init

    { &GPIO_I0_CTRL,    INPUT           },
    { &GPIO_I1_CTRL,    INPUT           },
    { &GPIO_I2_CTRL,    INPUT           },
    { &GPIO_I3_CTRL,    INPUT           },
    { &GPIO_I4_CTRL,    INPUT           },
    { &GPIO_I5_CTRL,    INPUT           },
    { &GPIO_I6_CTRL,    INPUT           },
    { &GPIO_I7_CTRL,    INPUT           },

    { &GPIOJ,           0x00            },  // Port J Data port Init

    { &GPIO_J0_CTRL,    INPUT           },
    { &GPIO_J1_CTRL,    INPUT           },
    { &GPIO_J2_CTRL,    INPUT           },
    { &GPIO_J3_CTRL,    INPUT           },
    { &GPIO_J4_CTRL,    INPUT           },
    { &GPIO_J5_CTRL,    INPUT           }

};
//-----------------------------------------------------------------------------
// Cold Boot GPIO Init
//-----------------------------------------------------------------------------
void uBoot_InitGPIO(void)
{
    BYTE iLoop;

    iLoop = 0x00;

    while(iLoop<(sizeof(uBootInitGPIOReg)/3))
    {
        *uBootInitGPIOReg[iLoop].reg=uBootInitGPIOReg[iLoop].value;
        iLoop++;
    }

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// The table of register init.
//-----------------------------------------------------------------------------
struct BB_REG_INIT
{
    unsigned char xdata       *address;
    unsigned char             initdata;
};
//-----------------------------------------------------------------------------
const struct BB_REG_INIT code BB_reg_init[] =
{

    // HOST interface
    { &BADRSEL      ,0x00           },  //Disable KBC IRQ
    { &RSTS         ,0x84           },  //Disable KBC IRQ
    { &KBIRQR       ,0x00           },  //Disable KBC IRQ
    { &KBHICR       ,BIT0+BIT3+BIT6 },  //KBC port control IBFCIE+PMICIE
    { &PM1CTL       ,BIT0+BIT6      },  //EC port control  IBFCIE+SCI low active
    { &PM2CTL       ,BIT0+BIT6      },  //EC port control  IBFCIE+SCI low active
    { &IER3         ,BIT1+BIT0      },  //EC port2 control IBFCIE+SCI low active
    { &KSOCTRL      ,BIT0+BIT2      },  //
    { &KSICTRL      ,BIT2           },  //
    { &DACCTRL      ,0x0F           },  //
    { &DACPDREG     ,0x00           },  //

};

//-----------------------------------------------------------------------------
// The function of register init.
//-----------------------------------------------------------------------------
void uBoot_InitRegs(void)
{
    BYTE index;
    index = 0x00;

    while(index < (sizeof(BB_reg_init)/3) )
    {
        *BB_reg_init[index].address=BB_reg_init[index].initdata;
        index ++;
    }

}

//-----------------------------------------------------------------------------

/* ************************************************************************* */
// CODE END
