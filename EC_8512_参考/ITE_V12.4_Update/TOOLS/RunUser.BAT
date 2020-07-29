@ECHO OFF
REM ####################################################################
REM #* Filename: RunUser.BAT
REM #* Function: The BATCH file for ITE Embedded Controller Firmware.
REM #* Notes: use with ** Keil uVision ** utility.
REM #*
REM #* Copyright (c) 2006-2008, ITE Tech. Inc. All Rights Reserved.
REM ####################################################################

SET OEMFOLDER=STDPRJ
SET BBKFOLDER=STDPRJ

SET BIOSFILENAME=TESTBIOS.ROM

SET ITECFILENAME=ITE_EC.001
SET ITECFILENAME2=ITE_EC.BIN

REM ####################################################################
REM #* START
REM ####################################################################

CD OBJ

COPY ..\..\..\..\BBLOCK\OEM\%BBKFOLDER%\uvBUILD\RELEASE\EC_BBK.*

..\..\..\..\TOOLS\HEX2BIN.EXE PROJECT.HEX

..\..\..\..\TOOLS\COMBECB.EXE EC_BBK.4KB PROJECT.BIN %ITECFILENAME%

MOVE /Y %ITECFILENAME% ..\RELEASE\

CD ..\RELEASE

COPY %ITECFILENAME% %ITECFILENAME2%

DIR
REM ####################################################################
REM #* END
