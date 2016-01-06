CREATE DATABASE orderDB;

USE orderDB;

CREATE TABLE publisher (
  id int(10) NOT NULL
);

CREATE TABLE book (
  id int(10) NOT NULL,
  title varchar(100) NOT NULL,
  authors varchar(200),
  publisher_id int(10) NOT NULL,
  copies int(10),
  PRIMARY KEY  (id)
);

CREATE TABLE customer (
  id int(10) NOT NULL,
  name varchar(25) NOT NULL,
  rank int(10) NOT NULL,
  PRIMARY KEY  (id)
);

CREATE TABLE orders (
  customer_id int(10) NOT NULL,
  book_id int(10) NOT NULL,
  quantity int(10) NOT NULL
); 
CREATE INDEX orders(quantity);


INSERT INTO publisher VALUES 
 (1);

INSERT INTO publisher VALUES 
 (2);

INSERT INTO publisher VALUES 
 (3);

INSERT INTO publisher VALUES 
 (4);

INSERT INTO publisher VALUES 
 (5);

SELECT  sum id from publisher WHERE id< 200020;
SELECT  max id from publisher WHERE id< 200020;
SELECT  min id from publisher WHERE id< 200020;
SELECT  average id from publisher WHERE id< 200020;