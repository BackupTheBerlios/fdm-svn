@ECHO OFF

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
rd /s /q Temp
xcopy /q /y /s *.* Temp\ > nul

ECHO ------------------------------------------------------------------------
ECHO Time To View.  Look In Temp Directory.
PAUSE

ECHO ------------------------------------------------------------------------
ECHO Cleaning Up . . .
rd /s /q Temp > nul
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