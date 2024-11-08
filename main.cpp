#define CROW_ENABLE_CORS
#include "crow.h"
#include "DatabaseHandler.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct CORSHandler {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        if (req.method == "OPTIONS"_method) 
        { res.add_header("Access-Control-Allow-Origin", "*"); 
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE"); 
        res.add_header("Access-Control-Allow-Headers", "Content-Type"); res.end(); }
    }
    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }
};

int main() {
    crow::App<CORSHandler> app;

    // Initialize database connection
    DatabaseHandler db("localhost", 33060, "root", "12345", "crm_database");

    // Try to connect to database
    if (!db.connect()) {
        std::cerr << "Failed to connect to database. Please check your connection settings." << std::endl;
        return 1;
    }

    /*// CORS middleware
   //auto& cors = app.get_middleware<crow::CORSHandler>();

    // Configure CORS
    cors
        .global()
        .headers("*")
        .methods("POST"_method, "GET"_method, "PUT"_method, "DELETE"_method)
        .prefix("/")
        .origin("*");*/

    // GET all customers
    CROW_ROUTE(app, "/customers")([&db]() {
        auto customers = db.getAllCustomers();
        json response = json::array();

        for (const auto& customer : customers) {
            response.push_back({
                {"id", customer.getId()},
                {"firstName", customer.getFirstName()},
                {"lastName", customer.getLastName()},
                {"email", customer.getEmail()},
                {"phone", customer.getPhone()},
                {"company", customer.getCompany()}
                });
        }

        return crow::response(response.dump());
        });

    // GET customer by ID
    CROW_ROUTE(app, "/customers/<int>")
        ([&db](int id) {
        std::cout << "Received GET request for customer ID: " << id << std::endl;
        auto customer = db.getCustomer(id);
        if (!customer) {
            return crow::response(404, "Customer not found");
        }

        json response = {
            {"id", customer->getId()},
            {"firstName", customer->getFirstName()},
            {"lastName", customer->getLastName()},
            {"email", customer->getEmail()},
            {"phone", customer->getPhone()},
            {"company", customer->getCompany()}
        };

        return crow::response(response.dump());
            });

    // POST new customer
    CROW_ROUTE(app, "/customers")
        .methods("POST"_method)
        ([&db](const crow::request& req) {
        try {
            auto body = json::parse(req.body);
            Customer customer(
                0,  // ID will be assigned by database
                body["firstName"].get<std::string>(),
                body["lastName"].get<std::string>(),
                body["email"].get<std::string>(),
                body["phone"].get<std::string>(),
                body["company"].get<std::string>()
            );

            if (db.createCustomer(customer)) {
                return crow::response(201, "Customer created successfully");
            }
            return crow::response(500, "Failed to create customer");
        }
        catch (const json::exception& e) {
            return crow::response(400, "Invalid JSON format");
        }
        catch (const std::exception& e) {
            return crow::response(500, "Internal server error");
        }
            });

    // PUT update customer
    CROW_ROUTE(app, "/customers/<int>")
        .methods("PUT"_method)
        ([&db](const crow::request& req, int id) {
        try {

            // Debug logging
            std::cout << "Received update request for customer ID: " << id << std::endl;
            std::cout << "Request body: " << req.body << std::endl;


            auto body = json::parse(req.body);
            Customer customer(
                id,
                body["firstName"].get<std::string>(),
                body["lastName"].get<std::string>(),
                body["email"].get<std::string>(),
                body["phone"].get<std::string>(),
                body["company"].get<std::string>()
            );

            if (db.updateCustomer(customer)) {
                return crow::response(200, "Customer updated successfully");
            }
            return crow::response(500, "Failed to update customer");
        }
        catch (const json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
            return crow::response(400, "Invalid JSON format");
        }
        catch (const std::exception& e) {
            std::cerr << "Error updating customer: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
            });

    // DELETE customer
    CROW_ROUTE(app, "/customers/<int>")
        .methods("DELETE"_method)
        ([&db](int id) {
        if (db.deleteCustomer(id)) {
            return crow::response(200, "Customer deleted successfully");
        }
        return crow::response(500, "Failed to delete customer");
            });

    // Start the server
    app.port(8080)
        .multithreaded()
        .run();

    return 0;
}