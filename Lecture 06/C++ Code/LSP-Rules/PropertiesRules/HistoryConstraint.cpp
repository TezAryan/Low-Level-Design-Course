#include<iostream>
#include<stdexcept>   // Required for exceptions

using namespace std;

// Sub class methods should not be allowed state changes
// What Base class never allowed.

// 🔹 History Constraint (LSP Rule)
// ---------------------------------
// A subclass must NOT change the allowed sequence of operations
// (i.e., object history/behavior expectations).
//
// If the parent class allows an operation,
// the child class should NOT completely forbid it.
//
// Otherwise, client code that depends on parent behavior will break.

class BankAccount {
protected:
    double balance;

public:
    // Constructor ensures invariant: balance >= 0
    BankAccount(double b) {
        if (b < 0) 
            throw invalid_argument("Balance can't be negative");
        balance = b;
    }

    // History Constraint : Withdraw should be allowed
    // -----------------------------------------------
    // Any BankAccount object is expected to allow withdraw.
    // Client code assumes withdraw() is valid operation.
    virtual void withdraw(double amount) {
        if (balance - amount < 0) 
            throw runtime_error("Insufficient funds");

        balance -= amount;

        cout << "Amount withdrawn. Remaining balance is " 
             << balance << endl;
    }
};
    

class FixedDepositAccount : public BankAccount {
public:
    FixedDepositAccount(double b) : BankAccount(b) {}

    // ❌ LSP break! History constraint broke!
    // ---------------------------------------
    // Parent class behaviour: withdraw() is allowed.
    // Child class behaviour: withdraw() completely forbidden.
    //
    // This changes expected behavior.
    // Client code that works with BankAccount
    // will fail if it receives FixedDepositAccount.
    //
    // So this violates Liskov Substitution Principle.

    void withdraw(double amount) override {
        throw runtime_error("Withdraw not allowed in Fixed Deposit");
    }
};
    
int main() {

    // Client code expects withdraw to work.
    BankAccount* bankAccount = new BankAccount(100);
    bankAccount->withdraw(100);

    // If we replace with:
    // BankAccount* bankAccount = new FixedDepositAccount(100);
    // bankAccount->withdraw(50);
    //
    // Program will throw exception.
    // Client code breaks.
    //
    // That is why this is a History Constraint violation.

    delete bankAccount;  // Good practice
}
