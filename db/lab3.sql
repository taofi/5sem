
CREATE TABLESPACE TS_VKS
DATAFILE 'TS_VKS.dbf'
SIZE 7M
AUTOEXTEND ON
NEXT 5M
MAXSIZE 20M;

CREATE TEMPORARY  TABLESPACE TS_VKS_TEMP
TEMPFILE 'TS_VKS_TEMP.dbf'
SIZE 5M
AUTOEXTEND ON
NEXT 3M
MAXSIZE 30M;

SELECT TABLESPACE_NAME, STATUS
FROM DBA_TABLESPACES;

CREATE ROLE RL_VKSCORE;

grant connect to RL_VKSCORE;
GRANT 
 CREATE TABLE,
 CREATE PROCEDURE,
 CREATE TRIGGER,
 CREATE VIEW
TO  RL_VKSCORE;
GRANT DELETE ANY TABLE TO RL_VKSCORE;
GRANT DROP ANY TABLE TO RL_VKSCORE;
GRANT DROP ANY PROCEDURE TO RL_VKSCORE;
GRANT DROP ANY TRIGGER TO RL_VKSCORE;
GRANT DROP ANY VIEW TO RL_VKSCORE;

SELECT r.ROLE, p.PRIVILEGE
FROM DBA_ROLES r
JOIN DBA_SYS_PRIVS p ON r.ROLE = p.GRANTEE
WHERE r.ROLE = 'RL_VKSCORE';


CREATE PROFILE PL_VKSCORE LIMIT
    PASSWORD_LIFE_TIME 180
    SESSIONS_PER_USER 3
    FAILED_LOGIN_ATTEMPTS 7
    PASSWORD_LOCK_TIME 1
    PASSWORD_GRACE_TIME DEFAULT
    CONNECT_TIME 180
    IDLE_TIME 30;
    
SELECT * FROM dba_profiles;
SELECT * FROM dba_profiles WHERE profile = 'PL_VKSCORE';
SELECT * FROM dba_profiles WHERE profile = 'DEFAULT';

CREATE USER U1_VKS_PDB 
  IDENTIFIED BY pass1 
  DEFAULT TABLESPACE TS_VKS 
  TEMPORARY TABLESPACE TS_VKS_TEMP 
  PROFILE PL_VKSCORE 
  ACCOUNT UNLOCK;

GRANT SELECT ON v_$session TO U1_VKS_PDB;
GRANT RL_VKSCORE to U1_VKS_PDB;
ALTER USER U1_VKS_PDB QUOTA UNLIMITED ON TS_VKS;




SELECT tablespace_name, status
FROM dba_tablespaces; --табличные пространства

SELECT file_name, tablespace_name, bytes, status
FROM dba_data_files; -- Перманентные файлы

SELECT file_name, tablespace_name, bytes, status
FROM dba_temp_files; --Временные файлы

SELECT grantee AS role, privilege
FROM dba_sys_privs
WHERE grantee IN (SELECT role FROM dba_roles);--роли и выданные им привилегии

SELECT profile, resource_name, limit
FROM dba_profiles;--профили безопасности

SELECT username, account_status, default_tablespace, profile
FROM dba_users;--пользователи

SELECT grantee, granted_role
FROM dba_role_privs;-- назначенные роли


GRANT CREATE SESSION TO C##YYY;
GRANT SELECT_CATALOG_ROLE TO U1_VKS_PDB;

SELECT s.sid, s.serial#, s.username, s.machine, s.program, p.name AS pdb_name
FROM v$session s
LEFT JOIN v$pdbs p ON s.con_id = p.con_id
WHERE s.type = 'USER';