#include<iostream>
#include<stdexcept>   // Required for invalid_argument and runtime_error

using namespace std;

// Class Invariant of a parent class Object should not be broken by child class Object.
// Hence child class can either maintain or strengthen the invariant but never narrows it down.

// Invariant : Balance cannot be negative
// --------------------------------------
// 🔹 Class Invariant means a condition that must ALWAYS remain true
//     for an object throughout its lifetime (except temporarily inside functions).
// 🔹 Here the invariant is: balance >= 0
// 🔹 No operation should ever make balance negative.

class BankAccount {
protected:
    double balance;   // Protected so child classes can access it

public:
    // Constructor enforces invariant at object creation
    BankAccount(double b) {
        // If someone tries to create account with negative balance,
        // we immediately stop execution by throwing exception.
        if (b < 0) 
            throw invalid_argument("Balance can't be negative");

        balance = b;
    }

    // Virtual function allows overriding in child classes
    virtual void withdraw(double amount) {

        // Before withdrawing, we check invariant condition.
        // If withdrawal makes balance negative → reject.
        if (balance - amount < 0) 
            throw runtime_error("Insufficient funds");

        balance -= amount;

        cout << "Amount withdrawn. Remaining balance is " 
             << balance << endl;
    }
};

// Breaks invariant : Should not be allowed.
// ------------------------------------------
// ❌ This class violates LSP (Liskov Substitution Principle)
// because it weakens the rule defined in parent class.
// Parent rule: balance should never go negative
// Child rule: allows negative balance → breaks invariant

class CheatAccount : public BankAccount {
public:
    CheatAccount(double b) : BankAccount(b) {}

    void withdraw(double amount) override {

        // ❌ No validation check here
        // ❌ Allows balance to become negative
        // ❌ Violates parent class invariant

        balance -= amount;  // LSP break! Negative balance allowed

        cout << "Amount withdrawn. Remaining balance is " 
             << balance << endl;
    }
};

int main() {

    // Using base class pointer
    BankAccount* bankAccount = new BankAccount(100);

    // This works correctly.
    // Withdraw 100 from 100 → balance becomes 0.
    bankAccount->withdraw(100);

    // If we instead used:
    // BankAccount* bankAccount = new CheatAccount(100);
    // bankAccount->withdraw(200);
    //
    // Then balance would become -100.
    // This breaks the invariant defined in BankAccount.
    // That is why CheatAccount violates LSP.

    delete bankAccount;  // Good practice to free memory
}
