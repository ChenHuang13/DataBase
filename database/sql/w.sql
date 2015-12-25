CREATE DATABASE orderDB;

USE orderDB;

CREATE TABLE publisher (
  id int(10) NOT NULL,
  name varchar(100) NOT NULL,
  nation varchar(3),
  PRIMARY KEY  (id)
);

INSERT INTO publisher VALUES (1, '0', '0');
INSERT INTO publisher VALUES (2, '0', '0');
INSERT INTO publisher VALUES (3, '0', '0');
INSERT INTO publisher VALUES (4, '0', '0');
INSERT INTO publisher VALUES (5, '0', '0');

SELECT id from publisher WHERE id< 200020;
