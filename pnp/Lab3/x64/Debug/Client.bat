@echo off
setlocal enabledelayedexpansion

rem Количество запусков
set count=10

rem Начальное значение параметра
set basePort=2000

for /l %%i in (0,1,%count%) do (
    set /a port=%basePort%+%%i
    echo Запуск ClientU.exe с параметром !port!
    start ClientU.exe !port!
)

endlocal
