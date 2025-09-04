@echo off

set CURDIR="%cd%"
set "CURDIR=%CURDIR:\=/%"

bash -lc "cd %CURDIR%; make all"

main.exe
