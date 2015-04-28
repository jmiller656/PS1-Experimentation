@echo off
cd C:\Users\Josh\Downloads\Projects\source\TicTacToe
:make
psymake
echo Run again?
set /p yn="y/n: "
if %yn% == y (goto :make) else(pause)