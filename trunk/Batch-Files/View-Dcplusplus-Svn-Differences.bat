@ECHO OFF

ECHO ---------------------------------------------------------------------------
ECHO Preparing . . .
cd..
attrib +h /S /D .svn > nul
attrib +h /S /D _svn > nul
attrib +h /S /D build > nul
attrib +h dcpp\stdinc.h.gch > nul
attrib +h win32\stdafx.h.gch > nul
attrib +h /S /D Batch-Files > nul
attrib +h /S /D Other-Projects > nul
attrib +h *.bat > nul
attrib +h Fdm*.* > nul
attrib +h dcpp\Fdm*.* > nul
attrib +h win32\Fdm*.* > nul
attrib +h res\Fdm*.* > nul
attrib +h res\merged_*.* > nul
attrib +h res\menu\Fdm*.* > nul
attrib +h Changelog-Fdm.txt > nul
attrib +h License-Fdm.txt > nul
attrib +h Example.xml > nul
attrib +h makedefs.py > nul
attrib +h dcpp\StringDefs.* > nul
attrib +h natupnp.h > nul
attrib +h dcpp\BCDC*.* > nul
attrib +h win32\BCDC*.* > nul
attrib +h fulDC*.* > nul
attrib +h dcpp/fulDC*.* > nul
attrib +h *.pyc > nul
attrib +h .sconsign.dblite > nul
attrib +h *.rar > nul

ECHO ---------------------------------------------------------------------------
ECHO Copying Files Over
xcopy /q /y /e /c *.* ..\original-dcplusplus\ > nul
attrib +h /S /D ..\original-dcplusplus\.svn > nul
attrib +h /S /D ..\original-dcplusplus\_svn > nul

ECHO ---------------------------------------------------------------------------
ECHO Cleaning Up . . .
attrib -h /S /D build > nul
attrib -h dcpp\stdinc.h.gch > nul
attrib -h win32\stdafx.h.gch > nul
attrib -h /S /D Batch-Files > nul
attrib -h /S /D Other-Projects > nul
attrib -h *.bat > nul
attrib -h Fdm*.* > nul
attrib -h dcpp\Fdm*.* > nul
attrib -h win32\Fdm*.* > nul
attrib -h res\Fdm*.* > nul
attrib -h res\merged_*.* > nul
attrib -h res\menu\Fdm*.* > nul
attrib -h Changelog-Fdm.txt > nul
attrib -h License-Fdm.txt > nul
attrib -h Example.xml > nul
attrib -h makedefs.py > nul
attrib -h dcpp\StringDefs.* > nul
attrib -h natupnp.h > nul
attrib -h dcpp\BCDC*.* > nul
attrib -h win32\BCDC*.* > nul
attrib -h fulDC*.* > nul
attrib -h dcpp/fulDC*.* > nul
attrib -h *.pyc > nul
attrib -h .sconsign.dblite > nul
attrib -h *.rar > nul
