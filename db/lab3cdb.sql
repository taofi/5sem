SELECT instance_number, instance_name, host_name, status, version
FROM gv$instance; --перечень экземпл€ров

SELECT comp_id, comp_name, version, status
FROM dba_registry;  --перечень установленных компонентов


CREATE USER C##VKS IDENTIFIED by pass;
GRANT CREATE SESSION TO C##VKS;
show pdbs


SELECT username, sid, status
FROM v$session
WHERE username IS NOT NULL;

SELECT s.sid, s.serial#, s.username, s.machine, s.program, p.name AS pdb_name
FROM v$session s
LEFT JOIN v$pdbs p ON s.con_id = p.con_id
WHERE s.type = 'USER';

show pdbs
ALTER PLUGGABLE DATABASE VKS_PDB OPEN READ WRITE;

ALTER PLUGGABLE DATABASE ORCLDB2 CLOSE IMMEDIATE;
DROP PLUGGABLE DATABASE ORCLPDB2 INCLUDING DATAFILES;