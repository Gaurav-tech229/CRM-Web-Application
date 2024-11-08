#pragma once
#include <mysqlx/xdevapi.h>
#include <memory>
#include <vector>
#include "Customer.h"

class DatabaseHandler {
private:
    mysqlx::Session* session;
    std::string host;
    uint16_t port;
    std::string user;
    std::string password;
    std::string schema;

public:
    DatabaseHandler(const std::string& host, uint16_t port,
        const std::string& user, const std::string& password,
        const std::string& schema);
    ~DatabaseHandler();

    bool connect();
    void disconnect();

    bool createCustomer(const Customer& customer);
    std::unique_ptr<Customer> getCustomer(int id);
    std::vector<Customer> getAllCustomers();
    bool updateCustomer(const Customer& customer);
    bool deleteCustomer(int id);

private:
    void handleDatabaseError(const std::string& operation, const std::exception& e);
};