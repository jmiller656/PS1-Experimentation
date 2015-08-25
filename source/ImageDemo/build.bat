@echo off
cd C:\Users\Josh\Downloads\timexample\timexample
:make
psymake
echo Run again?
set /p yn="y/n: "
if %yn% == y (goto :make) else(pause)