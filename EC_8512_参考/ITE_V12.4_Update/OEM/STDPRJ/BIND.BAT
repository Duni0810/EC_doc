@ECHO OFF
REM ####################################################################
REM #* Filename: BIND.BAT
REM #* Function: The BATCH file for ITE Embedded Controller Firmware.
REM #* Notes: use with Microsoft NMAKER utility version 1.50.
REM #*
REM #* FIXEDOUTSIZE=Y (Fix 64KB Sizes)  FIXEDOUTSIZE=N (Real Sizes)
REM #* FIXEDOUTSIZE=128 (Fix 128KB Sizes)
REM #* Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
REM ####################################################################

SET OEMFOLDER=STDPRJ
SET BIOSFILENAME=EV_NB.ROM
SET ITECFILENAME=EC_V12_4.BIN
SET PUASE=N
SET FIXEDOUTSIZE=Y

DEL BUILD\ERROR.TXT
IF  "%1" == "ALL"  	GOTO BUILDALL
IF  "%1" == "all"  	GOTO BUILDALL
IF  "%1" == "BBK"  	GOTO BUILDBBK
IF  "%1" == "bbk"  	GOTO BUILDBBK
IF  "%1" == "OEM"  	GOTO BUILDOEM
IF  "%1" == "oem"  	GOTO BUILDOEM
IF  "%1" == "CLR"  	GOTO BUILDCLR
IF  "%1" == "clr"  	GOTO BUILDCLR
IF  "%1" == "err"  	GOTO BUILDERR
IF  "%1" == "ERR"  	GOTO BUILDERR
IF  "%1" == "nobbk"  	GOTO BUILDNOBBK
IF  "%1" == "NOBBK"  	GOTO BUILDNOBBK
IF  "%1" == ""     	GOTO BUILD
IF  "%1" == "?"    	GOTO OPTIONS
IF  "%1" == "/?"   	GOTO OPTIONS
GOTO  OPTIONS

:BUILDERR
CD BUILD
BUILD %ITECFILENAME% %BIOSFILENAME% %OEMFOLDER% %PUASE% ALL %FIXEDOUTSIZE% >ERROR.TXT
GOTO DONE

:BUILD
CD BUILD
REM   ##### %1 ########## %2 ########### %3 ##### %4 ## %5 ###### %6 ####
BUILD %ITECFILENAME% %BIOSFILENAME% %OEMFOLDER% %PUASE% NEW %FIXEDOUTSIZE%
GOTO DONE

:BUILDALL
IF "%2" == "nobbk" GOTO BUILDALLNOBBK
IF "%2" == "NOBBK" GOTO BUILDALLNOBBK
IF "%2" == "" GOTO BUILDALL1
GOTO OPTIONS

:BUILDALL1
CD BUILD
BUILD %ITECFILENAME% %BIOSFILENAME% %OEMFOLDER% %PUASE% ALL %FIXEDOUTSIZE%
GOTO DONE

:BUILDALLNOBBK
CD BUILD
BUILD %ITECFILENAME% %BIOSFILENAME% %OEMFOLDER% %PUASE% ALLNOBBK %FIXEDOUTSIZE%
GOTO DONE

:BUILDBBK
CD BUILD
BUILD %ITECFILENAME% %BIOSFILENAME% %OEMFOLDER% %PUASE% BBK %FIXEDOUTSIZE%
GOTO DONE

:BUILDOEM
CD BUILD
BUILD %ITECFILENAME% %BIOSFILENAME% %OEMFOLDER% %PUASE% OEM %FIXEDOUTSIZE%
GOTO DONE

:BUILDCLR
CD BUILD
CALL BUILD CLEAN
CD..
GOTO DONE

:BUILDNOBBK
CD BUILD
BUILD %ITECFILENAME% %BIOSFILENAME% %OEMFOLDER% %PUASE% NOBBK %FIXEDOUTSIZE%
GOTO DONE

:OPTIONS
ECHO.
ECHO    ITE Embedded Controller Firmware Build Process
ECHO    Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
ECHO.
ECHO    USAGE:  BIND  [P1] [P2]
ECHO                  P1 =  NULL 		(Build changed Code only)
ECHO                  P1 = "OEM" 		(Re-Build OEM Code)
ECHO                  P1 = "ERR" 		(Build code message to ERROR.TXT)
ECHO                  P1 = "ALL" 		(Re-Build everything)
ECHO                  P1 = "ALL" P2 = "NOBBK"(Re-Build EC code without BBK function)
ECHO                  P1 = "BBK" 		(Build Boot-Block Code only) 
ECHO                  P1 = "CLR" 		(Clean all Build Buffer)
ECHO                  P1 = "NOBBK"  		(Build EC code without BBK function)
ECHO                  P1 = "/?"  		(This Help Message)
ECHO.

:DONE