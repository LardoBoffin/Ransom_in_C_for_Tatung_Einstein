SET "DISCTOOLS=C:\Einstein\DiscTools"
SET PYTHON_BIN="C:\Users\Lardo Boffin\AppData\Local\Programs\Python\Python314\python.exe"

rem %DISCTOOLS%\einstein_dsk_extract_v1.3.py --help || goto :error
%PYTHON_BIN% %DISCTOOLS%\mfi2dsk_v1.2.py SITC.MFI || goto :error
%PYTHON_BIN% %DISCTOOLS%\einstein_dsk_extract_v1.3.py  extract SITC.dsk SAVEM.DAT || goto :error 

goto EOF

:error
timeout /t 30
exit /b %errorlevel%

:EOF
timeout /t 60