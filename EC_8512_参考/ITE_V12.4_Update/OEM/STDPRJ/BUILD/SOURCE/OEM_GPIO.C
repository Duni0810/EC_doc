/*-----------------------------------------------------------------------------
 * TITLE: OEM_GPIO.C - MMCR (Mostly GPIO) Initialization.
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

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
//#define

//-----------------------------------------------------------------------------
// External ram GPIO Registers
// Parameter: INPUT,OUTPUT,PALT,PULL_UP,PULL_DN
//-----------------------------------------------------------------------------
const sInitGPIOReg code asInitGPIOReg[] =
{
    { &GCR,             0x06            },  //

    { &GPIOA,           0xF7            },  // Port A Data port Init

    { &GPIO_A0_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_A1_CTRL,    INPUT           },
    { &GPIO_A2_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_A3_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_A4_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_A5_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_A6_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_A7_CTRL,    OUTPUT+PULL_UP  },

    { &GPIOB,           0xF8            },  // Port B Data port Init

    { &GPIO_B0_CTRL,    INPUT           },
    { &GPIO_B1_CTRL,    INPUT           },
    { &GPIO_B2_CTRL,    OUTPUT          },
    { &GPIO_B3_CTRL,    PALT+PULL_UP    },
    { &GPIO_B4_CTRL,    PALT+PULL_UP    },
    { &GPIO_B5_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_B6_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_B7_CTRL,    OUTPUT+PULL_UP  },

    { &GPIOC,           0x00            },  // Port C Data port Init

    { &GPIO_C0_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_C1_CTRL,    PALT+PULL_UP    },
    { &GPIO_C2_CTRL,    PALT+PULL_UP    },
    { &GPIO_C3_CTRL,    PALT+PULL_UP    },
    { &GPIO_C4_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_C5_CTRL,    OUTPUT          },
    { &GPIO_C6_CTRL,    INPUT+PULL_UP   },
    { &GPIO_C7_CTRL,    INPUT+PULL_UP   },

    { &GPIOD,           0x5C            },  // Port D Data port Init

    { &GPIO_D0_CTRL,    INPUT           },
    { &GPIO_D1_CTRL,    INPUT+PULL_UP   },
    { &GPIO_D2_CTRL,    INPUT+PULL_UP   },
    { &GPIO_D3_CTRL,    OUTPUT          },
    { &GPIO_D4_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_D5_CTRL,    INPUT+PULL_UP   },
    { &GPIO_D6_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_D7_CTRL,    PALT            },

    { &GPIOE,           0x04            },  // Port E Data port Init

    { &GPIO_E0_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_E1_CTRL,    INPUT+PULL_DN   },
    { &GPIO_E2_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_E3_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_E4_CTRL,    INPUT+PULL_UP   },
    { &GPIO_E5_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_E6_CTRL,    INPUT+PULL_DN   },
    { &GPIO_E7_CTRL,    PALT            },

    { &GPIOF,           0x40            },  // Port F Data port Init

    { &GPIO_F0_CTRL,    PALT            },
    { &GPIO_F1_CTRL,    PALT            },
    { &GPIO_F2_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_F3_CTRL,    INPUT+PULL_UP   },
    { &GPIO_F4_CTRL,    INPUT           },
    { &GPIO_F5_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_F6_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_F7_CTRL,    OUTPUT+PULL_UP  },

    { &GPIOG,           0x02            },  // Port G Data port Init

    { &GPIO_G0_CTRL,    PALT            },
    { &GPIO_G1_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_G2_CTRL,    INPUT           },
    { &GPIO_G3_CTRL,    PALT            },
    { &GPIO_G4_CTRL,    PALT            },
    { &GPIO_G5_CTRL,    PALT            },
    { &GPIO_G6_CTRL,    INPUT           },
    { &GPIO_G7_CTRL,    PALT            },

    { &GPIOH,           0x01            },  // Port H Data port Init

    { &GPIO_H0_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_H1_CTRL,    INPUT+PULL_DN   },
    { &GPIO_H2_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_H3_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_H4_CTRL,    INPUT+PULL_DN   },
    { &GPIO_H5_CTRL,    INPUT+PULL_DN   },
    { &GPIO_H6_CTRL,    OUTPUT+PULL_UP  },
    { &GPIO_H7_CTRL,    PALT            },

    { &GPIOI,           0x30            },  // Port I Data port Init

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
    { &GPIO_J1_CTRL,    PALT            },
    { &GPIO_J2_CTRL,    PALT            },
    { &GPIO_J3_CTRL,    PALT            },
    { &GPIO_J4_CTRL,    INPUT           },
    { &GPIO_J5_CTRL,    INPUT           }

};

//-----------------------------------------------------------------------------
// The function of GPIO register init.
//-----------------------------------------------------------------------------
void Init_GPIO(void)
{
    BYTE iTemp;
    iTemp = 0x00;
    while( iTemp<(sizeof(asInitGPIOReg)/3) )
    {
        *asInitGPIOReg[iTemp].reg=asInitGPIOReg[iTemp].value;
        iTemp++;
    }

}
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/
