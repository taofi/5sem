


--zad1

BEGIN
    NULL;
END;
/

--zad2
BEGIN
    DBMS_OUTPUT.PUT_LINE('Hello World!');
END;
/

--zad3
BEGIN
    DECLARE
        num NUMBER := 10;
        denom NUMBER := 0;
        result NUMBER;
    BEGIN
        result := num / denom;  -- Здесь возникнет ошибка
    EXCEPTION
        WHEN OTHERS THEN
            DBMS_OUTPUT.PUT_LINE('Произошла ошибка!');
            DBMS_OUTPUT.PUT_LINE('Код ошибки: ' || SQLCODE);
            DBMS_OUTPUT.PUT_LINE('Сообщение ошибки: ' || SQLERRM);
    END;
END;
/

--zad4
BEGIN
    DBMS_OUTPUT.PUT_LINE('Начало внешнего блока');
    DECLARE
            num2 NUMBER := 10;
            denom2 NUMBER := 0;
            result2 NUMBER;
    BEGIN
        DBMS_OUTPUT.PUT_LINE('Начало внутреннего блока');
        
        -- Преднамеренно вызовем ошибку деления на ноль
        DECLARE
            num NUMBER := 10;
            denom NUMBER := 0;
            result NUMBER;
        BEGIN
            result := num / denom;  -- Ошибка деления на ноль
        EXCEPTION
            WHEN ZERO_DIVIDE THEN
                DBMS_OUTPUT.PUT_LINE('Исключение во внутреннем блоке: деление на ноль');
        END;
        result2 := num2 / denom2; 
        DBMS_OUTPUT.PUT_LINE('Конец внутреннего блока');
        
    EXCEPTION
        WHEN OTHERS THEN
            DBMS_OUTPUT.PUT_LINE('Исключение в обработке внутреннего блока');
    END;
    
    DBMS_OUTPUT.PUT_LINE('Конец внешнего блока');

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Исключение во внешнем блоке');
END;
/

--zad5
SHOW parameter PLSQL_WARNINGS;

ALTER SESSION SET PLSQL_WARNINGS = 'ENABLE:INFORMATIONAL';

--zad6

select keyword from v_$reserved_words
    where length = 1 and keyword != 'A'
/

--zad7 

select keyword from v_$reserved_words
    where length > 1 and keyword != 'A' order by keyword
/

--zad8
SELECT NAME, VALUE, DESCRIPTION 
FROM V$PARAMETER
WHERE UPPER(NAME) LIKE '%PLSQL%'
ORDER BY NAME;

SHOW PARAMETER PLSQL;



--zad10
DECLARE
    -- Объявление переменных типа NUMBER
    num1 NUMBER := 10;
    num2 NUMBER := 25;
BEGIN
    -- Вывод значений инициализированных переменных
    DBMS_OUTPUT.PUT_LINE('Переменная num1 инициализирована значением: ' || num1);
    DBMS_OUTPUT.PUT_LINE('Переменная num2 инициализирована значением: ' || num2);
END;
/
--zad11


DECLARE
    -- Объявление и инициализация переменных
    num1 NUMBER := 15;
    num2 NUMBER := 2;
    sum_result NUMBER;
    diff_result NUMBER;
    mult_result NUMBER;
    div_result NUMBER;
    ost_result NUMBER;
BEGIN
    -- Арифметические операции
    sum_result := num1 + num2;       -- Сложение
    diff_result := num1 - num2;      -- Вычитание
    mult_result := num1 * num2;      -- Умножение
    div_result := num1 / num2;       -- Деление
    ost_result := MOD(num1, num2);
    -- Вывод результатов
    DBMS_OUTPUT.PUT_LINE('Результаты арифметических операций:');
    DBMS_OUTPUT.PUT_LINE('Сложение: ' || num1 || ' + ' || num2 || ' = ' || sum_result);
    DBMS_OUTPUT.PUT_LINE('Вычитание: ' || num1 || ' - ' || num2 || ' = ' || diff_result);
    DBMS_OUTPUT.PUT_LINE('Умножение: ' || num1 || ' * ' || num2 || ' = ' || mult_result);
    DBMS_OUTPUT.PUT_LINE('Деление: ' || num1 || ' / ' || num2 || ' = ' || div_result);
    DBMS_OUTPUT.PUT_LINE('Деление c остатком: ' || num1 || ' / ' || num2 || ' = ' || ost_result);

END;
/



--zad12

DECLARE
    -- Объявление переменной с фиксированной точкой
    fixed_point NUMBER(5, 2) := 123.45;  -- Число с двумя знаками после запятой
BEGIN
    DBMS_OUTPUT.PUT_LINE('Переменная с фиксированной точкой: ' || fixed_point);
END;
/

--zad13

DECLARE
    -- Объявление переменной с фиксированной точкой и отрицательным масштабом
    rounded_fixed_point NUMBER(5, -1) := 123.45;  -- Округление до десятков
BEGIN
    DBMS_OUTPUT.PUT_LINE('Переменная с отрицательным масштабом: ' || rounded_fixed_point);
END;
/

--zad14

DECLARE
    -- Объявление и инициализация переменной типа BINARY_FLOAT
    binary_float_var BINARY_FLOAT := 12.34f;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Переменная типа BINARY_FLOAT: ' || binary_float_var);
END;
/

--zad15

DECLARE
    -- Объявление и инициализация переменной типа BINARY_DOUBLE
    binary_double_var BINARY_DOUBLE := 1234.5678;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Переменная типа BINARY_DOUBLE: ' || binary_double_var);
END;
/


--zad16

DECLARE
    -- Объявление переменных с использованием экспоненциальной нотации
    scientific_notation_1 NUMBER := 1.23E4;  -- 1.23 * 10^4 = 12300
    scientific_notation_2 NUMBER := 5.67E-3; -- 5.67 * 10^-3 = 0.00567
BEGIN
    DBMS_OUTPUT.PUT_LINE('Переменная scientific_notation_1: ' || scientific_notation_1);
    DBMS_OUTPUT.PUT_LINE('Переменная scientific_notation_2: ' || scientific_notation_2);
END;
/

--zad17

DECLARE
    -- Объявление и инициализация переменных типа BOOLEAN
    is_active BOOLEAN := TRUE;
    is_inactive BOOLEAN := FALSE;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Переменная is_active: ' || CASE WHEN is_active THEN 'TRUE' ELSE 'FALSE' END);
    DBMS_OUTPUT.PUT_LINE('Переменная is_inactive: ' || CASE WHEN is_inactive THEN 'TRUE' ELSE 'FALSE' END);
END;
/


--zad18

DECLARE
    -- Объявление констант
    conststr       CONSTANT VARCHAR2(10) := 'LA'; 
    constnumber   CONSTANT NUMBER := 100;         
    constchar    CONSTANT CHAR(1) := 'D';       
BEGIN
    -- Демонстрация операций с константами
    DBMS_OUTPUT.PUT_LINE('conststr: ' || conststr);
    DBMS_OUTPUT.PUT_LINE('constnumber: ' || constnumber);
    DBMS_OUTPUT.PUT_LINE('constchar ' || constchar);
END;
/


--zad19

DECLARE
    -- Объявление переменных с использованием %TYPE
    v_auditorium_name  AUDITORIUM.AUDITORIUM_NAME%TYPE;
    v_faculty_name     FACULTY.FACULTY_NAME%TYPE;
BEGIN
    -- Присвоение значений
    SELECT AUDITORIUM_NAME INTO v_auditorium_name
    FROM AUDITORIUM WHERE AUDITORIUM_NAME = 'A101';
    SELECT FACULTY_NAME INTO v_faculty_name 
    FROM FACULTY WHERE FACULTY = 'CS';

    -- Демонстрация работы с переменными
    DBMS_OUTPUT.PUT_LINE('Аудитория: ' || v_auditorium_name);
    DBMS_OUTPUT.PUT_LINE('Факультет: ' || v_faculty_name);
END;
/



--zad20
DECLARE
    -- Объявление переменной типа строки с использованием %ROWTYPE
    v_auditorium_record AUDITORIUM%ROWTYPE;
BEGIN
    -- Присвоение значения целой строки
    SELECT * INTO v_auditorium_record FROM AUDITORIUM WHERE AUDITORIUM_NAME = 'P202';

    -- Демонстрация работы с полями записи
    DBMS_OUTPUT.PUT_LINE('Аудитория: ' || v_auditorium_record.AUDITORIUM_NAME);
    DBMS_OUTPUT.PUT_LINE('Вместимость: ' || v_auditorium_record.AUDITORIUM_CAPACITY);
    DBMS_OUTPUT.PUT_LINE('Тип аудитории: ' || v_auditorium_record.AUDITORIUM_TYPE);
END;
/


--zad21
DECLARE
    v_score NUMBER := 75;
BEGIN
    -- Простой оператор IF
    IF v_score >= 90 THEN
        DBMS_OUTPUT.PUT_LINE('Оценка: A');
    ELSIF v_score >= 80 THEN
        DBMS_OUTPUT.PUT_LINE('Оценка: B');
    ELSIF v_score >= 70 THEN
        DBMS_OUTPUT.PUT_LINE('Оценка: C');
    ELSE
        DBMS_OUTPUT.PUT_LINE('Оценка: F');
    END IF;

    -- IF без ELSE
    IF v_score >= 60 THEN
        DBMS_OUTPUT.PUT_LINE('Прошел!');
    END IF;
END;
/

--zad23
DECLARE
    v_day NUMBER := 3;
    v_day_name VARCHAR2(10);
BEGIN
    CASE v_day
        WHEN 1 THEN v_day_name := 'Понедельник';
        WHEN 2 THEN v_day_name := 'Вторник';
        WHEN 3 THEN v_day_name := 'Среда';
        WHEN 4 THEN v_day_name := 'Четверг';
        WHEN 5 THEN v_day_name := 'Пятница';
        WHEN 6 THEN v_day_name := 'Суббота';
        WHEN 7 THEN v_day_name := 'Воскресенье';
        ELSE v_day_name := 'Некорректный день';
    END CASE;

    DBMS_OUTPUT.PUT_LINE('День недели: ' || v_day_name);
END;
/

--zad24
DECLARE
    v_counter NUMBER := 1;
BEGIN
    LOOP
        DBMS_OUTPUT.PUT_LINE('Итерация: ' || v_counter);
        v_counter := v_counter + 1;
        EXIT WHEN v_counter > 5;  -- Условие выхода
    END LOOP;
END;
/

--zad25
DECLARE
    v_counter NUMBER := 1;
BEGIN
    WHILE v_counter <= 5 LOOP
        DBMS_OUTPUT.PUT_LINE('Итерация: ' || v_counter);
        v_counter := v_counter + 1;
    END LOOP;
END;
/

--zad26

BEGIN
    FOR v_counter IN 1..5 LOOP
        DBMS_OUTPUT.PUT_LINE('Итерация: ' || v_counter);
    END LOOP;
END;
/
