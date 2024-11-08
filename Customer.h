#pragma once
#include <string>

class Customer {
private:
    int id;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    std::string company;

public:
    // Default constructor
    Customer() : id(0), firstName(""), lastName(""), email(""), phone(""), company("") {}

    // Main constructor
    Customer(int id, std::string firstName, std::string lastName,
        std::string email, std::string phone, std::string company);

    // Getter methods
    int getId() const { return id; }
    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    const std::string& getEmail() const { return email; }
    const std::string& getPhone() const { return phone; }
    const std::string& getCompany() const { return company; }

    // Setter methods
    void setId(int newId) { id = newId; }
    void setFirstName(const std::string& name) { firstName = name; }
    void setLastName(const std::string& name) { lastName = name; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }
    void setCompany(const std::string& newCompany) { company = newCompany; }
};