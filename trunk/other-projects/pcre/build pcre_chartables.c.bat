@ECHO OFF
rem CONFIG START
rem -------------------------------------------------------------------------------
set NET="%PROGRAMFILES%\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat"
rem CONFIG END
rem -------------------------------------------------------------------------------
IF NOT EXIST "%PROGRAMFILES%\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat" (
	echo "visual studio could not be found."
	pause
	goto:eof
)
ECHO -------------------------------
ECHO Making Sure Path Is Clear
del pcre_chartables.c

ECHO -------------------------------
ECHO Making Chartables.c for PCRE...
call %NET%
cl -DSUPPORT_UTF8 -DSUPPORT_UCP dftables.c
dftables.exe pcre_chartables.c

ECHO -------------------------------
ECHO Cleaning up...
del dftables.exe
del dftables.obj

ECHO -------------------------------
ECHO finished!
pause