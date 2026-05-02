# Bank Management System — C++ Console Application 
Console-based C++ banking management system with client CRUD, transactions, login, user management, and role-based permissions using file-based storage.
 Features
- Client management: add, view, search, update, delete
- Transactions: deposit, withdraw, total balance report
- User login
- User management: add, update, delete, find, list
- Role-based access control using bitwise permissions
- File-based data persistence

## Technologies Used
- C++
- STL vectors and strings
- File I/O with fstream
- Structs and enums
- Bitwise permission flags
- Console UI

## Limitations
- Passwords are stored as plain text because this is a learning project
- Menu navigation currently uses recursive calls and will be refactored later
- Data is stored in text files rather than a database

## Future Improvements
- Refactor menu navigation into loop-based flow
- Convert procedural design into OOP classes
- Add password hashing
- Add transaction history
- Split code into header/source files

## Default Login

Username: Admin  
Password: 1234
