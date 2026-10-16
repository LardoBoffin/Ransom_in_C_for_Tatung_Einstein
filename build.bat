rem this batch file needs to be run from the folder where the source C file is placed

rem set some base file paths
SET "SOURCE_DIR=C:\z88dk\examples\tatung\SITC"
SET "MAME=C:\MAME\ES-DE\Emulators\MAME"
SET "ROMS=C:\MAME\ES-DE\Roms\einstein\EinTK02"
SET "DISCTOOLS=C:\Einstein\DiscTools"
SET PYTHON_BIN="C:\Users\Lardo Boffin\AppData\Local\Programs\Python\Python314\python.exe"

rem compile program
zcc +cpm -lm -leinstein -Iinclude -DAMALLOC -o SITC.COM main.c  || goto :error

rem zcc +cpm -compiler=sdcc -SO3 --max-allocs-per-node200000 -Iinclude -pragma-define:CRT_HEAP_AMALLOC=1 -pragma-define:CLIB_EXIT_STACK_SIZE=0 -pragma-define:nostreams -pragma-define:nofileio -create-app -oSITC main.c

rem zcc +cpm -subtype=einstein -compiler=sdcc -O3 --max-allocs-per-node200000 -pragma-define:CRT_HEAP_AMALLOC=1 main.c -o SITC.COM -create-app || goto :error

rem C:\Einstein\DiscTools\einstein_dsk_v1.6.py -h

rem copy the source dos disk to the current folder to be populated
copy /Y "%SOURCE_DIR%\Source\SITC.dsk" "%SOURCE_DIR%" || goto :error

rem create a disc
rem C:\Einstein\DiscTools\einstein_dsk_v1.6.py create SITC.DSK || goto :error

rem add the program(s) and any data files etc.
%PYTHON_BIN% %DISCTOOLS%\einstein_dsk_v1.6.py add SITC.DSK SITC.COM || goto :error
%PYTHON_BIN% %DISCTOOLS%\einstein_dsk_v1.6.py add SITC.DSK NEWGAME.DAT || goto :error
%PYTHON_BIN% %DISCTOOLS%\einstein_dsk_v1.6.py add SITC.DSK SAVEM.DAT || goto :error
rem C:\Einstein\DiscTools\einstein_dsk_v1.6.py add SITC.DSK DATAM.DAT || goto :error
rem C:\Einstein\DiscTools\einstein_dsk_v1.6.py add SITC.DSK IDXM.DAT || goto :error
%PYTHON_BIN% %DISCTOOLS%\einstein_dsk_v1.6.py add SITC.DSK Data || goto :error

rem make the boot disc autoboot the disk we have just created
%PYTHON_BIN% %DISCTOOLS%\einstein_dsk_v1.6.py add SITC.DSK --autorun "SITC" || goto :error

rem convert to MFI
%PYTHON_BIN% %DISCTOOLS%\dsk2mfi_v1.1.py SITC.DSK || goto :error

rem boot mame passing in the boot disc to flop 1 and the program disc to flop 2
rem %MAME%\mame.exe einstein -uimodekey 7_PAD -inipath "%MAME%" -cfg_directory "%MAME%\cfg\einstein\EinTK02\btp" -debug -nowindow -pipe tk02 -skip_gameinfo -rompath "%ROMS%"  -flop1 "%SOURCE_DIR%\SITC.mfi" -window  || goto :error
%MAME%\mame.exe einstein -uimodekey 7_PAD -inipath "%MAME%" -cfg_directory "%MAME%\cfg\einstein\EinTK02\btp" -nowindow -skip_gameinfo -rompath "%ROMS%"  -flop1 "%SOURCE_DIR%\SITC.mfi" -window  || goto :error

goto EOF

:error
timeout /t 30
exit /b %errorlevel%

:EOF