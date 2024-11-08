#include "Customer.h"

Customer::Customer(int id, std::string firstName, std::string lastName,
    std::string email, std::string phone, std::string company)
    : id(id)
    , firstName(std::move(firstName))
    , lastName(std::move(lastName))
    , email(std::move(email))
    , phone(std::move(phone))
    , company(std::move(company))
{
}