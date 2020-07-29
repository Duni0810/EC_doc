###############################################################################
#* Filename: ITEEC_BB.mak
#* Function: Makefile for ITE Embedded Controller Boot-Block Firmware.
#* Notes: use with Microsoft NMAKER utility version 1.50.
#*
#* Copyright (c) 2006-2008, ITE Tech. Inc. All Rights Reserved.
#*                          ITE Technical Marketing Firmware Center
###############################################################################
COREInclude=SOURCE\*.H
OEMInclude=SOURCE\*.H
CDirectives=LA WL(1) CD OT(8, size) OR
ADirectives=SET (LA) DEBUG EP
BuildPath_OBJ=OBJ
BuildPath_LST=LST

# ----- Common Definitions ----------------------------------------------------
ON=1
OFF=0

# ----- Kernel Boot-Block CORE Features ---------------------------------------
CoreBBK_01=ITE_BOOT
CoreBBK_02=CORE_MEMORY
CoreBBK_03=BBK_MAIN
CoreBBK_04=BBK_IRQ
CoreBBK_05=BBK_OEM

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
ALL:EC_BBK.BIN

# -----------------------------------------------------------------------------
# Hex file to bin file
# -----------------------------------------------------------------------------
EC_BBK.BIN:EC_BBK.HEX
            Hex2bin -s 0x0000 -k 2 EC_BBK.HEX
            

EC_BBK.HEX:EC_BBK.ABS
            OHX51 EC_BBK.ABS HEXFILE (EC_BBK.HEX)

# -----------------------------------------------------------------------------
# Link all obj fils
# -----------------------------------------------------------------------------
EC_BBK.ABS:  $(BuildPath_OBJ)\$(CoreBBK_01).OBJ\
             $(BuildPath_OBJ)\$(CoreBBK_02).OBJ\
             $(BuildPath_OBJ)\$(CoreBBK_03).OBJ\
             $(BuildPath_OBJ)\$(CoreBBK_04).OBJ\
             $(BuildPath_OBJ)\$(CoreBBK_05).OBJ\
             $(BuildPath_OBJ)\L51_BANK.OBJ\
             $(BuildPath_OBJ)\STARTUP.OBJ
        lx51 @BBK_4K.L51

# -----------------------------------------------------------------------------
# Compile: Kernel Boot-Block Core files
# -----------------------------------------------------------------------------
$(BuildPath_OBJ)\$(CoreBBK_01).OBJ:SOURCE\$(CoreBBK_01).C  SOURCE\*.H
         $(CC) SOURCE\$(CoreBBK_01).C $(CDirectives)
         move  SOURCE\$(CoreBBK_01).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreBBK_01).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreBBK_02).OBJ:SOURCE\$(CoreBBK_02).C SOURCE\*.H
         $(CC) SOURCE\$(CoreBBK_02).C $(CDirectives)
         move  SOURCE\$(CoreBBK_02).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreBBK_02).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreBBK_03).OBJ:SOURCE\$(CoreBBK_03).C SOURCE\*.H
         $(CC) SOURCE\$(CoreBBK_03).C $(CDirectives)
         move  SOURCE\$(CoreBBK_03).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreBBK_03).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreBBK_04).OBJ:SOURCE\$(CoreBBK_04).C SOURCE\*.H
         $(CC) SOURCE\$(CoreBBK_04).C $(CDirectives)
         move  SOURCE\$(CoreBBK_04).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreBBK_04).LST $(BuildPath_LST)

$(BuildPath_OBJ)\$(CoreBBK_05).OBJ:SOURCE\$(CoreBBK_05).C SOURCE\*.H
         $(CC) SOURCE\$(CoreBBK_05).C $(CDirectives)
         move  SOURCE\$(CoreBBK_05).OBJ $(BuildPath_OBJ)
         move  SOURCE\$(CoreBBK_05).LST $(BuildPath_LST)

# -----------------------------------------------------------------------------
# Compile: Code and Variable Banking for Classic 8051 Derivatives
# -----------------------------------------------------------------------------
$(BuildPath_OBJ)\L51_BANK.OBJ:SOURCE\L51_BANK.A51
         $(AS) SOURCE\L51_BANK.A51 $(ADirectives)
         move  SOURCE\L51_BANK.OBJ $(BuildPath_OBJ)
         move  SOURCE\L51_BANK.LST $(BuildPath_LST)

# -----------------------------------------------------------------------------
# Compile: For Code Start-up C51 package
# -----------------------------------------------------------------------------
$(BuildPath_OBJ)\STARTUP.OBJ:SOURCE\STARTUP.A51
         $(AS) SOURCE\STARTUP.A51 $(ADirectives)
         move  SOURCE\STARTUP.OBJ $(BuildPath_OBJ)
         move  SOURCE\STARTUP.LST $(BuildPath_LST)

# -----------------------------------------------------------------------------
# End...
