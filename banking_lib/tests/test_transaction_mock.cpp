#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mock_account.h"
#include "Transaction.h"

using ::testing::Return;
using ::testing::Exactly;
using ::testing::_;

class TransactionMockTest : public ::testing::Test {
protected:
    MockAccount* from;
    MockAccount* to;
    
    void SetUp() override {
        from = new MockAccount("FROM123", 500.0);
        to = new MockAccount("TO456", 100.0);
    }
    
    void TearDown() override {
        delete from;
        delete to;
    }
};

// Тест с использованием mock: успешная транзакция
TEST_F(TransactionMockTest, ExecuteValidTransactionWithMock) {
    // Ожидаем, что withdraw и deposit будут вызваны по 1 разу
    EXPECT_CALL(*from, withdraw(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    EXPECT_CALL(*to, deposit(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    Transaction tx(from, to, 200.0);
    EXPECT_TRUE(tx.execute());
}

// Тест с использованием mock: недостаточно средств
TEST_F(TransactionMockTest, ExecuteTransactionInsufficientFundsMock) {
    EXPECT_CALL(*from, withdraw(600.0))
        .Times(Exactly(1))
        .WillOnce(Return(false));
    
    // deposit не должен вызываться, так как withdraw вернул false
    EXPECT_CALL(*to, deposit(_))
        .Times(0);
    
    Transaction tx(from, to, 600.0);
    EXPECT_FALSE(tx.execute());
}

// Тест с использованием mock: ошибка при deposit (откат)
TEST_F(TransactionMockTest, ExecuteTransactionDepositFailsRollbackMock) {
    // Первый вызов withdraw успешен
    EXPECT_CALL(*from, withdraw(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    // deposit возвращает false (ошибка на стороне получателя)
    EXPECT_CALL(*to, deposit(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(false));
    
    // Ожидаем откат - повторный deposit на from
    EXPECT_CALL(*from, deposit(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    Transaction tx(from, to, 200.0);
    EXPECT_FALSE(tx.execute());
}

// Тест с использованием mock: транзакция уже выполнена
TEST_F(TransactionMockTest, ExecuteTransactionAlreadyCompletedMock) {
    EXPECT_CALL(*from, withdraw(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    EXPECT_CALL(*to, deposit(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    Transaction tx(from, to, 200.0);
    EXPECT_TRUE(tx.execute());
    
    // Повторный execute не должен вызывать withdraw/deposit
    EXPECT_CALL(*from, withdraw(_)).Times(0);
    EXPECT_CALL(*to, deposit(_)).Times(0);
    
    EXPECT_FALSE(tx.execute());
}

// Тест с использованием mock: проверка revert
TEST_F(TransactionMockTest, RevertCompletedTransactionMock) {
    EXPECT_CALL(*from, withdraw(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    EXPECT_CALL(*to, deposit(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    Transaction tx(from, to, 200.0);
    EXPECT_TRUE(tx.execute());
    
    // Ожидаем откат: withdraw с to, deposit на from
    EXPECT_CALL(*to, withdraw(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    EXPECT_CALL(*from, deposit(200.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    tx.revert();
}
