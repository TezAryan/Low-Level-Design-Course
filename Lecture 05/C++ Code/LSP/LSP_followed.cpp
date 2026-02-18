#include <iostream>
#include <vector>
#include <typeinfo>
#include <stdexcept>

using namespace std;

/*
    LISKOV SUBSTITUTION PRINCIPLE (LSP):
    ------------------------------------
    "Objects of a superclass should be replaceable with objects of a subclass
     without affecting correctness of the program."

    Here:
    - DepositOnlyAccount is the base abstraction.
    - WithdrawableAccount extends DepositOnlyAccount.
    - SavingAccount, CurrentAccount, FixedTermAccount are concrete implementations.

    The BankClient uses base class pointers.
    So any subclass object can replace the base class reference safely.
*/


// Base abstract class: Only deposit allowed
class DepositOnlyAccount {
public:
    virtual void deposit(double amount) = 0;  // Pure virtual function
};


// Extended abstract class: Deposit + Withdraw allowed
class WithdrawableAccount : public DepositOnlyAccount {
public:
    virtual void withdraw(double amount) = 0;  // Additional capability
};


// Concrete class: Savings Account
// Can deposit and withdraw
class SavingAccount : public WithdrawableAccount {
private:
    double balance;

public:
    SavingAccount() { 
        balance = 0; 
    }

    // Implements deposit (from DepositOnlyAccount)
    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount 
             << " in Savings Account. New Balance: " 
             << balance << endl;
    }

    // Implements withdraw (from WithdrawableAccount)
    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            cout << "Withdrawn: " << amount 
                 << " from Savings Account. New Balance: " 
                 << balance << endl;
        } else {
            cout << "Insufficient funds in Savings Account!\n";
        }
    }
};


// Concrete class: Current Account
// Also supports deposit and withdraw
class CurrentAccount : public WithdrawableAccount {
private:
    double balance;

public:
    CurrentAccount() { 
        balance = 0; 
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount 
             << " in Current Account. New Balance: " 
             << balance << endl;
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            cout << "Withdrawn: " << amount 
                 << " from Current Account. New Balance: " 
                 << balance << endl;
        } else {
            cout << "Insufficient funds in Current Account!\n";
        }
    }
};


// Concrete class: Fixed Term Account
// Only supports deposit
// Cannot withdraw (because it inherits from DepositOnlyAccount only)
class FixedTermAccount : public DepositOnlyAccount {
private:
    double balance;

public:
    FixedTermAccount() { 
        balance = 0; 
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount 
             << " in Fixed Term Account. New Balance: " 
             << balance << endl;
    }
};


// Client class that works with base class pointers
class BankClient {
private:
    vector<WithdrawableAccount*> withdrawableAccounts;
    vector<DepositOnlyAccount*> depositOnlyAccounts;

public:
    BankClient(
        vector<WithdrawableAccount*> withdrawableAccounts, 
        vector<DepositOnlyAccount*> depositOnlyAccounts
    ) { 
        this->withdrawableAccounts = withdrawableAccounts; 
        this->depositOnlyAccounts = depositOnlyAccounts;
    }

    void processTransactions() {

        // For withdrawable accounts:
        // We can safely call deposit() and withdraw()
        // because LSP guarantees subclasses behave correctly
        for (WithdrawableAccount* acc : withdrawableAccounts) {
            acc->deposit(1000);
            acc->withdraw(500); 
        }

        // For deposit-only accounts:
        // We only call deposit()
        // No withdraw() call → so no LSP violation
        for (DepositOnlyAccount* acc : depositOnlyAccounts) {
            acc->deposit(5000);
        }
    }
};


int main() {

    // Vector of withdrawable accounts
    // SavingAccount and CurrentAccount can be substituted
    // wherever WithdrawableAccount is expected
    vector<WithdrawableAccount*> withdrawableAccounts;
    withdrawableAccounts.push_back(new SavingAccount());
    withdrawableAccounts.push_back(new CurrentAccount());

    // Vector of deposit-only accounts
    // FixedTermAccount can substitute DepositOnlyAccount
    vector<DepositOnlyAccount*> depositOnlyAccounts;
    depositOnlyAccounts.push_back(new FixedTermAccount());

    // BankClient depends only on abstractions (base classes)
    BankClient* client = new BankClient(
        withdrawableAccounts, 
        depositOnlyAccounts
    );

    client->processTransactions();

    return 0;
}
