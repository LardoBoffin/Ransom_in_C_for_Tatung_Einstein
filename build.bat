rem this batch file needs to be run from the folder where the source C file is placed

rem set some base file paths
SET "SOURCE_DIR=C:\z88dk\examples\tatung\Ransom"
SET "MAME=C:\MAME\ES-DE\Emulators\MAME"
SET "ROMS=C:\MAME\ES-DE\Roms\einstein\EinTK02"
SET "DISCTOOLS=C:\Einstein\DiscTools"

rem compile program
zcc +cpm -lm -leinstein -Iinclude -DAMALLOC -o RANSOM.COM main.c  || goto :error
rem zcc +cpm -subtype=einstein -compiler=sdcc -O3 --max-allocs-per-node200000 -pragma-define:CRT_HEAP_AMALLOC=1 main.c -o RANSOM.COM -create-app || goto :error

rem C:\Einstein\DiscTools\einstein_dsk_v1.6.py -h

rem copy the source dos disk to the current folder to be populated
copy /Y "%SOURCE_DIR%\Source\RANSOM.dsk" "%SOURCE_DIR%" || goto :error

rem create a disc
rem C:\Einstein\DiscTools\einstein_dsk_v1.6.py create RANSOM.DSK || goto :error

rem add the program(s) and any data files etc.
%DISCTOOLS%\einstein_dsk_v1.6.py add RANSOM.DSK RANSOM.COM || goto :error
%DISCTOOLS%\einstein_dsk_v1.6.py add RANSOM.DSK NEWGAME.DAT || goto :error
rem C:\Einstein\DiscTools\einstein_dsk_v1.6.py add RANSOM.DSK DATAM.DAT || goto :error
rem C:\Einstein\DiscTools\einstein_dsk_v1.6.py add RANSOM.DSK IDXM.DAT || goto :error
%DISCTOOLS%\einstein_dsk_v1.6.py add RANSOM.DSK Data || goto :error

rem make the boot disc autoboot the disk we have just created
%DISCTOOLS%\einstein_dsk_v1.6.py add RANSOM.DSK --autorun "RANSOM" || goto :error

rem convert to MFI
%DISCTOOLS%\dsk2mfi_v1.1.py RANSOM.DSK || goto :error

rem boot mame passing in the boot disc to flop 1 and the program disc to flop 2
rem %MAME%\mame.exe einstein -uimodekey 7_PAD -inipath "%MAME%" -cfg_directory "%MAME%\cfg\einstein\EinTK02\btp" -debug -nowindow -pipe tk02 -skip_gameinfo -rompath "%ROMS%"  -flop1 "%SOURCE_DIR%\RANSOM.mfi" -window  || goto :error
%MAME%\mame.exe einstein -uimodekey 7_PAD -inipath "%MAME%" -cfg_directory "%MAME%\cfg\einstein\EinTK02\btp" -nowindow -skip_gameinfo -rompath "%ROMS%"  -flop1 "%SOURCE_DIR%\RANSOM.mfi" -window  || goto :error

goto EOF

:error
timeout /t 30
exit /b %errorlevel%

:EOF