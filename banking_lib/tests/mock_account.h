#pragma once
#include <gmock/gmock.h>
#include "Account.h"

class MockAccount : public Account {
public:
    MockAccount(const std::string& iban, double balance = 0.0) 
        : Account(iban, balance) {}
    
    MOCK_METHOD(bool, deposit, (double amount), (override));
    MOCK_METHOD(bool, withdraw, (double amount), (override));
    MOCK_METHOD(double, getBalance, (), (const, override));
    MOCK_METHOD(std::string, getIban, (), (const, override));
};
