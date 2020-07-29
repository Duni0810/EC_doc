@ECHO OFF
REM ####################################################################
REM #* Filename: BUILD.BAT
REM #* Function: The BATCH file for ITE Embedded Controller Firmware.
REM #* Notes: use with Microsoft NMAKER utility version 1.50.
REM #*
REM #* Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
REM ####################################################################

IF     "%5" == "ALL"  GOTO BUILDALL
IF     "%5" == "BBK"  GOTO BUILDBBK
IF     "%5" == "NEW"  GOTO BUILDNEW
IF     "%5" == "OEM"  GOTO BUILDOEM
IF     "%5" == "all"  GOTO BUILDALL
IF     "%5" == "bbk"  GOTO BUILDBBK
IF     "%5" == "new"  GOTO BUILDNEW
IF     "%5" == "oem"  GOTO BUILDOEM
IF     "%5" == "NOBBK"  GOTO BUILDNOBBK
IF     "%5" == "ALLNOBBK"  GOTO BUILDALLNOBBK

IF     "%1" == "CLEAN"  GOTO BUILDCLR
IF     "%1" == "clean"  GOTO BUILDCLR
GOTO OPTIONS

:BUILDCLR
del /Q *.BIN
del /Q *.64K
del /Q OBJ\*.*
del /Q LST\*.*
del /Q SOURCE\*.*
del /Q MAP\*.*
ECHO.
ECHO    ITE Embedded Controller Firmware Build Process
ECHO    Clean All Build Buffer .................. OK !
ECHO.
GOTO DONE

:BUILDOEM
CD BUILD
REM #* CLEAR OEM BUILD ...
del /Q *.bin
del /Q OBJ\OEM*.*
del /Q LST\OEM*.*
del /Q INC\OEM*.*
GOTO BUILDNEW

:BUILDALL
CD BUILD
REM #* CLEAR BUILD ...
del /Q *.BIN
del /Q C\*.*
del /Q OBJ\*.*
del /Q LST\*.*
del /Q INC\*.*

:BUILDNEW

REM #* COPY CORE SOURCE INTO BUILD ...
COPY ..\..\..\BBLOCK\*.* SOURCE\*.*
COPY ..\..\..\BBLOCK\INCLUDE\*.* SOURCE\*.*

REM #* COPY OEM SOURCE INTO BUILD ...
COPY ..\..\..\BBLOCK\OEM\%3\*.* SOURCE\*.*
COPY ..\..\..\BBLOCK\OEM\%3\INCLUDE\*.* SOURCE\*.*

REM #* COPY CORE SOURCE INTO BUILD ...
COPY ..\..\..\CORE\*.* SOURCE\*.*
COPY ..\..\..\CORE\INCLUDE\*.H SOURCE\*.*

REM #* COPY MODULE SOURCE INTO BUILD ...
COPY ..\..\..\MODULE.OBJ\*.* SOURCE\*.*

REM #* COPY OEM SOURCE INTO BUILD ...
COPY ..\*.C SOURCE\*.*
COPY ..\*.H  SOURCE\*.*
COPY ..\INCLUDE\*.H SOURCE\*.*

REM #* COPY SUB MODEL SOURCE INTO BUILD ...
REM COPY ..\MODEL\%3\*.* SOURCE\*.*

GOTO MAKEALL

:BUILDBBK
@ECHO OFF
REM #* CLEAR BBK BUILD ...
del /Q OBJ\BBK*.OBJ
del /Q OBJ\ITE_BOOT.OBJ
del /Q OBJ\CORE_MEMORY.OBJ
del /Q OBJ\ITE_STR1.OBJ
del /Q OBJ\L51_BANK.OBJ
del /Q OBJ\STARTUP.OBJ

REM #* UPDATE BBK COMMON SOURCE BEFORE BUILD ...
COPY ..\..\..\BBLOCK\*.* SOURCE\*.*
COPY ..\..\..\BBLOCK\INCLUDE\*.* SOURCE\*.*

REM #* COPY OEM SOURCE INTO BUILD ...
COPY ..\..\..\BBLOCK\OEM\%3\*.* SOURCE\*.*
COPY ..\..\..\BBLOCK\OEM\%3\INCLUDE\*.* SOURCE\*.*

REM #* COPY CORE SOURCE INTO BUILD ...
COPY ..\..\..\CORE\CORE_MEMORY.C SOURCE\CORE_MEMORY.C
COPY ..\..\..\CORE\INCLUDE\*.H SOURCE\*.H

REM #* COPY OEM SOURCE INTO BUILD ...
REM COPY ..\..\..\OEM\%3\PROJECT.H SOURCE\PROJECT.H
REM COPY ..\..\..\OEM\%3\INCLUDE\*.* SOURCE\*.*
COPY ..\PROJECT.H SOURCE\PROJECT.H
COPY ..\INCLUDE\*.* SOURCE\*.*

PATH=%PATH%;C:\Keil\C51\BIN;..\..\..\Tools;..\..\..\Tools\NMake;
ECU /DISP

IF NOT EXIST %1 GOTO SKIPJ1
DEL /Q ..\RELEASE\EC_BBK.4KB
DEL /Q ..\RELEASE\EC_BBK.BIN
:SKIPJ1

NMAKE BBK_4K.mak
if errorlevel 2 goto BBKNG

move /Y *.MAP .\MAP
move /Y *.ABS .\MAP 
move /Y *.HEX .\MAP
move /Y *.SBR .\MAP

IF "%6" == "N"  GOTO SKIPFIXSZ1

ECU /SIZE 4 EC_BBK.BIN EC_BBK.4KB 00
move /Y EC_BBK.4KB ..\RELEASE
CD ..\RELEASE
ECU /+BIN EC_BBK.4KB ITE_EC.BIN %1
MOVE ITE_EC.BIN ..\BUILD
GOTO BBKOK2

:SKIPFIXSZ1
move /Y *.BIN      ..\RELEASE
CD ..\RELEASE
ECU /+BIN EC_BBK.BIN ITE_EC.BIN %1
:BBKOK2
COPY ..\..\..\BIOS_ROM\%2 %2
ECU /+BIN %1 %2 BIOS.ROM
DEL %2

DIR
ECU /CHECKSUM BIOS.ROM
ECU /CHECKSUM %1
ECU /CHECKSUM EC_BBK.4KB

IF "%4" == "N"  GOTO DONE
PAUSE
GOTO DONE

:BBKNG
move /Y *.MAP .\MAP
move /Y *.ABS .\MAP
move /Y *.HEX .\MAP
move /Y *.SBR .\MAP
move /Y *.BIN ..\RELEASE

ECHO.
ECHO    ITE Embedded Controller Firmware Build Process
ECHO    !!! Build Boot-Block Failed !!! ...
CD..
IF "%4" == "N"  GOTO DONE
ECHO.
PAUSE

GOTO DONE

:MAKEALL

PATH=C:\Keil\C51\BIN;..\..\..\Tools;..\..\..\Tools\NMake;
SET C51INC=..\INC\
ECU /DISP

IF NOT EXIST ..\RELEASE\%1 GOTO SKIPJ1
DEL /Q ..\RELEASE\%1
:SKIPJ1

NMAKE PROJECT.mak
if errorlevel 2 goto FAILED

NMAKE BBK_4K.mak
if errorlevel 2 goto FAILED

move /Y *.MAP .\MAP
move /Y *.ABS .\MAP
move /Y *.HEX .\MAP
move /Y *.SBR .\MAP

IF "%6" == "N"    GOTO SKIPFIXSZ2
IF "%6" == "128"  GOTO FIXSZ128

ECU /SIZE 4 EC_BBK.BIN EC_BBK.4KB 00
ECU /SIZE 64 ITE_EC.BIN ITE_EC.64K 00
ECU /+BIN EC_BBK.4KB ITE_EC.64K %1
GOTO FIXDOK
	
:FIXSZ128
ECU /SIZE 4 EC_BBK.BIN EC_BBK.4KB 00
ECU /SIZE 128 ITE_EC.BIN ITE_EC.128 00
ECU /+BIN EC_BBK.4KB ITE_EC.128 %1

:FIXDOK
move /Y EC_BBK.4KB ..\RELEASE
move /Y ITE_EC.BIN ..\RELEASE
move /Y %1 ..\RELEASE
CD ..\RELEASE
COPY ..\..\..\BIOS_ROM\%2 %2
ECU /+BIN %1 %2 BIOS.ROM
DEL %2
MOVE ITE_EC.BIN ..\BUILD
DIR
ECU /CHECKSUM BIOS.ROM
ECU /CHECKSUM %1
IF "%4" == "N"  GOTO DONE
PAUSE
GOTO DONE


:SKIPFIXSZ2
ECU /+BIN EC_BBK.BIN ITE_EC.BIN %1

move /Y EC_BBK.4KB ..\RELEASE
move /Y ITE_EC.BIN ..\RELEASE
move /Y %1 ..\RELEASE
CD ..\RELEASE
COPY ..\..\..\BIOS_ROM\%2 %2
ECU /+BIN %1 %2 BIOS.ROM
DEL %2
MOVE ITE_EC.BIN ..\BUILD
DIR
ECU /CHECKSUM BIOS.ROM
ECU /CHECKSUM %1

IF "%4" == "N"  GOTO DONE
PAUSE
GOTO DONE

:BUILDALLNOBBK
del /Q *.BIN
del /Q OBJ\*.*
del /Q LST\*.*

:BUILDNOBBK
REM #* COPY CORE SOURCE INTO BUILD ...
COPY ..\..\..\BBLOCK\*.* SOURCE\*.*
COPY ..\..\..\BBLOCK\INCLUDE\*.* SOURCE\*.*

REM #* COPY OEM SOURCE INTO BUILD ...
COPY ..\..\..\BBLOCK\OEM\%3\*.* SOURCE\*.*
COPY ..\..\..\BBLOCK\OEM\%3\INCLUDE\*.* SOURCE\*.*

REM #* COPY CORE SOURCE INTO BUILD ...
COPY ..\..\..\CORE\*.* SOURCE\*.*
COPY ..\..\..\CORE\INCLUDE\*.H SOURCE\*.*

REM #* COPY MODULE SOURCE INTO BUILD ...
COPY ..\..\..\MODULE.OBJ\*.* SOURCE\*.*

REM #* COPY OEM SOURCE INTO BUILD ...
COPY ..\*.C SOURCE\*.*
COPY ..\*.H  SOURCE\*.*
COPY ..\INCLUDE\*.H SOURCE\*.*

REM #* COPY SUB MODEL SOURCE INTO BUILD ...
REM COPY ..\MODEL\%3\*.* SOURCE\*.*

PATH=C:\Keil\C51\BIN;..\..\..\Tools;..\..\..\Tools\NMake;
SET C51INC=..\INC\
ECU /DISP

IF NOT EXIST ..\RELEASE\%1 GOTO SKIPJ1
DEL /Q ..\RELEASE\%1
:SKIPJ1

NMAKE P_NBBK.mak
if errorlevel 2 goto FAILED

move /Y *.MAP .\MAP
move /Y *.ABS .\MAP
move /Y *.HEX .\MAP
move /Y *.SBR .\MAP

IF "%6" == "N"    GOTO NBBKSKIPFIX
IF "%6" == "128"  GOTO NBBKFIX128

ECU /SIZE 64 ITE_EC.BIN ITE_EC.64K FF
copy ITE_EC.64K %1
GOTO NBBKFIXDOK
	
:NBBKFIX128
ECU /SIZE 128 ITE_EC.BIN ITE_EC.128 FF
copy ITE_EC.128 %1

:NBBKFIXDOK
move /Y ITE_EC.BIN ..\RELEASE
move /Y %1 ..\RELEASE
CD ..\RELEASE
COPY ..\..\..\BIOS_ROM\%2 %2
ECU /+BIN %1 %2 BIOS.ROM
DEL %2
MOVE ITE_EC.BIN ..\BUILD
ECU /CHECKSUM %1

IF "%4" == "N"  GOTO DONE
PAUSE
GOTO DONE


:NBBKSKIPFIX
copy ITE_EC.BIN %1
GOTO NBBKFIXDOK

:FAILED
move /Y *.MAP .\MAP
move /Y *.ABS .\MAP
move /Y *.HEX .\MAP
move /Y *.SBR .\MAP
move /Y *.BIN ..\RELEASE

ECHO.
ECHO    ITE Embedded Controller Firmware Build Process
ECHO    !!! Build Failed !!! ...
IF "%4" == "N"  GOTO DONE
ECHO.
PAUSE
GOTO FAILEDDONE



:OPTIONS
ECHO.
ECHO    ITE Embedded Controller Firmware Build Process
ECHO    Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
ECHO.
ECHO    USAGE:  BUILD CLEAN (Clear All Complier Copies)
ECHO.
ECHO    USAGE:  BUILD [ITECFILENAME] [BIOSFILENAME] [PUASE] [PROC]
ECHO                  ITECFILENAME = Build EC Filename
ECHO                  BIOSFILENAME = Combine System BIOS Filename
ECHO                  PUASE = "Y" or "N" (Pause when building)
ECHO                  PROC = "NEW" (Build changed Code only)
ECHO                  PROC = "OEM" (Re-Build OEM Code)
ECHO                  PROC = "ALL" (Re-Build everythings)
ECHO                  PROC = "BBK" (Build Boot-Block Code only)
ECHO.
ECHO.

:DONE
cd ..

:FAILEDDONE