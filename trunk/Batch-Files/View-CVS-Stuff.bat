@ECHO OFF

ECHO ------------------------------------------------------------------------
ECHO Preparing . . .
cd..
attrib +h /S /D Batch-Files
attrib +h /S /D Fdm-Client
attrib +h /S /D Fdm-Windows
attrib +h /S /D Other-Projects
attrib +h /S /D .svn
attrib +h /S /D _svn
attrib +h /S /D vc7
attrib +h /S /D app
attrib +h /S *.ncb
attrib +h /S *.suo
attrib +h /S *.aps
attrib +h Changelog-Fdm.txt
attrib +h Fdm-Example.xml
attrib +h License-Fdm.txt
attrib +h *.rar
rd /s /q Temp
xcopy /q /y /s *.* Temp\

ECHO ------------------------------------------------------------------------
ECHO Time To View.  Look In Temp Directory.
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Cleaning Up . . .
rd /s /q Temp
attrib -h /S /D Batch-Files
attrib -h /S /D Fdm-Client
attrib -h /S /D Fdm-Windows
attrib -h /S /D Other-Projects
attrib -h /S /D vc7
attrib -h /S /D app
attrib -h /S *.ncb
attrib -h /S *.suo
attrib -h /S *.aps
attrib -h Changelog-Fdm.txt
attrib -h Fdm-Example.xml
attrib -h License-Fdm.txt
attrib -h *.rar