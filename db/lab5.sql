--z1
SELECT * FROM v$sga;
--z2
SELECT POOL, SUM(BYTES) 
FROM v$sgastat
group by pool;
--z3
SELECT COMPONENT, GRANULE_SIZE, MAX_SIZE, CURRENT_SIZE 
FROM v$sga_dynamic_components
where current_size > 0;
--z4
SELECT CURRENT_SIZE 
FROM v$sga_dynamic_free_memory;
--z5
select value from v$parameter where name = 'sga_target';
select value from v$parameter where name = 'sga_min_size';
select value from v$parameter where name = 'sga_max_size';

--z6
SELECT COMPONENT, GRANULE_SIZE, MAX_SIZE, CURRENT_SIZE 
FROM v$sga_dynamic_components;
--z7
CREATE TABLE keep_table (
    id NUMBER PRIMARY KEY
) STORAGE (BUFFER_POOL KEEP);

SELECT SEGMENT_NAME, SEGMENT_TYPE, TABLESPACE_NAME, BUFFER_POOL 
FROM dba_segments
WHERE SEGMENT_NAME = 'KEEP_TABLE';
--z8
CREATE TABLE my_default_table (
    id NUMBER PRIMARY KEY
) STORAGE (BUFFER_POOL DEFAULT);
SELECT SEGMENT_NAME, SEGMENT_TYPE, TABLESPACE_NAME, BUFFER_POOL 
FROM dba_segments
WHERE SEGMENT_NAME = 'MY_DEFAULT_TABLE';

--z9
show parameter log_buffer;
--z10
show parameter large_pool;

--z11
SELECT * 
FROM v$session 
WHERE USERNAME IS NOT NULL;
--z12
SELECT NAME, DESCRIPTION 
FROM v$bgprocess
WHERE PADDR != hextoraw('00') order by name;
--z13
SELECT SPID, PROGRAM 
FROM v$process
WHERE BACKGROUND IS NULL;
--z14
SELECT COUNT(*) 
FROM v$process
WHERE PROGRAM LIKE '%DBW%';
--z15
SELECT NAME 
FROM v$active_services;
--z16
SELECT name, value 
FROM v$parameter
WHERE name LIKE 'dispatchers%';
--z20 lsnrctl services

