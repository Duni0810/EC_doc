@ECHO OFF
REM ####################################################################
REM #* Filename: CLEAN.BAT
REM #* Function: The BATCH file for ITE Embedded Controller Firmware.
REM #*  		 Do this CLEAN for Backup source code reduce ZIP sizes.
REM #*			 Will Keep release folder.
REM #* Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
REM ####################################################################

SET OEMFOLDER1=STDPRJ
SET OEMFOLDER2=EV_VIA
SET OEMFOLDER3=NULL

ECHO ...OEM Folder-1 Clean Process...
del /Q OEM\%OEMFOLDER1%\BUILD\*.BIN
del /Q OEM\%OEMFOLDER1%\BUILD\*.64K
del /Q OEM\%OEMFOLDER1%\BUILD\OBJ\*.*
del /Q OEM\%OEMFOLDER1%\BUILD\LST\*.*
del /Q OEM\%OEMFOLDER1%\BUILD\SOURCE\*.*
del /Q OEM\%OEMFOLDER1%\BUILD\MAP\*.*

ECHO ...OEM Folder-2 Clean Process...
del /Q OEM\%OEMFOLDER2%\BUILD\*.BIN
del /Q OEM\%OEMFOLDER2%\BUILD\*.64K
del /Q OEM\%OEMFOLDER2%\BUILD\OBJ\*.*
del /Q OEM\%OEMFOLDER2%\BUILD\LST\*.*
del /Q OEM\%OEMFOLDER2%\BUILD\SOURCE\*.*
del /Q OEM\%OEMFOLDER2%\BUILD\MAP\*.*

ECHO ...OEM Folder-3 Clean Process...
del /Q OEM\%OEMFOLDER3%\BUILD\*.BIN
del /Q OEM\%OEMFOLDER3%\BUILD\*.64K
del /Q OEM\%OEMFOLDER3%\BUILD\OBJ\*.*
del /Q OEM\%OEMFOLDER3%\BUILD\LST\*.*
del /Q OEM\%OEMFOLDER3%\BUILD\SOURCE\*.*
del /Q OEM\%OEMFOLDER3%\BUILD\MAP\*.*

:END
ECHO.
ECHO    ITE Embedded Controller Firmware Clean Process
ECHO    Clean All Build Buffer .................. OK !
ECHO.

pause
