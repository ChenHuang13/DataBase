SELECT book.title,orders.quantity FROM book,orders WHERE book.id=orders.book_id AND orders.quantity>8;
SELECT book.title,orders.quantity,publisher.id FROM book,orders,publisher WHERE book.id=orders.book_id AND orders.quantity<3 AND orders.quantity=publisher.id;
