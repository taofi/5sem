@echo off
if "%1"=="" (
    echo 㪠��� ���७�� 䠩���
    exit /b
)

mkdir TXT

move *.%1 TXT\

echo �� .%1 䠩�� ��६�饭� � ��⠫�� TXT.
pause