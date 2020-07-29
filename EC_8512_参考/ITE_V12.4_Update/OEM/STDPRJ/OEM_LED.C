/*-----------------------------------------------------------------------------
 * TITLE: OEM_LED.C
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

/*-----------------------------------------------------------------------------
 * Local Process Definition
 *---------------------------------------------------------------------------*/


/* ----------------------------------------------------------------------------
 * FUNCTION: OEM_Write_Leds
 *
 * Write to SCROLL LOCK, NUM LOCK, CAPS LOCK, and any custom LEDs (including
 * an "Fn" key LED).
 *
 * Input: data (LED_ON bit) = Data to write to the LEDs
 *         bit 0 - SCROLL, Scroll Lock
 *         bit 1 - NUM,    Num Lock
 *         bit 2 - CAPS,   Caps Lock
 *         bit 3 - OVL,    Fn LED
 *         bit 4 - LED4,   Undefined
 *         bit 5 - LED5,   Undefined
 *         bit 6 - LED6,   Undefined
 *         bit 7 - LED7,   Undefined
 *
 * Updates Led_Data variable and calls send_leds if appropriate.
 * ------------------------------------------------------------------------- */
void OEM_Write_Leds(void)
{
    #if LED_SCROLLOCK_SUPPORTED
    if ( Led_Data_SCROLL )  // Check ScrollLock LED Status
    {
        SCROLED_ON();
    }
    else
    {
        SCROLED_OFF();
    }
    #endif

    #if LED_NUMLOCK_SUPPORTED
    if ( Led_Data_NUM ) // Check NumLock LED Status
    {
        NUMLED_ON();
    }
    else
    {
        NUMLED_OFF();
    }
    #endif

    #if LED_CAPSLOCK_SUPPORTED
    if ( Led_Data_CAPS )    // Check CapsLock LED Status
    {
        CAPLED_ON();
    }
    else
    {
        CAPLED_OFF();
    }
    #endif

}


//-----------------------------------------------------------------------------
// Oem LED Here
// Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
//-----------------------------------------------------------------------------
void Service_LED_Indicator(void)
{

    if (SysPowState == _SYSTEM_S5)
    {
        return;
    }

    if (CORE_PMFLAG_S0)
    {
        PWRLED1_ON();
    }
    else if (SysPowState == _SYSTEM_S3)
    {
        PWRLED1_TG();
    }

    if( STATE_CARD() )
    {
        CARD_LED_OFF(); //Hi->No Card
    }
    else
    {
        CARD_LED_ON();
    }
    //BATLED0_TG();   //BLUE COLOR
    BATLED1_TG();   //YELLOW COLOR
}


//-----------------------------------------------------------------------------
// Oem LED Here
//-----------------------------------------------------------------------------





















/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/