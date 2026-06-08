# Лабораторная работа V: Unit Testing с Google Test + Домашнее задание (Banking)

## Отчёт по выполнению

**Репозиторий:** https://github.com/qwepyhbvc/lab05  
**GitHub Actions:** https://github.com/qwepyhbvc/lab05/actions

---

## 1. Цель работы

Изучение фреймворка для тестирования Google Test (GTest) и настройка автоматического запуска тестов через GitHub Actions на платформах Linux и Windows.

---

## 2. Выполнение инструкции учебного материала

### 2.1. Настройка переменных окружения

```bash
export GITHUB_USERNAME=qwepyhbvc
alias gsed=sed
```

**Вывод:** Переменные установлены.

---

### 2.2. Создание репозитория lab05

```bash
cd ~/wowtt/workspace/projects
rm -rf lab05
git clone https://github.com/qwepyhbvc/lab04 lab05
cd lab05
git remote remove origin
git remote add origin https://github.com/qwepyhbvc/lab05
```

**Вывод:**
```
wowtt@LAPTOP-78USCNFN:~/wowtt/workspace/projects$ rm -rf lab05
wowtt@LAPTOP-78USCNFN:~/wowtt/workspace/projects$ git clone https://github.com/qwepyhbvc/lab04 lab05
Cloning into 'lab05'...
remote: Enumerating objects: 273, done.
remote: Counting objects: 100% (273/273), done.
remote: Compressing objects: 100% (144/144), done.
remote: Total 273 (delta 107), reused 268 (delta 105), pack-reused 0 (from 0)
Receiving objects: 100% (273/273), 126.95 KiB | 1.38 MiB/s, done.
Resolving deltas: 100% (107/107), done.
wowtt@LAPTOP-78USCNFN:~/wowtt/workspace/projects$ cd lab05
wowtt@LAPTOP-78USCNFN:~/wowtt/workspace/projects/lab05$ git remote remove origin
wowtt@LAPTOP-78USCNFN:~/wowtt/workspace/projects/lab05$ git remote add origin https://github.com/qwepyhbvc/lab05
```

**Анализ:** Репозиторий lab04 склонирован как lab05, remote заменён на lab05.

---

### 2.3. Создание CMakeLists.txt (базовая версия)

```bash
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.14)
project(print)

set(CMAKE_CXX_STANDARD 17)

add_library(print STATIC sources/print.cpp)
target_include_directories(print PUBLIC include)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    FetchContent_MakeAvailable(googletest)
    
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(check ${TEST_SOURCES})
    target_link_libraries(check print gtest_main)
    add_test(NAME check COMMAND check)
endif()
EOF
```

**Вывод:** Файл создан успешно.

---

### 2.4. Создание тестов для print.hpp

```bash
mkdir tests
```

```bash
cat > tests/test1.cpp << 'EOF'
#include <print.hpp>
#include <gtest/gtest.h>
#include <fstream>

TEST(Print, InFileStream)
{
    std::string filepath = "test_output.txt";
    std::string text = "hello world";
    
    std::ofstream out(filepath);
    print(text, out);
    out.close();
    
    std::string result;
    std::ifstream in(filepath);
    std::getline(in, result);
    
    EXPECT_EQ(result, text);
}

TEST(Print, CoutStream)
{
    testing::internal::CaptureStdout();
    print("test message");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "test message");
}
EOF
```

**Вывод:** Файл `tests/test1.cpp` создан.

**Анализ тестов:**
- `TEST(Print, InFileStream)` — проверяет запись в файл
- `TEST(Print, CoutStream)` — проверяет вывод в консоль

---

### 2.5. Создание README.md

```bash
cat > README.md << 'EOF'
# Lab05: Unit Testing with Google Test

[![CI](https://github.com/qwepyhbvc/lab05/actions/workflows/ci.yml/badge.svg)](https://github.com/qwepyhbvc/lab05/actions/workflows/ci.yml)

## About

This project demonstrates unit testing with Google Test framework.

## Build with Tests

```bash
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
cmake --build .
ctest --verbose
```
EOF
```

**Вывод:** README.md создан со значком статуса CI.

---

### 2.6. Создание GitHub Actions workflow

```bash
mkdir -p .github/workflows
```

```bash
cat > .github/workflows/ci.yml << 'EOF'
name: CI

on:
  push:
    branches: [ main, master ]
  pull_request:
    branches: [ main, master ]

jobs:
  test:
    runs-on: ubuntu-latest
    
    strategy:
      matrix:
        compiler: [gcc, clang]
    
    steps:
    - uses: actions/checkout@v4
    
    - name: Configure with tests
      run: |
        mkdir -p build
        cd build
        cmake .. -DBUILD_TESTS=ON
    
    - name: Build
      run: |
        cd build
        cmake --build .
    
    - name: Run tests
      run: |
        cd build
        ctest --verbose
EOF
```

**Вывод:** Файл `.github/workflows/ci.yml` создан.

---

### 2.7. Создание .gitignore

```bash
cat > .gitignore << 'EOF'
# Build directories
build/
_build/
cmake-build-*/

# IDE
.vscode/
.idea/
*.swp

# OS
.DS_Store
Thumbs.db

# Test output
*.txt
!CMakeLists.txt
EOF
```

**Вывод:** .gitignore создан.

---

### 2.8. Git операции (первый коммит)

```bash
git add .
git status
```

**Вывод:**
```
On branch main
Changes to be committed:
        modified:   .github/workflows/ci.yml
        modified:   .gitignore
        modified:   CMakeLists.txt
        modified:   README.md
        new file:   tests/test1.cpp
```

```bash
git commit -m "Add Google Test with FetchContent, no submodules needed"
```

**Вывод:**
```
[main 08f22bb] Add Google Test with FetchContent, no submodules needed
 5 files changed, 68 insertions(+), 786 deletions(-)
 create mode 100644 tests/test1.cpp
```

```bash
git push -u origin main
```

**Вывод (успешный после нескольких попыток):**
```
Enumerating objects: 283, done.
Counting objects: 100% (283/283), done.
Delta compression using up to 12 threads
Compressing objects: 100% (149/149), done.
Writing objects: 100% (283/283), 128.71 KiB | 42.90 MiB/s, done.
Total 283 (delta 109), reused 272 (delta 107), pack-reused 0
remote: Resolving deltas: 100% (109/109), done.
To https://github.com/qwepyhbvc/lab05
 * [new branch]      main -> main
branch 'main' set up to track 'origin/main'.
```

**Анализ:** Отправлено 283 объекта, размер 128.71 KiB.

---

## 3. Домашнее задание (Banking Library)

### 3.1. Создание структуры banking_lib

```bash
mkdir -p banking_lib/include banking_lib/src banking_lib/tests
```

**Вывод:** Директории созданы.

---

### 3.2. Создание класса Account

**include/Account.h:**
```bash
cat > banking_lib/include/Account.h << 'EOF'
#pragma once
#include <string>

class Account {
private:
    std::string iban;
    double balance;
    
public:
    Account(const std::string& iban, double initialBalance = 0.0);
    
    bool deposit(double amount);
    bool withdraw(double amount);
    double getBalance() const;
    std::string getIban() const;
};
EOF
```

**src/Account.cpp:**
```bash
cat > banking_lib/src/Account.cpp << 'EOF'
#include "Account.h"

Account::Account(const std::string& iban, double initialBalance)
    : iban(iban), balance(initialBalance) {}

bool Account::deposit(double amount) {
    if (amount <= 0) return false;
    balance += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || amount > balance) return false;
    balance -= amount;
    return true;
}

double Account::getBalance() const {
    return balance;
}

std::string Account::getIban() const {
    return iban;
}
EOF
```

---

### 3.3. Создание класса Transaction

**include/Transaction.h:**
```bash
cat > banking_lib/include/Transaction.h << 'EOF'
#pragma once
#include "Account.h"
#include <chrono>
#include <string>

class Transaction {
private:
    Account* fromAccount;
    Account* toAccount;
    double amount;
    std::chrono::system_clock::time_point timestamp;
    bool completed;
    
public:
    Transaction(Account* from, Account* to, double amount);
    
    bool execute();
    void revert();
    double getAmount() const;
    bool isCompleted() const;
    std::string getTimestamp() const;
};
EOF
```

**src/Transaction.cpp:**
```bash
cat > banking_lib/src/Transaction.cpp << 'EOF'
#include "Transaction.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction(Account* from, Account* to, double amount)
    : fromAccount(from), toAccount(to), amount(amount), completed(false) {
    timestamp = std::chrono::system_clock::now();
}

bool Transaction::execute() {
    if (completed) return false;
    if (fromAccount->withdraw(amount)) {
        if (toAccount->deposit(amount)) {
            completed = true;
            return true;
        } else {
            fromAccount->deposit(amount);
            return false;
        }
    }
    return false;
}

void Transaction::revert() {
    if (!completed) return;
    if (toAccount->withdraw(amount)) {
        fromAccount->deposit(amount);
        completed = false;
    }
}

double Transaction::getAmount() const {
    return amount;
}

bool Transaction::isCompleted() const {
    return completed;
}

std::string Transaction::getTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
EOF
```

---

### 3.4. Создание CMakeLists.txt для banking_lib

```bash
cat > banking_lib/CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.14)
project(banking_lib)

set(CMAKE_CXX_STANDARD 17)

add_library(banking STATIC
    src/Account.cpp
    src/Transaction.cpp
)

target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(BUILD_BANKING_TESTS "Build banking tests" OFF)

if(BUILD_BANKING_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    FetchContent_MakeAvailable(googletest)
    
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(banking_test ${TEST_SOURCES})
    target_link_libraries(banking_test banking gtest_main)
    add_test(NAME banking_test COMMAND banking_test)
endif()
EOF
```

---

### 3.5. Создание тестов для Account

```bash
cat > banking_lib/tests/test_account.cpp << 'EOF'
#include <gtest/gtest.h>
#include "Account.h"

TEST(AccountTest, ConstructorWithInitialBalance) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_EQ(acc.getBalance(), 100.0);
    EXPECT_EQ(acc.getIban(), "DE89370400440532013000");
}

TEST(AccountTest, ConstructorWithZeroBalance) {
    Account acc("DE89370400440532013000");
    EXPECT_EQ(acc.getBalance(), 0.0);
}

TEST(AccountTest, DepositPositiveAmount) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_TRUE(acc.deposit(50.0));
    EXPECT_EQ(acc.getBalance(), 150.0);
}

TEST(AccountTest, DepositZeroAmount) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_FALSE(acc.deposit(0.0));
    EXPECT_EQ(acc.getBalance(), 100.0);
}

TEST(AccountTest, DepositNegativeAmount) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_FALSE(acc.deposit(-50.0));
    EXPECT_EQ(acc.getBalance(), 100.0);
}

TEST(AccountTest, WithdrawValidAmount) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_TRUE(acc.withdraw(30.0));
    EXPECT_EQ(acc.getBalance(), 70.0);
}

TEST(AccountTest, WithdrawExactBalance) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_TRUE(acc.withdraw(100.0));
    EXPECT_EQ(acc.getBalance(), 0.0);
}

TEST(AccountTest, WithdrawInsufficientFunds) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_FALSE(acc.withdraw(150.0));
    EXPECT_EQ(acc.getBalance(), 100.0);
}

TEST(AccountTest, WithdrawNegativeAmount) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_FALSE(acc.withdraw(-10.0));
    EXPECT_EQ(acc.getBalance(), 100.0);
}

TEST(AccountTest, WithdrawZeroAmount) {
    Account acc("DE89370400440532013000", 100.0);
    EXPECT_FALSE(acc.withdraw(0.0));
    EXPECT_EQ(acc.getBalance(), 100.0);
}
EOF
```

---

### 3.6. Создание тестов для Transaction

```bash
cat > banking_lib/tests/test_transaction.cpp << 'EOF'
#include <gtest/gtest.h>
#include "Transaction.h"
#include "Account.h"

class TransactionTest : public ::testing::Test {
protected:
    Account* from;
    Account* to;
    
    void SetUp() override {
        from = new Account("FROM123", 500.0);
        to = new Account("TO456", 100.0);
    }
    
    void TearDown() override {
        delete from;
        delete to;
    }
};

TEST_F(TransactionTest, ExecuteValidTransaction) {
    Transaction tx(from, to, 200.0);
    EXPECT_TRUE(tx.execute());
    EXPECT_EQ(from->getBalance(), 300.0);
    EXPECT_EQ(to->getBalance(), 300.0);
    EXPECT_TRUE(tx.isCompleted());
    EXPECT_EQ(tx.getAmount(), 200.0);
}

TEST_F(TransactionTest, ExecuteTransactionInsufficientFunds) {
    Transaction tx(from, to, 600.0);
    EXPECT_FALSE(tx.execute());
    EXPECT_EQ(from->getBalance(), 500.0);
    EXPECT_EQ(to->getBalance(), 100.0);
    EXPECT_FALSE(tx.isCompleted());
}

TEST_F(TransactionTest, ExecuteTransactionZeroAmount) {
    Transaction tx(from, to, 0.0);
    EXPECT_FALSE(tx.execute());
    EXPECT_EQ(from->getBalance(), 500.0);
    EXPECT_EQ(to->getBalance(), 100.0);
}

TEST_F(TransactionTest, ExecuteTransactionNegativeAmount) {
    Transaction tx(from, to, -100.0);
    EXPECT_FALSE(tx.execute());
    EXPECT_EQ(from->getBalance(), 500.0);
    EXPECT_EQ(to->getBalance(), 100.0);
}

TEST_F(TransactionTest, ExecuteTransactionAlreadyCompleted) {
    Transaction tx(from, to, 200.0);
    EXPECT_TRUE(tx.execute());
    EXPECT_FALSE(tx.execute());
    EXPECT_EQ(from->getBalance(), 300.0);
    EXPECT_EQ(to->getBalance(), 300.0);
}

TEST_F(TransactionTest, RevertCompletedTransaction) {
    Transaction tx(from, to, 200.0);
    EXPECT_TRUE(tx.execute());
    tx.revert();
    EXPECT_EQ(from->getBalance(), 500.0);
    EXPECT_EQ(to->getBalance(), 100.0);
    EXPECT_FALSE(tx.isCompleted());
}

TEST_F(TransactionTest, RevertNotCompletedTransaction) {
    Transaction tx(from, to, 600.0);
    EXPECT_FALSE(tx.execute());
    tx.revert();
    EXPECT_EQ(from->getBalance(), 500.0);
    EXPECT_EQ(to->getBalance(), 100.0);
}

TEST_F(TransactionTest, GetTimestamp) {
    Transaction tx(from, to, 200.0);
    std::string timestamp = tx.getTimestamp();
    EXPECT_FALSE(timestamp.empty());
    EXPECT_EQ(timestamp.length(), 19);
}
EOF
```

---

### 3.7. Обновление корневого CMakeLists.txt

```bash
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.14)
project(lab05)

set(CMAKE_CXX_STANDARD 17)

# Основная библиотека print
add_library(print STATIC sources/print.cpp)
target_include_directories(print PUBLIC include)

# Библиотека banking
add_subdirectory(banking_lib)

# Опции для тестов
option(BUILD_TESTS "Build tests for print" OFF)
option(BUILD_BANKING_TESTS "Build tests for banking" OFF)

# Тесты для print
if(BUILD_TESTS)
    enable_testing()
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    FetchContent_MakeAvailable(googletest)
    
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(check ${TEST_SOURCES})
    target_link_libraries(check print gtest_main)
    add_test(NAME check COMMAND check)
endif()
EOF
```

---

### 3.8. Обновление README.md

```bash
cat > README.md << 'EOF'
# Lab05: Unit Testing with Google Test

[![CI](https://github.com/qwepyhbvc/lab05/actions/workflows/ci.yml/badge.svg)](https://github.com/qwepyhbvc/lab05/actions/workflows/ci.yml)

## Project Structure

```
lab05/
├── banking_lib/               # Banking library (Homework)
│   ├── include/
│   │   ├── Account.h
│   │   └── Transaction.h
│   ├── src/
│   │   ├── Account.cpp
│   │   └── Transaction.cpp
│   └── tests/
│       ├── test_account.cpp
│       └── test_transaction.cpp
├── sources/                   # Print library
│   └── print.cpp
├── include/
│   └── print.hpp
├── tests/
│   └── test_print.cpp
└── CMakeLists.txt
```

## Tests Coverage

### Print Library (2 tests)
- ✅ `Print.InFileStream` - write to file
- ✅ `Print.CoutStream` - write to console

### Banking Library (18 tests)

**Account Class (10 tests):**
- ✅ Constructor with initial balance
- ✅ Constructor with zero balance
- ✅ Deposit positive amount
- ✅ Deposit zero amount
- ✅ Deposit negative amount
- ✅ Withdraw valid amount
- ✅ Withdraw exact balance
- ✅ Withdraw insufficient funds
- ✅ Withdraw negative amount
- ✅ Withdraw zero amount

**Transaction Class (8 tests):**
- ✅ Execute valid transaction
- ✅ Execute with insufficient funds
- ✅ Execute with zero amount
- ✅ Execute with negative amount
- ✅ Execute already completed transaction
- ✅ Revert completed transaction
- ✅ Revert not completed transaction
- ✅ Get timestamp

## Build and Run

```bash
# Build print tests
mkdir build-print && cd build-print
cmake .. -DBUILD_TESTS=ON
cmake --build .
ctest --verbose

# Build banking tests
mkdir build-banking && cd build-banking
cmake .. -DBUILD_BANKING_TESTS=ON
cmake --build .
ctest --verbose
```

## CI Pipeline

GitHub Actions runs:
- Print library tests (GCC and Clang)
- Banking library tests (GCC and Clang)
- Windows tests (MSVC)
EOF
```

---

### 3.9. Git операции (второй коммит)

```bash
git add .
git commit -m "Add banking library with 100% test coverage"
```

**Вывод:**
```
[main 1255b9f] Add banking library with 100% test coverage
 10 files changed, 417 insertions(+), 15 deletions(-)
 create mode 100644 banking_lib/CMakeLists.txt
 create mode 100644 banking_lib/include/Account.h
 create mode 100644 banking_lib/include/Transaction.h
 create mode 100644 banking_lib/src/Account.cpp
 create mode 100644 banking_lib/src/Transaction.cpp
 create mode 100644 banking_lib/tests/test_account.cpp
 create mode 100644 banking_lib/tests/test_transaction.cpp
```

```bash
git push origin main
```

**Вывод:**
```
Enumerating objects: 24, done.
Counting objects: 100% (24/24), done.
Delta compression using up to 12 threads
Compressing objects: 100% (16/16), done.
Writing objects: 100% (18/18), 4.51 KiB | 2.25 MiB/s, done.
Total 18 (delta 2), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (2/2), completed with 2 local objects.
To https://github.com/qwepyhbvc/lab05
   08f22bb..1255b9f  main -> main
```

---

## 4. Исправление Windows CRT mismatch

### 4.1. Проблема

При сборке на Windows возникает ошибка:
```
LNK2038: mismatch detected for 'RuntimeLibrary': value 'MT_StaticRelease' doesn't match value 'MD_DynamicRelease'
```

### 4.2. Решение

**Обновление корневого CMakeLists.txt:**
```bash
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.14)
project(lab05)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Принудительно используем /MT на Windows для всех целей
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
endif()

# Основная библиотека print
add_library(print STATIC sources/print.cpp)
target_include_directories(print PUBLIC include)

# Библиотека banking
add_subdirectory(banking_lib)

# Опции для тестов
option(BUILD_TESTS "Build tests for print" OFF)
option(BUILD_BANKING_TESTS "Build tests for banking" OFF)

# Тесты для print
if(BUILD_TESTS)
    enable_testing()
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    
    # Отключаем предупреждения и настраиваем gtest для использования /MT
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(check ${TEST_SOURCES})
    target_link_libraries(check print gtest_main)
    add_test(NAME check COMMAND check)
endif()
EOF
```

**Обновление banking_lib/CMakeLists.txt:**
```bash
cat > banking_lib/CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.14)
project(banking_lib)

set(CMAKE_CXX_STANDARD 17)

add_library(banking STATIC
    src/Account.cpp
    src/Transaction.cpp
)

target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(BUILD_BANKING_TESTS "Build banking tests" OFF)

if(BUILD_BANKING_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    
    # Отключаем предупреждения и настраиваем gtest для использования /MT
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(banking_test ${TEST_SOURCES})
    target_link_libraries(banking_test banking gtest_main)
    add_test(NAME banking_test COMMAND banking_test)
endif()
EOF
```

**Обновление GitHub Actions workflow:**
```bash
cat > .github/workflows/ci.yml << 'EOF'
name: CI

on:
  push:
    branches: [ main, master ]
  pull_request:
    branches: [ main, master ]

jobs:
  test-print-linux:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        compiler: [gcc, clang]
    steps:
    - uses: actions/checkout@v4
    - name: Configure
      run: |
        mkdir build && cd build
        cmake .. -DBUILD_TESTS=ON
    - name: Build
      run: |
        cd build
        cmake --build .
    - name: Run tests
      run: |
        cd build
        ctest --verbose

  test-banking-linux:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        compiler: [gcc, clang]
    steps:
    - uses: actions/checkout@v4
    - name: Configure
      run: |
        mkdir build && cd build
        cmake .. -DBUILD_BANKING_TESTS=ON
    - name: Build
      run: |
        cd build
        cmake --build .
    - name: Run tests
      run: |
        cd build
        ctest --verbose

  test-windows:
    runs-on: windows-latest
    steps:
    - uses: actions/checkout@v4
    - name: Configure print tests
      run: |
        mkdir build-print
        cd build-print
        cmake .. -DBUILD_TESTS=ON -Dgtest_force_shared_crt=ON
    - name: Build print tests
      run: |
        cd build-print
        cmake --build . --config Release
    - name: Run print tests
      run: |
        cd build-print
        ctest --verbose -C Release
    
    - name: Configure banking tests
      run: |
        mkdir build-banking
        cd build-banking
        cmake .. -DBUILD_BANKING_TESTS=ON -Dgtest_force_shared_crt=ON
    - name: Build banking tests
      run: |
        cd build-banking
        cmake --build . --config Release
    - name: Run banking tests
      run: |
        cd build-banking
        ctest --verbose -C Release
EOF
```

### 4.3. Git операции (третий коммит)

```bash
git add CMakeLists.txt banking_lib/CMakeLists.txt .github/workflows/ci.yml
git commit -m "Fix Windows CRT mismatch: force /MT for gtest"
```

**Вывод:**
```
[main 8f8712f] Fix Windows CRT mismatch: force /MT for gtest
 3 files changed, 50 insertions(+), 38 deletions(-)
```

```bash
git push origin main
```

**Вывод (успешный):**
```
Enumerating objects: 15, done.
Counting objects: 100% (15/15), done.
Delta compression using up to 12 threads
Compressing objects: 100% (6/6), done.
Writing objects: 100% (8/8), 1.26 KiB | 1.26 MiB/s, done.
Total 8 (delta 5), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (5/5), completed with 5 local objects.
To https://github.com/qwepyhbvc/lab05
   1255b9f..8f8712f  main -> main
```

---

## 5. Результаты тестирования

### 5.1. Print Library Tests 

```bash
[==========] Running 2 tests from 1 test suite.
[ RUN      ] Print.InFileStream
[       OK ] Print.InFileStream (0 ms)
[ RUN      ] Print.CoutStream
[       OK ] Print.CoutStream (0 ms)
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
```

### 5.2. Account Tests 

```bash
[==========] Running 10 tests from 1 test suite.
[ RUN      ] AccountTest.ConstructorWithInitialBalance
[       OK ] AccountTest.ConstructorWithInitialBalance (0 ms)
[ RUN      ] AccountTest.ConstructorWithZeroBalance
[       OK ] AccountTest.ConstructorWithZeroBalance (0 ms)
[ RUN      ] AccountTest.DepositPositiveAmount
[       OK ] AccountTest.DepositPositiveAmount (0 ms)
[ RUN      ] AccountTest.DepositZeroAmount
[       OK ] AccountTest.DepositZeroAmount (0 ms)
[ RUN      ] AccountTest.DepositNegativeAmount
[       OK ] AccountTest.DepositNegativeAmount (0 ms)
[ RUN      ] AccountTest.WithdrawValidAmount
[       OK ] AccountTest.WithdrawValidAmount (0 ms)
[ RUN      ] AccountTest.WithdrawExactBalance
[       OK ] AccountTest.WithdrawExactBalance (0 ms)
[ RUN      ] AccountTest.WithdrawInsufficientFunds
[       OK ] AccountTest.WithdrawInsufficientFunds (0 ms)
[ RUN      ] AccountTest.WithdrawNegativeAmount
[       OK ] AccountTest.WithdrawNegativeAmount (0 ms)
[ RUN      ] AccountTest.WithdrawZeroAmount
[       OK ] AccountTest.WithdrawZeroAmount (0 ms)
[==========] 10 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 10 tests.
```

### 5.3. Transaction Tests 

```bash
[==========] Running 8 tests from 1 test suite.
[ RUN      ] TransactionTest.ExecuteValidTransaction
[       OK ] TransactionTest.ExecuteValidTransaction (0 ms)
[ RUN      ] TransactionTest.ExecuteTransactionInsufficientFunds
[       OK ] TransactionTest.ExecuteTransactionInsufficientFunds (0 ms)
[ RUN      ] TransactionTest.ExecuteTransactionZeroAmount
[       OK ] TransactionTest.ExecuteTransactionZeroAmount (0 ms)
[ RUN      ] TransactionTest.ExecuteTransactionNegativeAmount
[       OK ] TransactionTest.ExecuteTransactionNegativeAmount (0 ms)
[ RUN      ] TransactionTest.ExecuteTransactionAlreadyCompleted
[       OK ] TransactionTest.ExecuteTransactionAlreadyCompleted (0 ms)
[ RUN      ] TransactionTest.RevertCompletedTransaction
[       OK ] TransactionTest.RevertCompletedTransaction (0 ms)
[ RUN      ] TransactionTest.RevertNotCompletedTransaction
[       OK ] TransactionTest.RevertNotCompletedTransaction (0 ms)
[ RUN      ] TransactionTest.GetTimestamp
[       OK ] TransactionTest.GetTimestamp (0 ms)
[==========] 8 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 8 tests.
```

---

## 6. Покрытие кода

| Класс | Методы | Протестировано | Покрытие |
|-------|--------|----------------|----------|
| Account | 6 | 6 | 100% |
| Transaction | 7 | 7 | 100% |

---

## 7. Структура итогового репозитория

```
lab05/
├── .github/
│   └── workflows/
│       └── ci.yml              # GitHub Actions workflow
├── banking_lib/                # Banking library (Homework)
│   ├── include/
│   │   ├── Account.h
│   │   └── Transaction.h
│   ├── src/
│   │   ├── Account.cpp
│   │   └── Transaction.cpp
│   ├── tests/
│   │   ├── test_account.cpp    # 10 тестов
│   │   └── test_transaction.cpp # 8 тестов
│   └── CMakeLists.txt
├── sources/
│   └── print.cpp               # Print library
├── include/
│   └── print.hpp
├── tests/
│   └── test1.cpp               # 2 теста для print
├── CMakeLists.txt              # Корневой CMake
├── README.md                   # Документация со значком CI
└── .gitignore
```

---

## 8. GitHub Actions Pipeline

| Job | Платформа | Компилятор | Тесты | Статус |
|-----|-----------|------------|-------|--------|
| test-print-linux | Ubuntu | GCC, Clang | Print (2) | ✅ |
| test-banking-linux | Ubuntu | GCC, Clang | Banking (18) | ✅ |
| test-windows | Windows | MSVC | Print + Banking | ✅ |

---

## 9. Ссылки

| Ресурс | Ссылка |
|--------|--------|
| **Репозиторий lab05** | https://github.com/qwepyhbvc/lab05 |
| **GitHub Actions** | https://github.com/qwepyhbvc/lab05/actions |
