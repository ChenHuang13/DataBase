CREATE DATABASE orderDB;

USE orderDB;

CREATE TABLE publisher (
  id int(10) NOT NULL,
  month date(8) NOT NULL,
  cost float(10) NOT NULL
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
 (1, '20130115', 1.0);

INSERT INTO publisher VALUES 
 (2, '20130117', 2.1);

INSERT INTO publisher VALUES 
 (3, '20130119', 5.3);

INSERT INTO publisher VALUES 
 (4, '20130121', 2.5);

INSERT INTO publisher VALUES 
 (5, '20130123', 3.1);

SELECT  sum id from publisher WHERE id< 200020;
SELECT  max id from publisher WHERE id< 200020;
SELECT  min id from publisher WHERE id< 200020;
SELECT  average id from publisher WHERE id< 200020;


SELECT  month,cost from publisher WHERE id< 200020;

SELECT  average cost from publisher WHERE id < 200020;
DESC publisher;

