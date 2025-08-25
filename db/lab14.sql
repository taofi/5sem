BEGIN
    FOR rec IN (SELECT JOB FROM USER_JOBS) LOOP
        DBMS_JOB.REMOVE(rec.JOB);
    END LOOP;
END;
/



-- Таблица-источник
CREATE TABLE source_table (
    id NUMBER PRIMARY KEY,
    data VARCHAR2(100)
);

-- Таблица-назначение
CREATE TABLE target_table (
    id NUMBER PRIMARY KEY,
    data VARCHAR2(100)
);

-- Таблица для лога заданий
CREATE TABLE job_log (
    log_id NUMBER PRIMARY KEY,
    job_id NUMBER,
    status VARCHAR2(20),
    run_date DATE,
    message VARCHAR2(4000)
);

INSERT INTO source_table (id, data)
VALUES (1, 'Test Data 1');

INSERT INTO source_table (id, data)
VALUES (2, 'Test Data 2');

INSERT INTO source_table (id, data)
VALUES (3, 'Test Data 3');

INSERT INTO source_table (id, data)
VALUES (101, 'Ignore Data 1');

COMMIT;


DELETE FROM source_table;
COMMIT;

DELETE FROM target_table;
COMMIT;

DELETE FROM job_log;
COMMIT;


select * from job_log;


CREATE SEQUENCE job_log_seq
START WITH 1
INCREMENT BY 1
NOCACHE;


CREATE OR REPLACE PROCEDURE copy_and_delete_data IS
    l_status VARCHAR2(20);
    l_message VARCHAR2(4000);
BEGIN
    BEGIN
        INSERT INTO target_table (id, data)
        SELECT id, data FROM source_table WHERE id < 100;

        DELETE FROM source_table WHERE id < 100;

        l_status := 'SUCCESS';
        l_message := 'Data copied and deleted successfully';
    EXCEPTION
        WHEN OTHERS THEN
            l_status := 'FAILED';
            l_message := SQLERRM;
    END;

    INSERT INTO job_log (log_id, job_id, status, run_date, message)
    VALUES (job_log_seq.NEXTVAL, job_management.JOB, l_status, SYSDATE, l_message);

    COMMIT;
END;
/



CREATE OR REPLACE PACKAGE job_management IS
    JOB NUMBER;

    PROCEDURE schedule_job(start_time VARCHAR2);
    PROCEDURE run_now;
    PROCEDURE stop_job;
    PROCEDURE cancel_job;
    FUNCTION is_running RETURN BOOLEAN;
END;
/

CREATE OR REPLACE PACKAGE BODY job_management IS

    PROCEDURE schedule_job(start_time VARCHAR2) IS
    BEGIN
        -- Планирование задания
        DBMS_JOB.SUBMIT(
            job        => JOB,
            what       => 'BEGIN copy_and_delete_data; END;',
            next_date  => TO_DATE(start_time, 'YYYY-MM-DD HH24:MI:SS'),
            interval   => 'TRUNC(SYSDATE + 7)' -- Раз в неделю
        );
        COMMIT;
    END;

    PROCEDURE run_now IS
    BEGIN
        -- Немедленный запуск
        IF JOB IS NOT NULL THEN
            DBMS_JOB.RUN(JOB);
        ELSE
            RAISE_APPLICATION_ERROR(-20001, 'Job is not scheduled.');
        END IF;
    END;

    PROCEDURE stop_job IS
    BEGIN
        -- Остановка задания
        IF JOB IS NOT NULL THEN
            DBMS_JOB.BROKEN(JOB, TRUE);
            COMMIT;
        ELSE
            RAISE_APPLICATION_ERROR(-20002, 'Job is not scheduled.');
        END IF;
    END;

    PROCEDURE cancel_job IS
    BEGIN
        -- Удаление задания
        IF JOB IS NOT NULL THEN
            DBMS_JOB.REMOVE(JOB);
            JOB := NULL; -- Сброс значения
            COMMIT;
        ELSE
            RAISE_APPLICATION_ERROR(-20003, 'Job is not scheduled.');
        END IF;
    END;

    FUNCTION is_running RETURN BOOLEAN IS
        l_running NUMBER;
    BEGIN
        -- Проверка выполнения задания
        IF JOB IS NOT NULL THEN
            SELECT COUNT(*)
            INTO l_running
            FROM USER_JOBS
            WHERE JOB = JOB AND BROKEN = 'N';

            RETURN l_running > 0;
        ELSE
            RETURN FALSE;
        END IF;
    END;

END;
/




EXEC job_management.schedule_job(TO_CHAR(TRUNC(SYSDATE + 1) + (10 / 24), 'YYYY-MM-DD HH24:MI:SS'));


SELECT JOB, WHAT, NEXT_DATE, INTERVAL, BROKEN
FROM USER_JOBS;

select * from job_log;

EXEC job_management.run_now;

SELECT * FROM source_table;
SELECT * FROM target_table;


EXEC job_management.stop_job;
EXEC job_management.cancel_job;














CREATE OR REPLACE PACKAGE scheduler_management IS
    PROCEDURE schedule_job(start_time IN VARCHAR2);
    PROCEDURE run_now;
    PROCEDURE stop_job;
    PROCEDURE cancel_job;
    FUNCTION is_running RETURN BOOLEAN;
END;
/

CREATE OR REPLACE PACKAGE BODY scheduler_management IS

    PROCEDURE schedule_job(start_time IN VARCHAR2) IS
    BEGIN
        -- Создание задания
        DBMS_SCHEDULER.CREATE_JOB(
            job_name        => 'COPY_DATA_JOB',
            job_type        => 'PLSQL_BLOCK',
            job_action      => 'BEGIN copy_and_delete_data; END;',
            start_date      => TO_DATE(start_time, 'YYYY-MM-DD HH24:MI:SS'),
            repeat_interval => 'FREQ=WEEKLY', -- Раз в неделю
            enabled         => TRUE
        );
    END;

    PROCEDURE run_now IS
    BEGIN
        -- Немедленный запуск задания
        DBMS_SCHEDULER.RUN_JOB('COPY_DATA_JOB');
    END;

    PROCEDURE stop_job IS
    BEGIN
        -- Приостановка выполнения задания
        DBMS_SCHEDULER.DISABLE('COPY_DATA_JOB');
    END;

    PROCEDURE cancel_job IS
    BEGIN
        -- Удаление задания
        DBMS_SCHEDULER.DROP_JOB('COPY_DATA_JOB');
    END;

    FUNCTION is_running RETURN BOOLEAN IS
        l_running NUMBER;
    BEGIN
        -- Проверка выполнения задания
        SELECT COUNT(*)
        INTO l_running
        FROM ALL_SCHEDULER_RUNNING_JOBS
        WHERE JOB_NAME = 'COPY_DATA_JOB';

        RETURN l_running > 0;
    END;

END;
/




EXEC scheduler_management.schedule_job(TO_CHAR(TRUNC(SYSDATE + 1) + (10 / 24), 'YYYY-MM-DD HH24:MI:SS'));

SELECT JOB_NAME, START_DATE, STATE FROM USER_SCHEDULER_JOBS WHERE JOB_NAME = 'COPY_DATA_JOB';

EXEC scheduler_management.run_now;

SELECT * FROM target_table;

SELECT * FROM source_table;

SELECT * FROM job_log;

EXEC scheduler_management.stop_job;

SELECT JOB_NAME, STATE FROM USER_SCHEDULER_JOBS WHERE JOB_NAME = 'COPY_DATA_JOB';

EXEC scheduler_management.cancel_job;

SELECT * FROM USER_SCHEDULER_JOBS WHERE JOB_NAME = 'COPY_DATA_JOB';

