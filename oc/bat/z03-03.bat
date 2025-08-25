@echo off
if "%1"=="" (
    echo укажите расширение файлов
    exit /b
)

mkdir TXT

move *.%1 TXT\

echo Все .%1 файлы перемещены в каталог TXT.
pause