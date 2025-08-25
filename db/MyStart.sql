CREATE TABLE VKS_t (
    x NUMBER(3) PRIMARY KEY,
    s VARCHAR2(50)
);

insert all
    into VKS_t(x, s) VALUES(4, 'ss')
    into VKS_t(x, s) values(1, 'fsssss')
    into VKS_t(x, s) values (2, 'dsa')
    select * from dual;
commit;
update VKS_t  set x = 10
where x = 4;

update VKS_t set s = 'new'
where x = 1;
commit;
select * from VKS_t;

SELECT *
FROM VKS_t
WHERE x > 1;

SELECT 
    COUNT(*) AS total_count,
    AVG(x) AS average_x,
    MAX(x) AS max_x,
    MIN(x) AS min_x
FROM VKS_t;

DELETE FROM VKS_t
WHERE x = 1; 
commit;


CREATE TABLE VKS_t1 (
    id NUMBER(3) PRIMARY KEY,
    vks_id NUMBER(3),
    description VARCHAR2(50),
    FOREIGN KEY (vks_id) REFERENCES VKS_t(x)
);

commit;

INSERT INTO VKS_t1 (id, vks_id, description) 
VALUES (1, 10, 'First'),
(2, 2, 'Second'),
(3, 2, 'Third');


COMMIT;

-- Левое соединение (LEFT JOIN)
SELECT 
    a.x, 
    a.s, 
    b.description
FROM 
    VKS_t a
LEFT JOIN 
    VKS_t1 b ON a.x = b.vks_id;

-- Правое соединение (RIGHT JOIN)
SELECT 
    a.x, 
    a.s, 
    b.description
FROM 
    VKS_t a
RIGHT JOIN 
    VKS_t1 b ON a.x = b.vks_id;

-- Внутреннее соединение (INNER JOIN)
SELECT 
    a.x, 
    a.s, 
    b.description
FROM 
    VKS_t a
INNER JOIN 
    VKS_t1 b ON a.x = b.vks_id;

drop table VKS_t1;
commit;
drop table VKS_t;
commit;

