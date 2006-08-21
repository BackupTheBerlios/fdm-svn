@ECHO OFF
rem CONFIG START
rem -------------------------------------------------------------------------------
set NET="%PROGRAMFILES%\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.exe"

set OPTS=dcplusplus.sln /build release
set CLEAN=dcplusplus.sln /clean release

set BINARY="dcdm-svn.rar"
set SOURCE="dcdm-svn-src.rar"

rem CONFIG END
rem -------------------------------------------------------------------------------

IF NOT EXIST "%PROGRAMFILES%\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.exe" (
	echo "visual studio could not be found."
	pause
	goto:eof
)


ECHO ------------------------------------------------------------------------
ECHO Cleaning solution...
cd..
IF EXIST "app\DCPlusPlus.pdb" (
	del "app\*.pdb"
	ECHO Found Old PDB files.  Just deleted.
)

%NET% %CLEAN%

ECHO ------------------------------------------------------------------------
ECHO Compiling solution in release mode...
%NET% %OPTS%
ECHO Done!

ECHO ------------------------------------------------------------------------
ECHO finished!
pause