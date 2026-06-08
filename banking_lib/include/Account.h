#pragma once
#include <string>

class Account {
private:
    std::string iban;
    double balance;
    
public:
    Account(const std::string& iban, double initialBalance = 0.0);
    
    virtual bool deposit(double amount);
    virtual bool withdraw(double amount);
    virtual double getBalance() const;
    virtual std::string getIban() const;
};
