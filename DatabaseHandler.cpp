#include "DatabaseHandler.h"
#include <iostream>
#include <sstream>

DatabaseHandler::DatabaseHandler(const std::string& host, uint16_t port,
    const std::string& user, const std::string& password,
    const std::string& schema)
    : host(host), port(port), user(user), password(password), schema(schema), session(nullptr) {}

DatabaseHandler::~DatabaseHandler() {
    disconnect();
}

bool DatabaseHandler::connect() {
    try {
        if (session != nullptr) {
            disconnect();
        }

        std::stringstream connection_str;
        connection_str << user << ":" << password << "@" << host << ":" << port << "/" << schema;

        session = new mysqlx::Session(connection_str.str());
        return session != nullptr;
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Connection failed: " << err << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        return false;
    }
}

void DatabaseHandler::disconnect() {
    try {
        if (session != nullptr) {
            session->close();
            delete session;
            session = nullptr;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error during disconnect: " << e.what() << std::endl;
    }
}

bool DatabaseHandler::createCustomer(const Customer& customer) {
    if (!session) {
        std::cerr << "No database connection" << std::endl;
        return false;
    }

    try {
        auto table = session->getSchema(schema).getTable("customers");

        table.insert("firstName", "lastName", "email", "phone", "company")
            .values(customer.getFirstName(),
                customer.getLastName(),
                customer.getEmail(),
                customer.getPhone(),
                customer.getCompany())
            .execute();

        return true;
    }
    catch (const mysqlx::Error& err) {
        handleDatabaseError("Create customer", err);
        return false;
    }
}

std::unique_ptr<Customer> DatabaseHandler::getCustomer(int id) {
    if (!session) {
        std::cerr << "No database connection" << std::endl;
        return nullptr;
    }

    try {
        auto table = session->getSchema(schema).getTable("customers");

        // Debug logging
        std::cout << "Fetching customer with ID: " << id << std::endl;

        auto result = table.select("*")
            .where("id = :id")
            .bind("id", id)
            .execute();

        auto row = result.fetchOne();
        if (!row) {
            return nullptr;
        }

        // Debug logging
        std::cout << "Found customer: " << std::endl;
        std::cout << "ID: " << (int)row[0] << std::endl;
        std::cout << "First Name: " << (std::string)row[1] << std::endl;
        std::cout << "Last Name: " << (std::string)row[2] << std::endl;

        return std::make_unique<Customer>(
            static_cast<int>(row[0]),          // id
            static_cast<std::string>(row[1]),  // firstName
            static_cast<std::string>(row[2]),  // lastName
            static_cast<std::string>(row[3]),  // email
            static_cast<std::string>(row[4]),  // phone
            static_cast<std::string>(row[5])   // company
        );
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "MySQL Error: " << err.what() << std::endl;
        handleDatabaseError("Get customer", err);
        return nullptr;
    }
    catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<Customer> DatabaseHandler::getAllCustomers() {
    std::vector<Customer> customers;

    if (!session) {
        std::cerr << "No database connection" << std::endl;
        return customers;
    }

    try {
        auto table = session->getSchema(schema).getTable("customers");
        auto result = table.select("*").execute();

        for (auto row : result) {
            customers.emplace_back(
                static_cast<int>(row[0]),          // id
                static_cast<std::string>(row[1]),  // firstName
                static_cast<std::string>(row[2]),  // lastName
                static_cast<std::string>(row[3]),  // email
                static_cast<std::string>(row[4]),  // phone
                static_cast<std::string>(row[5])   // company
            );
        }
    }
    catch (const mysqlx::Error& err) {
        handleDatabaseError("Get all customers", err);
    }
    return customers;
}

bool DatabaseHandler::updateCustomer(const Customer& customer) {
    if (!session) {
        std::cerr << "No database connection" << std::endl;
        return false;
    }

    try {
        auto table = session->getSchema(schema).getTable("customers");

        // Debug logging
        std::cout << "Updating customer:" << std::endl;
        std::cout << "ID: " << customer.getId() << std::endl;

        // Create the update statement with proper SET expressions
        auto statement = table.update()
            .set("firstName", mysqlx::Value(customer.getFirstName()))
            .set("lastName", mysqlx::Value(customer.getLastName()))
            .set("email", mysqlx::Value(customer.getEmail()))
            .set("phone", mysqlx::Value(customer.getPhone()))
            .set("company", mysqlx::Value(customer.getCompany()))
            .where("id = :id");

        // Execute with parameter binding
        auto result = statement.bind("id", customer.getId()).execute();

        std::cout << "Affected rows: " << result.getAffectedItemsCount() << std::endl;

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "MySQL Error Message: " << err.what() << std::endl;
        handleDatabaseError("Update customer", err);
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseHandler::deleteCustomer(int id) {
    if (!session) {
        std::cerr << "No database connection" << std::endl;
        return false;
    }

    try {
        auto table = session->getSchema(schema).getTable("customers");

        table.remove()
            .where("id = :id")
            .bind("id", id)
            .execute();

        return true;
    }
    catch (const mysqlx::Error& err) {
        handleDatabaseError("Delete customer", err);
        return false;
    }
}

void DatabaseHandler::handleDatabaseError(const std::string& operation, const std::exception& e) {
    std::cerr << "Error during " << operation << ": " << e.what() << std::endl;
}