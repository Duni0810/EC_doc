/*-----------------------------------------------------------------------------
 * Filename: OEM_GPIO.H
 * Function: OEM_GPIO.C GPIO Prototype Include Definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
//#ifndef OEM_GPIO_H
//#define OEM_GPIO_H

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern  void Init_GPIO(void);
//extern  void Process_EGPC_Mapping(void);

//-----------------------------------------------------------------------------


/*-----------------------------------------------------------------------------
 * Parameter Definition
 *---------------------------------------------------------------------------*/
#define PALT            0x00
#define INPUT           0x80
#define OUTPUT          0x40
#define PULL_UP         0x04
#define PULL_DN         0x02
#define Driving4mA      0x00    //4mA
#define Driving8mA      0x10    //8mA
#define Driving12mA     0x18    //12mA
#define MaxDriving      0x38    //16mA

//-----------------------------------------------------------------
// GPIO Active
//-----------------------------------------------------------------
#define GPin0           bit0
#define GPin1           bit1
#define GPin2           bit2
#define GPin3           bit3
#define GPin4           bit4
#define GPin5           bit5
#define GPin6           bit6
#define GPin7           bit7
#define HighActive      1
#define LowActive       0

/*-----------------------------------------------------------------------------
 * Data Structure
 *---------------------------------------------------------------------------*/
typedef struct InitGPIOReg
{
    XBYTE   *reg;
    BYTE    value;
} sInitGPIOReg;


/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
//extern void Init_GPIO(void);


//-----------------------------------------------------------------
// GPIO Active
//-----------------------------------------------------------------

#define adapter_in      BIT0        // 1 AC adapter exist
#define power_on        BIT1        // 1 system power on
#define enter_S3        BIT2        // 1 host enter suspend to RAM mode
#define wait_PSW_off    BIT3        // 1 waiting for PSW released
#define enter_S4        BIT4        // 1 host enter suspend to HD mode
#define system_on       BIT5        // 1 system on (SWPG issued)
#define wait_HWPG       BIT6        // 1 wait hardware power good
#define rvcc_on         BIT7        // 1 RVCC turnned on

#define susb_on         BIT0        // 1 On SUSB Status
#define susc_on         BIT1        // 1 On SUSC Status
#define fan_on          BIT2        // 1 Fan on

//-----------------------------------------------------------------
// INPUT Function
//-----------------------------------------------------------------
#define STATE_PowerSW()     IS_BIT_SET(GPIOE,4)
#define PowerSW_Active      0           //1:HighActive 0:LowActive

#define STATE_PM_SLPS3()    IS_BIT_SET(GPIOA,1) //SUSB
#define STATE_PM_SLPS5()    IS_BIT_SET(GPIOF,3) //SUSC
#define STATE_SUSB()        IS_BIT_SET(GPIOA,1) //SUSB
#define STATE_SUSC()        IS_BIT_SET(GPIOF,3) //SUSC
#define STATE_HWPG()        IS_BIT_SET(GPIOI,7) //Hardware Power Good
#define STATE_RCVV()        IS_BIT_SET(GPIOH,2) //RCVV
#define STATE_ACin()        IS_BIT_SET(GPIOD,1) //RCVV
#define STATE_BATin()       IS_BIT_SET(GPIOI,1) //RCVV
#define STATE_PCIRST()      IS_BIT_SET(GPIOD,2) //PCIRST

//-----------------------------------------------------------------
// Host Bus Control
//-----------------------------------------------------------------
#define SCI_ON()            CLR_BIT(GPIOD,3)
#define SCI_OFF()           SET_BIT(GPIOD,3)
#define STATE_SCI()         IS_BIT_SET(GPIOD,3)

#define SMI_ON()            CLR_BIT(GPIOD,4)
#define SMI_OFF()           SET_BIT(GPIOD,4)
#define STATE_SMI()         IS_BIT_SET(GPIOD,4)

#define GATEA20_ON()        SET_BIT(GPIOB,5)
#define GATEA20_OFF()       CLR_BIT(GPIOB,5)
#define STATE_GATEA20()     IS_BIT_SET(GPIOB,5)

#define KBRST_ON()          CLR_BIT(GPIOB,6)
#define KBRST_OFF()         SET_BIT(GPIOB,6)

#define PM_PWRBTN_LO()      CLR_BIT(GPIOF,6)
#define PM_PWRBTN_HI()      SET_BIT(GPIOF,6)

//-----------------------------------------------------------------
// Power Sequence Output Control
//-----------------------------------------------------------------
#define PWRBTN_LO()     CLR_BIT(GPIOF,6)
#define PWRBTN_HI()     SET_BIT(GPIOF,6)
#define DNBSWON_ON()    CLR_BIT(GPIOF,6)
#define DNBSWON_OFF()   SET_BIT(GPIOF,6)
#define RSMRST_ON()     CLR_BIT(GPIOF,5)
#define RSMRST_OFF()    SET_BIT(GPIOF,5)
#define RVCC_ON()       SET_BIT(GPIOH,2)
#define RVCC_OFF()      CLR_BIT(GPIOH,2)
#define USBPWER_ON()    CLR_BIT(GPIOE,0)
#define USBPWER_OFF()   SET_BIT(GPIOE,0)
#define LANRSET_ON()    SET_BIT(GPIOD,1)
#define LANRSET_OFF()   CLR_BIT(GPIOD,1)
#define SUSON_ON()      SET_BIT(GPIOH,6)
#define SUSON_OFF()     CLR_BIT(GPIOH,6)
#define MAINON_ON()     SET_BIT(GPIOF,7)
#define MAINON_OFF()    CLR_BIT(GPIOF,7)
#define VRON_ON()       SET_BIT(GPIOC,4)
#define VRON_OFF()      CLR_BIT(GPIOC,4)
#define POWEROK_ON()    SET_BIT(GPIOA,4)
#define POWEROK_OFF()   CLR_BIT(GPIOA,4)
#define MPWROK_ON()     SET_BIT(GPIOF,2)
#define MPWROK_OFF()    CLR_BIT(GPIOF,2)
#define CCDPWR_ON()     CLR_BIT(GPIOE,5)
#define CCDPWR_OFF()    SET_BIT(GPIOE,5)

//-----------------------------------------------------------------
// Pin For Debug Signal
//-----------------------------------------------------------------
#define TEST_PIN1()     INVERSE_REG(GPIOG,5)
#define TEST_PIN2()     INVERSE_REG(GPIOG,5)
#define TEST_PIN3()     INVERSE_REG(GPIOG,5)

//-----------------------------------------------------------------
// OEM_LED.C
//-----------------------------------------------------------------
// KB LEDs - Scrollock LED,NumLock LED,CapsLock LED
#define NUMLED_ON()     CLR_BIT(GPIOC,5)
#define NUMLED_OFF()    SET_BIT(GPIOC,5)
#define NUMLED_TG()     INVERSE_REG(GPIOC,5)
#define SCROLED_ON()    CLR_BIT(GPIOA,0)
#define SCROLED_OFF()   SET_BIT(GPIOA,0)
#define SCROLED_TG()    INVERSE_REG(GPIOA,0)
#define CAPLED_ON()     CLR_BIT(GPIOG,1)
#define CAPLED_OFF()    SET_BIT(GPIOG,1)
#define CAPLED_TG()     INVERSE_REG(GPIOG,1)
//-----------------------------------------------------------------
// System LED Indicator -
#define PWRLED1_ON()    CLR_BIT(GPIOA,5)
#define PWRLED1_OFF()   SET_BIT(GPIOA,5)
#define PWRLED1_TG()    INVERSE_REG(GPIOA,5)


#define BATLED0_ON()    CLR_BIT(GPIOA,7)
#define BATLED0_OFF()   SET_BIT(GPIOA,7)
#define BATLED0_TG()    INVERSE_REG(GPIOA,7)

#define BATLED1_ON()    CLR_BIT(GPIOA,6)
#define BATLED1_OFF()   SET_BIT(GPIOA,6)
#define BATLED1_TG()    INVERSE_REG(GPIOA,6)

#define SUSLED_ON()     CLR_BIT(GPIOA,5)
#define SUSLED_OFF()    SET_BIT(GPIOA,5)
#define SUSLED_TG()     INVERSE_REG(GPIOA,5)

#define STATE_CARD()    IS_BIT_SET(GPIOC,0) //CARD DETECT
#define CARD_LED_ON()   CLR_BIT(GPIOB,2)
#define CARD_LED_OFF()  SET_BIT(GPIOB,2)
#define CARD_LED_TG()   INVERSE_REG(GPIOB,2)

//-----------------------------------------------------------------------------
//#endif
