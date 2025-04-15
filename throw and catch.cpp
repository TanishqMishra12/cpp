#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    double bal = 1000.0;
    try {
        double amt;
        // Deposit
        cout << "Enter Deposit Amount: ";
        cin >> amt;
        if (amt <= 0) {
            throw invalid_argument("Invalid Deposit Amount");
        }
        bal = bal + amt;
        cout << "Available Amount: " << bal << endl;

        // Withdrawal
        cout << "Enter Withdrawal Amount: ";
        cin >> amt;
        if (amt <= 0) {
            throw invalid_argument("Invalid Withdrawal Amount");
        }
        if (amt > bal) {
            throw runtime_error("Insufficient Funds");
        }
        bal = bal - amt;
        cout << "Available Amount: " << bal << endl;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}