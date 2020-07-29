###############################################################################
#* Filename: PROJECT.mak
#* Function: The top level makefile for ITE Embedded Controller Firmware.
#* Notes: use with Microsoft NMAKER utility version 1.50.
#*
#* Copyright (c) 2006-2008, ITE Tech. Inc. All Rights Reserved.
#*                          ITE Technical Marketing Firmware Center
###############################################################################
COREInclude=SOURCE\*.H
OEMInclude=SOURCE\*.H
CDirectives=LA WL(1) CD OT(8, size) OR
CDirectives1=LA WL(1) CD OT(8, size) OR
ADirectives=SET (LA) DEBUG EP
BuildPath_OBJ=OBJ
BuildPath_LST=LST

# ----- Common Definitions ----------------------------------------------------
ON=1
OFF=0

# ----- Kernel CORE Features --------------------------------------------------
CoreModule01=CORE_MEMORY
CoreModule02=CORE_ACPI
CoreModule03=CORE_MODULE
CoreModule04=CORE_XLT
CoreModule05=CORE_FLASH
CoreModule06=CORE_HOSTIF
CoreModule07=CORE_INIT
CoreModule08=CORE_MAIN
CoreModule09=CORE_PORT6064
CoreModule10=CORE_PS2
CoreModule11=CORE_SCAN
CoreModule12=CORE_SMBUS
CoreModule13=CORE_TIMERS
CoreModule14=CORE_PECI
CoreModule15=CORE_HSPI

# ----- OEM CORE Features -----------------------------------------------------
OemModule_01=OEM_BATTERY
OemModule_02=OEM_CIR
OemModule_03=OEM_EC6266
OemModule_04=OEM_FAN
OemModule_05=OEM_FUNCTION
OemModule_06=OEM_GPIO
OemModule_07=OEM_KBC6064
OemModule_08=OEM_KBMATRIX
OemModule_09=OEM_LED
OemModule_10=OEM_MAIN    # Oem Hooks
OemModule_11=OEM_MEMORY
OemModule_12=OEM_PMC2
OemModule_13=OEM_POWER   # Oem use EC Control System Power.
OemModule_14=OEM_SMBUS
OemModule_15=OEM_SBS

###############################################################################
# SUFFIXES & COMPILER/ASSEMBLER/LINKER DEFINITIONS
# AS, AFLAGS    - Assembler, Assembler command options
# CC, CFLAGS    - Compiler, Compiler command options
###############################################################################
CC=C51  # Keil C51 Compiler
AS=AX51 # Keil Macro Assembler

# -----------------------------------------------------------------------------
# ALL: Building Process starts here
# -----------------------------------------------------------------------------
ALL:ITE_EC.BIN

# -----------------------------------------------------------------------------
# Hex file to bin file
# -----------------------------------------------------------------------------
ITE_EC.BIN:ITE_EC.HEX
        Hex2bin -s 0x0000 -k 2 ITE_EC.HEX

ITE_EC.HEX:ITE_EC.ABS
        OHX51 ITE_EC.ABS HEXFILE (ITE_EC.HEX) H386 MERGE32K

# -----------------------------------------------------------------------------
# Link all obj fils
# -----------------------------------------------------------------------------
ITE_EC.ABS:$(BuildPath_OBJ)\$(CoreModule01).OBJ\
           $(BuildPath_OBJ)\$(CoreModule02).OBJ\
           $(BuildPath_OBJ)\$(CoreModule03).OBJ\
           $(BuildPath_OBJ)\$(CoreModule04).OBJ\
           $(BuildPath_OBJ)\$(CoreModule05).OBJ\
           $(BuildPath_OBJ)\$(CoreModule06).OBJ\
           $(BuildPath_OBJ)\$(CoreModule07).OBJ\
           $(BuildPath_OBJ)\$(CoreModule08).OBJ\
           $(BuildPath_OBJ)\$(CoreModule09).OBJ\
           $(BuildPath_OBJ)\$(CoreModule10).OBJ\
           $(BuildPath_OBJ)\$(CoreModule11).OBJ\
           $(BuildPath_OBJ)\$(CoreModule12).OBJ\
           $(BuildPath_OBJ)\$(CoreModule13).OBJ\
           $(BuildPath_OBJ)\$(CoreModule14).OBJ\
           $(BuildPath_OBJ)\$(CoreModule15).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_01).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_02).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_03).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_04).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_05).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_06).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_07).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_08).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_09).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_10).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_11).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_12).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_13).OBJ\
                          $(BuildPath_OBJ)\$(OemModule_14).OBJ\
          $(BuildPath_OBJ)\L51_BANK.OBJ
    LX51  @PROJECT.L51

# -----------------------------------------------------------------------------
# Compile: Kernel Core files
# -----------------------------------------------------------------------------
$(BuildPath_OBJ)\$(CoreModule01).OBJ:SOURCE\$(CoreModule01).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule01).C $(CDirectives)
         move  SOURCE\$(CoreModule01).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule01).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule02).OBJ:SOURCE\$(CoreModule02).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule02).C $(CDirectives)
         move  SOURCE\$(CoreModule02).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule02).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule03).OBJ:SOURCE\$(CoreModule03).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule03).C $(CDirectives)
         move  SOURCE\$(CoreModule03).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule03).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule04).OBJ:SOURCE\$(CoreModule04).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule04).C $(CDirectives)
         move  SOURCE\$(CoreModule04).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule04).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule05).OBJ:SOURCE\$(CoreModule05).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule05).C $(CDirectives1)
         move  SOURCE\$(CoreModule05).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule05).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule06).OBJ:SOURCE\$(CoreModule06).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule06).C $(CDirectives)
         move  SOURCE\$(CoreModule06).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule06).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule07).OBJ:SOURCE\$(CoreModule07).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule07).C $(CDirectives)
         move  SOURCE\$(CoreModule07).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule07).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule08).OBJ:SOURCE\$(CoreModule08).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule08).C $(CDirectives)
         move  SOURCE\$(CoreModule08).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule08).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule09).OBJ:SOURCE\$(CoreModule09).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule09).C $(CDirectives)
         move  SOURCE\$(CoreModule09).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule09).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule10).OBJ:SOURCE\$(CoreModule10).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule10).C $(CDirectives)
         move  SOURCE\$(CoreModule10).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule10).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule11).OBJ:SOURCE\$(CoreModule11).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule11).C $(CDirectives)
         move  SOURCE\$(CoreModule11).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule11).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule12).OBJ:SOURCE\$(CoreModule12).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule12).C $(CDirectives)
         move  SOURCE\$(CoreModule12).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule12).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule13).OBJ:SOURCE\$(CoreModule13).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule13).C $(CDirectives)
         move  SOURCE\$(CoreModule13).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule13).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule14).OBJ:SOURCE\$(CoreModule14).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule14).C $(CDirectives)
         move  SOURCE\$(CoreModule14).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule14).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreModule15).OBJ:SOURCE\$(CoreModule15).C SOURCE\*.H
         $(CC) SOURCE\$(CoreModule15).C $(CDirectives)
         move  SOURCE\$(CoreModule15).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreModule15).LST $(BuildPath_LST)             

#------------------------------------------------------------------
# Compile: OEM files
#=-----------------------------------------------------------------
$(BuildPath_OBJ)\$(OemModule_01).OBJ:SOURCE\$(OemModule_01).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_01).C $(CDirectives)
         move  SOURCE\$(OemModule_01).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_01).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_02).OBJ:SOURCE\$(OemModule_02).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_02).C $(CDirectives)
         move  SOURCE\$(OemModule_02).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_02).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_03).OBJ:SOURCE\$(OemModule_03).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_03).C $(CDirectives)
         move  SOURCE\$(OemModule_03).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_03).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_04).OBJ:SOURCE\$(OemModule_04).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_04).C $(CDirectives)
         move  SOURCE\$(OemModule_04).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_04).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_05).OBJ:SOURCE\$(OemModule_05).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_05).C $(CDirectives)
         move  SOURCE\$(OemModule_05).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_05).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_06).OBJ:SOURCE\$(OemModule_06).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_06).C $(CDirectives)
         move  SOURCE\$(OemModule_06).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_06).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_07).OBJ:SOURCE\$(OemModule_07).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_07).C $(CDirectives)
         move  SOURCE\$(OemModule_07).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_07).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_08).OBJ:SOURCE\$(OemModule_08).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_08).C $(CDirectives)
         move  SOURCE\$(OemModule_08).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_08).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_09).OBJ:SOURCE\$(OemModule_09).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_09).C $(CDirectives)
         move  SOURCE\$(OemModule_09).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_09).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_10).OBJ:SOURCE\$(OemModule_10).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_10).C $(CDirectives)
         move  SOURCE\$(OemModule_10).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_10).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_11).OBJ:SOURCE\$(OemModule_11).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_11).C $(CDirectives)
         move  SOURCE\$(OemModule_11).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_11).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_12).OBJ:SOURCE\$(OemModule_12).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_12).C $(CDirectives)
         move  SOURCE\$(OemModule_12).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_12).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_13).OBJ:SOURCE\$(OemModule_13).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_13).C $(CDirectives)
         move  SOURCE\$(OemModule_13).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_13).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(OemModule_14).OBJ:SOURCE\$(OemModule_14).C SOURCE\*.H
         $(CC) SOURCE\$(OemModule_14).C $(CDirectives)
         move  SOURCE\$(OemModule_14).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(OemModule_14).LST $(BuildPath_LST)

# -----------------------------------------------------------------------------
# Compile: Code and Variable Banking for Classic 8051 Derivatives
# -----------------------------------------------------------------------------
$(BuildPath_OBJ)\L51_BANK.OBJ:SOURCE\L51_BANK.A51
         $(AS) SOURCE\L51_BANK.A51 $(ADirectives)
         move  SOURCE\L51_BANK.OBJ $(BuildPath_OBJ)
         move  SOURCE\L51_BANK.LST $(BuildPath_LST)

# -----------------------------------------------------------------------------
# End...