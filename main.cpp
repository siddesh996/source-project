#include <iostream>
#include <vector>
#include <string>
#include <iomanip>  // For setprecision
#include <limits>   // For input validation

class BankAccount {
private:
    int accountNumber;
    std::string accountHolder;
    double balance;

public:
    BankAccount(int accNo, const std::string& name, double initialBalance)
        : accountNumber(accNo), accountHolder(name), balance(initialBalance) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposited: " << amount << "\n";
        } else {
            std::cout << "Invalid deposit amount.\n";
        }
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawn: " << amount << "\n";
            return true;
        } else {
            std::cout << "Invalid or insufficient funds.\n";
            return false;
        }
    }

    void display() const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Account Number: " << accountNumber << "\n"
                  << "Holder Name: " << accountHolder << "\n"
                  << "Balance: $" << balance << "\n";
    }

    int getAccountNumber() const {
        return accountNumber;
    }
};

// Helper to find account by number
BankAccount* findAccount(std::vector<BankAccount>& accounts, int accNo) {
    for (auto& acc : accounts) {
        if (acc.getAccountNumber() == accNo) {
            return &acc;
        }
    }
    return nullptr;
}

// Menu functions
void createAccount(std::vector<BankAccount>& accounts) {
    int accNo;
    std::string name;
    double initBal;

    std::cout << "Enter Account Number: ";
    std::cin >> accNo;
    if (!std::cin) return;

    if (findAccount(accounts, accNo)) {
        std::cout << "Account number already exists.\n";
        return;
    }

    std::cin.ignore(); // Clear newline for getline
    std::cout << "Enter Account Holder Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Initial Balance: ";
    std::cin >> initBal;

    accounts.emplace_back(accNo, name, initBal);
    std::cout << "Account created successfully.\n";
}

void depositToAccount(std::vector<BankAccount>& accounts) {
    int accNo;
    double amount;
    std::cout << "Enter Account Number: ";
    std::cin >> accNo;

    BankAccount* acc = findAccount(accounts, accNo);
    if (acc) {
        std::cout << "Enter Amount to Deposit: ";
        std::cin >> amount;
        acc->deposit(amount);
    } else {
        std::cout << "Account not found.\n";
    }
}

void withdrawFromAccount(std::vector<BankAccount>& accounts) {
    int accNo;
    double amount;
    std::cout << "Enter Account Number: ";
    std::cin >> accNo;

    BankAccount* acc = findAccount(accounts, accNo);
    if (acc) {
        std::cout << "Enter Amount to Withdraw: ";
        std::cin >> amount;
        acc->withdraw(amount);
    } else {
        std::cout << "Account not found.\n";
    }
}

void displayAccount(const std::vector<BankAccount>& accounts) {
    int accNo;
    std::cout << "Enter Account Number: ";
    std::cin >> accNo;

    const BankAccount* acc = nullptr;
    for (const auto& a : accounts) {
        if (a.getAccountNumber() == accNo) {
            acc = &a;
            break;
        }
    }

    if (acc) {
        acc->display();
    } else {
        std::cout << "Account not found.\n";
    }
}

void clearInput() {
    std::cin.clear(); // clear fail state
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
}

int main() {
    std::vector<BankAccount> accounts;
    int choice;

    while (true) {
        std::cout << "\n==== Bank Menu ====\n";
        std::cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Display Account\n5. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (!std::cin) {
            std::cout << "Invalid input. Try again.\n";
            clearInput();
            continue;
        }

        switch (choice) {
            case 1:
                createAccount(accounts);
                break;
            case 2:
                depositToAccount(accounts);
                break;
            case 3:
                withdrawFromAccount(accounts);
                break;
            case 4:
                displayAccount(accounts);
                break;
            case 5:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}

