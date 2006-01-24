@ECHO OFF
rem CONFIG START
rem -------------------------------------------------------------------------------
set RAR="%PROGRAMFILES%\winRar\rar.exe"
set RARSRC=a -r -eH -ep1 -idp -inul -m3 fdm-src.rar Temp\*.*
rem CONFIG END
rem -------------------------------------------------------------------------------

IF NOT EXIST "%PROGRAMFILES%\WinRAR\Rar.exe" (
	echo "Winrar could not be found."
	pause
	goto:eof
)

ECHO ------------------------------------------------------------------------
ECHO Preparing Source
cd..
attrib +h /S /D .svn
attrib +h /S /D _svn
attrib +h /S /D CVS
attrib +h /S /D vc7
attrib +h /S /D app
attrib +h /S *.ncb
attrib +h /S *.suo
attrib +h /S *.aps
attrib +h *.rar
rd /s /q Temp
xcopy /q /y /s *.* Temp\

ECHO ------------------------------------------------------------------------
ECHO packing source...
%RAR% %RARSRC%

ECHO ------------------------------------------------------------------------
ECHO cleaning up
rd /s /q Temp
attrib -h /S /D CVS
attrib -h /S /D vc7
attrib -h /S /D app
attrib -h /S *.ncb
attrib -h /S *.suo
attrib -h /S *.aps
attrib -h *.rar

ECHO ------------------------------------------------------------------------
ECHO finished!
pause