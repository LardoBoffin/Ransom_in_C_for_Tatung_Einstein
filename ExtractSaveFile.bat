SET "DISCTOOLS=C:\Einstein\DiscTools"

rem %DISCTOOLS%\einstein_dsk_extract_v1.3.py --help || goto :error
%DISCTOOLS%\mfi2dsk_v1.2.py RANSOM.MFI || goto :error
%DISCTOOLS%\einstein_dsk_extract_v1.3.py  extract RANSOM.dsk SAVEM.DAT || goto :error 

goto EOF

:error
timeout /t 30
exit /b %errorlevel%

:EOF
timeout /t 60