@echo off
IF EXIST stlport\build\lib\configure.bat (
	echo Building STLPort
	cd stlport\build\lib
	call configure -c gcc
	make -fgcc.mak install-release-static
	make -fgcc.mak install-dbg-static
	make -fgcc.mak install-stldbg-static
        cd ..\..\..
)
