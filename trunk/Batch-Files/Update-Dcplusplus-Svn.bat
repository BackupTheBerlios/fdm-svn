@ECHO OFF
rem CONFIG START
rem -------------------------------------------------------------------------------
set RAR="%PROGRAMFILES%\winRar\rar.exe"

set DCSVN="original-dcplusplus-svn-files.rar"
set UNRARSVNFILES=x -r -idp -inul %DCSVN%
set RARSVNFILES=a -r -eH -ep1 -idp -inul -m3 %DCSVN% original-dcplusplus

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
rd /s /q original-dcplusplus

ECHO ------------------------------------------------------------------------
ECHO Extracting DC++ Svn's Files
%RAR% %UNRARSVNFILES%

ECHO ------------------------------------------------------------------------
ECHO Copying Files Over
attrib +h /S /D original-dcplusplus > nul
xcopy /q /y /e *.* original-dcplusplus\ > nul

ECHO ------------------------------------------------------------------------
ECHO Time To Update.  Look In original-dcplusplus Directory.
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Remove All Conflicts Before Proceeding
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Remove All Conflicts Before Proceeding
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Copying Files Back
attrib +h /S /D .svn > nul
attrib +h /S /D _svn > nul
xcopy /q /y /e original-dcplusplus\*.* > nul

ECHO ------------------------------------------------------------------------
ECHO Removing DC++ Files
del /q original-dcplusplus\*.* > nul
del /q original-dcplusplus\bzip2\*.* > nul
del /q original-dcplusplus\client\*.* > nul
del /q original-dcplusplus\help\*.* > nul
del /q original-dcplusplus\res\*.* > nul
del /q original-dcplusplus\stlport\*.*  > nul
del /q original-dcplusplus\stlport\config\*.*  > nul
del /q original-dcplusplus\stlport\stl\*.*  > nul
del /q original-dcplusplus\stlport\using\*.*  > nul
del /q original-dcplusplus\windows\*.* > nul
del /q original-dcplusplus\wtl\*.* > nul
del /q original-dcplusplus\yassl\*.* > nul
del /q original-dcplusplus\yassl\certs\*.* > nul
del /q original-dcplusplus\yassl\include\*.* > nul
del /q original-dcplusplus\yassl\include\openssl\*.* > nul
del /q original-dcplusplus\yassl\mySTL\*.* > nul
del /q original-dcplusplus\yassl\src\*.* > nul
del /q original-dcplusplus\yassl\taocrypt\*.* > nul
del /q original-dcplusplus\yassl\taocrypt\include\*.* > nul
del /q original-dcplusplus\yassl\taocrypt\src\*.* > nul
del /q original-dcplusplus\zlib\*.* > nul

ECHO ------------------------------------------------------------------------
ECHO Raring DC++'s Svn Structure
del /a /q %DCSVN% > nul
%RAR% %RARSVNFILES%

ECHO ------------------------------------------------------------------------
ECHO Cleaning Up . . .
rd /s /q original-dcplusplus > nul
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