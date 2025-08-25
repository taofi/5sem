

CREATE TABLE VKS_table (
    id NUMBER PRIMARY KEY,
    name VARCHAR2(50)
);
commit;
INSERT INTO VKS_table (id, name) VALUES (1, 'dfsdg');
INSERT INTO VKS_table (id, name) VALUES (2, 'asfas');
INSERT INTO VKS_table (id, name) VALUES (3, 'asfasf');
commit;

select * from VKS_table

