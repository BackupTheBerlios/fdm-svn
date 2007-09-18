@ECHO OFF
rem CONFIG START
rem -------------------------------------------------------------------------------
set RAR="%PROGRAMFILES%\winRar\rar.exe"

set DCSVN="original-dcplusplus-svn-files.rar"
set UNRARSVNFILES=x -r -idp -inul "..\%DCSVN%" "..\"
set RARSVNFILES=a -r -eH -ep1 -idp -inul -m3 "..\%DCSVN%" "..\original-dcplusplus"

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
attrib +h /S /D Batch-Files > nul
attrib +h /S /D Fdm-Client > nul
attrib +h /S /D Fdm-Windows > nul
attrib +h /S /D Fdm-Res > nul
attrib +h /S /D Other-Projects > nul
attrib +h /S /D .svn > nul
attrib +h /S /D _svn > nul
attrib +h /S /D vc7 > nul
attrib +h /S /D app > nul
attrib +h /S *.ncb > nul
attrib +h /S *.suo > nul
attrib +h /S *.aps > nul
attrib +h Fdm-*.* > nul
attrib +h Changelog-Fdm.txt > nul
attrib +h License-Fdm.txt > nul
attrib +h *.rar > nul
rd /s /q ..\original-dcplusplus < nul

ECHO ------------------------------------------------------------------------
ECHO Extracting DC++ Svn's Files
%RAR% %UNRARSVNFILES%

ECHO ------------------------------------------------------------------------
ECHO Copying Files Over
xcopy /q /y /e *.* ..\original-dcplusplus\ > nul

ECHO ------------------------------------------------------------------------
ECHO Time To Update.  Look In ..\original-dcplusplus Directory.
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Remove All Conflicts Before Proceeding
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Remove All Conflicts Before Proceeding
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Copying Files Back
attrib +h /S /D ..\original-dcplusplus\.svn > nul
attrib +h /S /D ..\original-dcplusplus\_svn > nul
xcopy /q /y /e ..\original-dcplusplus\*.* > nul

ECHO ------------------------------------------------------------------------
ECHO Removing DC++ Files
del /q /s ..\original-dcplusplus\*.* > nul

ECHO ------------------------------------------------------------------------
ECHO Raring DC++'s Svn Structure
del /a /q "..\%DCSVN%" > nul
%RAR% %RARSVNFILES%

ECHO ------------------------------------------------------------------------
ECHO Cleaning Up . . .
rd /s /q ..\original-dcplusplus > nul
attrib -h /S /D Batch-Files > nul
attrib -h /S /D Fdm-Client > nul
attrib -h /S /D Fdm-Windows > nul
attrib -h /S /D Fdm-Res > nul
attrib -h /S /D Other-Projects > nul
attrib -h /S /D vc7 > nul
attrib -h /S /D app > nul
attrib -h /S *.ncb > nul
attrib -h /S *.suo > nul
attrib -h /S *.aps > nul
attrib -h Fdm-*.* > nul
attrib -h Changelog-Fdm.txt > nul
attrib -h License-Fdm.txt > nul
attrib -h *.rar > nul

IF EXIST "C:\Shared On C\Bot Share\fdm\%DCSVN%" (
	ECHO ------------------------------------------------------------------------
	ECHO Copying DC++ SVN Directory Rar Files To Share
	copy /Y "..\%DCSVN%" "C:\Shared\Bot Share\fdm\" > nul
	pause
	goto:eof
)
