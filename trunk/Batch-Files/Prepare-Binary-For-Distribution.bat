@ECHO OFF
rem CONFIG START
rem -------------------------------------------------------------------------------
set RAR="%PROGRAMFILES%\winRar\rar.exe"

set RARNAME="fdm.rar"
set RARBIN=a -r -eH -ep1 -idp -inul -m3 RARNAME Temp\*.*

rem CONFIG END
rem -------------------------------------------------------------------------------

IF NOT EXIST "%PROGRAMFILES%\WinRAR\Rar.exe" (
	echo "Winrar could not be found."
	pause
	goto:eof
)

cd..

IF EXIST "%RARNAME%" (
	ECHO ------------------------------------------------------------------------
	ECHO Deleting Old Rar
	del /q %RARNAME% > nul
)

IF EXIST "..\updateGeoIP.py" (
	ECHO ------------------------------------------------------------------------
	ECHO updating Geo ip database...
	..\updateGeoIP.py
)

ECHO ------------------------------------------------------------------------
ECHO Make sure the path is clear

rd /s /q Temp > nul
md Temp > nul

ECHO ------------------------------------------------------------------------
ECHO copying files...
xcopy /H /Q /Y App\*.exe Temp\ > nul
xcopy /H /Q /Y App\*.pdb Temp\ > nul
xcopy /H /Q /Y App\*.chm Temp\ > nul
xcopy /H /Q /Y App\*.dll Temp\ > nul
xcopy /H /Q /Y *.dll Temp\ > nul
xcopy /H /Q /Y changelog*.txt Temp\ > nul
xcopy /H /Q /Y *.xml Temp\ > nul

IF EXIST "App\GeoIPCountryWhois.csv" (
	xcopy /H /Q /Y App\*.cvs Temp\ > nul
)

md Temp\Licenses > nul
xcopy /H /Q /Y License.txt Temp\Licenses > nul
xcopy /H /Q /Y License-Fdm.txt Temp\Licenses > nul
xcopy /H /Q /Y LICENSE-GeoIP.txt Temp\Licenses > nul
xcopy /H /Q /Y bzip2\LICENSE Temp\Licenses\License-bzip2.txt > nul
xcopy /H /Q /Y Other-Projects\pcre\LICENSE Temp\Licenses\License-pcre.txt > nul

ECHO ------------------------------------------------------------------------
ECHO packing executable...
%RAR% %RARBIN%

ECHO ------------------------------------------------------------------------
ECHO cleaning up
rd /s /q Temp > nul

ECHO ------------------------------------------------------------------------
ECHO finished!
pause