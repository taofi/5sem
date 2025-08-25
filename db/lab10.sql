







--zad1
ALTER TABLE TEACHER ADD (BIRTHDAY DATE, SALARY NUMBER);

UPDATE TEACHER SET BIRTHDAY = TO_DATE('1980-05-12', 'YYYY-MM-DD'), SALARY = 80000 WHERE TEACHER_NAME = 'Dr. John Doe';
UPDATE TEACHER SET BIRTHDAY = TO_DATE('1975-08-18', 'YYYY-MM-DD'), SALARY = 90000 WHERE TEACHER_NAME = 'Prof. Jane Smith';
UPDATE TEACHER SET BIRTHDAY = TO_DATE('1912-06-23', 'YYYY-MM-DD'), SALARY = 100000 WHERE TEACHER_NAME = 'Dr. Alan Turing';

--zad2
select * FROM TEACHER;
SELECT 
    REGEXP_SUBSTR(TEACHER_NAME, '^[^ ]+') AS LAST_NAME,
    SUBSTR(REGEXP_SUBSTR(TEACHER_NAME, ' [^ ]+'), 2, 1) || '.' AS FIRST_INITIAL,
    SUBSTR(REGEXP_SUBSTR(TEACHER_NAME, ' [^ ]+$'), 2, 1) || '.' AS MIDDLE_INITIAL
FROM TEACHER;

--zad3

SELECT TEACHER_NAME, BIRTHDAY
FROM TEACHER
WHERE TO_CHAR(BIRTHDAY, 'DY', 'NLS_DATE_LANGUAGE=ENGLISH') = 'MON';



--zad4
CREATE VIEW Teachers_Born_Next_Month AS
SELECT TEACHER_NAME, BIRTHDAY
FROM TEACHER
WHERE EXTRACT(MONTH FROM BIRTHDAY) = EXTRACT(MONTH FROM ADD_MONTHS(SYSDATE, 1));

SELECT * FROM Teachers_Born_Next_Month;
--zad5

CREATE VIEW Teachers_Born_Per_Month AS
SELECT EXTRACT(MONTH FROM BIRTHDAY) AS MONTH, COUNT(*) AS TEACHERS_COUNT
FROM TEACHER
GROUP BY EXTRACT(MONTH FROM BIRTHDAY)
ORDER BY MONTH;

SELECT * FROM Teachers_Born_Per_Month;

--zad6

DECLARE
    CURSOR cur_birthday_jubilee IS
    SELECT TEACHER_NAME, BIRTHDAY
    FROM TEACHER
    WHERE MOD(EXTRACT(YEAR FROM SYSDATE) + 1, 10) = MOD(EXTRACT(YEAR FROM BIRTHDAY), 10);
BEGIN
    FOR rec IN cur_birthday_jubilee LOOP
        DBMS_OUTPUT.PUT_LINE('Teacher: ' || rec.TEACHER_NAME || ', Birthday: ' || rec.BIRTHDAY);
    END LOOP;
END;
/

--zad7
DECLARE
    CURSOR cur_avg_salary IS
    SELECT PULPIT, FLOOR(AVG(SALARY)) AS AVG_SALARY
    FROM TEACHER
    GROUP BY PULPIT;
    
    CURSOR cur_avg_salary_faculty IS
    SELECT F.FACULTY_NAME, FLOOR(AVG(T.SALARY)) AS AVG_SALARY
    FROM TEACHER T
    JOIN PULPIT P ON T.PULPIT = P.PULPIT_NAME
    JOIN FACULTY F ON P.FACULTY = F.FACULTY
    GROUP BY F.FACULTY_NAME;
    
    CURSOR cur_avg_salary_total IS
    SELECT FLOOR(AVG(SALARY)) AS AVG_SALARY_TOTAL
    FROM TEACHER;
BEGIN
    -- По кафедрам
    FOR rec IN cur_avg_salary LOOP
        DBMS_OUTPUT.PUT_LINE('Pulpit: ' || rec.PULPIT || ', Average Salary: ' || rec.AVG_SALARY);
    END LOOP;
    
    -- По факультетам
    FOR rec IN cur_avg_salary_faculty LOOP
        DBMS_OUTPUT.PUT_LINE('Faculty: ' || rec.FACULTY_NAME || ', Average Salary: ' || rec.AVG_SALARY);
    END LOOP;
    
    -- По всем преподавателям
    FOR rec IN cur_avg_salary_total LOOP
        DBMS_OUTPUT.PUT_LINE('Total Average Salary: ' || rec.AVG_SALARY_TOTAL);
    END LOOP;
END;
/

--zad8

-- Создание типа записи
DECLARE
    TYPE Teacher_Record IS RECORD (
        TEACHER_NAME VARCHAR2(100),
        PULPIT_NAME VARCHAR2(100),
        BIRTHDAY DATE
    );
    
    -- Объявление переменной типа записи
    teacher_info Teacher_Record;
    
    -- Вложенные записи
    TYPE Faculty_Record IS RECORD (
        FACULTY_NAME VARCHAR2(100),
        TEACHERS Teacher_Record
    );
    
    faculty_info Faculty_Record;
BEGIN
    -- Присвоение значений переменной типа записи
    teacher_info.TEACHER_NAME := 'Dr. John Doe';
    teacher_info.PULPIT_NAME := 'Software Engineering';
    teacher_info.BIRTHDAY := TO_DATE('1980-05-12', 'YYYY-MM-DD');
    
    DBMS_OUTPUT.PUT_LINE('Teacher: ' || teacher_info.TEACHER_NAME || ', Pulpit: ' || teacher_info.PULPIT_NAME || ', Birthday: ' || teacher_info.BIRTHDAY);
    
    -- Присвоение вложенной записи
    faculty_info.FACULTY_NAME := 'Computer Science';
    faculty_info.TEACHERS := teacher_info;
    
    DBMS_OUTPUT.PUT_LINE('Faculty: ' || faculty_info.FACULTY_NAME || ', Teacher: ' || faculty_info.TEACHERS.TEACHER_NAME);
END;
/