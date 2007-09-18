@ECHO OFF
rem CONFIG START
rem -------------------------------------------------------------------------------
set RAR="%PROGRAMFILES%\winRar\rar.exe"
set RARNAME="fdm-src.rar"
set RARSRC=a -r -eH -ep1 -idp -inul -m3 %RARNAME% ..\Temp\*.*
rem CONFIG END
rem -------------------------------------------------------------------------------

IF NOT EXIST "%PROGRAMFILES%\WinRAR\Rar.exe" (
	echo "Winrar could not be found."
	pause
	goto:eof
)

IF EXIST "%RARNAME%" (
ECHO ------------------------------------------------------------------------
ECHO Deleting Old Source Rar
del %RARNAME% > nul
)

ECHO ------------------------------------------------------------------------
ECHO Preparing Source
cd..
attrib +h /S /D .svn > nul
attrib +h /S /D _svn > nul
attrib +h /S /D CVS > nul
attrib +h /S /D vc7 > nul
attrib +h /S /D app > nul
attrib +h /S *.ncb > nul
attrib +h /S *.suo > nul
attrib +h /S *.aps > nul
attrib +h /S *.user > nul
attrib +h *.rar > nul
rd /s /q Temp
xcopy /q /y /s *.* ..\Temp\ > nul

ECHO ------------------------------------------------------------------------
ECHO packing source...
%RAR% %RARSRC%

ECHO ------------------------------------------------------------------------
ECHO cleaning up
rd /s /q ..\Temp > nul
attrib -h /S /D CVS > nul
attrib -h /S /D vc7 > nul
attrib -h /S /D app > nul
attrib -h /S *.ncb > nul
attrib -h /S *.suo > nul
attrib -h /S *.aps > nul
attrib -h /S *.user > nul
attrib -h *.rar > nul

ECHO ------------------------------------------------------------------------
ECHO finished!
pause