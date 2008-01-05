@ECHO OFF
rem CONFIG START
rem -------------------------------------------------------------------------------
set RAR="%PROGRAMFILES%\winRar\rar.exe"

set DCSVN="original-dcplusplus-svn-files.rar"
set UNRARSVNFILES=x -r -idp -inul "..\%DCSVN%" "..\"

rem CONFIG END
rem -------------------------------------------------------------------------------

IF NOT EXIST "%PROGRAMFILES%\WinRAR\Rar.exe" (
	echo "Winrar could not be found."
	pause
	goto:eof
)

ECHO ------------------------------------------------------------------------
ECHO Preparing . . .
cd..
attrib +h /S /D "bleh these files need manually merging" > nul
attrib +h /S /D build > nul
attrib +h /S /D app > nul
attrib +h /S /D Batch-Files > nul
attrib +h /S /D Fdm-Client > nul
attrib +h /S /D Fdm-Windows > nul
attrib +h /S /D Fdm-Res > nul
attrib +h /S /D Other-Projects > nul
attrib +h /S /D .svn > nul
attrib +h /S /D _svn > nul
attrib +h Fdm-*.* > nul
attrib +h Changelog-Fdm.txt > nul
attrib +h License-Fdm.txt > nul
attrib +h build_stl.bat > nul
attrib +h natupnp.h > nul
attrib +h Example.xml > nul
attrib +h *.rar > nul
rd /s /q ..\original-dcplusplus > nul

ECHO ------------------------------------------------------------------------
ECHO Extracting DC++ Svn's Files
%RAR% %UNRARSVNFILES%

ECHO ------------------------------------------------------------------------
ECHO Copying Files Over
xcopy /q /y /e *.* ..\original-dcplusplus > nul

ECHO ------------------------------------------------------------------------
ECHO Time To View.  Look In ..\original-dcplusplus Directory.
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Cleaning Up . . .
rd /s /q ..\original-dcplusplus > nul
attrib -h /S /D "bleh these files need manually merging" > nul
attrib -h /S /D build > nul
attrib -h /S /D app > nul
attrib -h /S /D Batch-Files > nul
attrib -h /S /D Fdm-Client > nul
attrib -h /S /D Fdm-Windows > nul
attrib -h /S /D Fdm-Res > nul
attrib -h /S /D Other-Projects > nul
attrib -h Fdm-*.* > nul
attrib -h Changelog-Fdm.txt > nul
attrib -h License-Fdm.txt > nul
attrib -h build_stl.bat > nul
attrib -h natupnp.h > nul
attrib -h Example.xml > nul
attrib -h *.rar > nul
