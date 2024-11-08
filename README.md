# CRM-Web-Application
A Customer Relationship Management (CRM) system built with C++, Crow framework, and MySQL.

## Features

- Create, Read, Update, and Delete customer records
- Responsive web interface
- Real-time updates
- Database persistence

## Technologies Used

- C++ for backend
- Crow framework for REST API
- MySQL X DevAPI for database operations
- HTML/JavaScript/Tailwind CSS for frontend

## Prerequisites

- Visual Studio 2022
- vcpkg package manager
- MySQL Server
- C++17 or later

## Required Libraries

- Crow
- MySQL Connector/C++
- nlohmann-json

## Setup

1. Clone the repository
```bash
git clone https://github.com/username/CRM-Web-Application.git
```

2. Install dependencies using vcpkg:
```bash
vcpkg install crow:x64-windows
vcpkg install mysql-connector-cpp:x64-windows
vcpkg install nlohmann-json:x64-windows
```

3. Create the database:
```sql
CREATE DATABASE crm_database;
USE crm_database;

CREATE TABLE customers (
    id INT AUTO_INCREMENT PRIMARY KEY,
    firstName VARCHAR(50) NOT NULL,
    lastName VARCHAR(50) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    phone VARCHAR(20) NOT NULL,
    company VARCHAR(100) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
```

4. Update database credentials in `main.cpp`

5. Build and run the project in Visual Studio 2022

## Usage

1. Start the server
2. Open `index.html` in a web browser
3. Use the form to add new customers
4. View, edit, and delete customers from the list

## Screenshots
[Add screenshots of your application here]
