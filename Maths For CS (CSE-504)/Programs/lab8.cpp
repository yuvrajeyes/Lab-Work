#include <iostream>
using namespace std;

class ModularArithmetic {
private:
    int p; 

    // Function to perform modulo operation
    int mod(int a, int b) {
        int result = a % b;
        if (result < 0) result += b;
        
        return result;
    }

public:
    ModularArithmetic(int modulus) : p(modulus) {}

    // Function to calculate modular exponentiation (a^b mod p)
    int modExp(int a, int b) {
        if (b == 0) return 1;
        long long int temp = modExp(a, b / 2);
        long long int result = (temp * temp) % p;
        if (b % 2 == 1) result = (result * a) % p;
        
        return static_cast<int>(result);
    }

    // Function to calculate modular inverse (a^(-1) mod p)
    int modInverse(int a) {
        return modExp(a, p - 2);
    }

    // Function to perform addition in Zp
    int add(int a, int b) {
        return mod(a + b, p);
    }

    // Function to perform subtraction in Zp
    int subtract(int a, int b) {
        return mod(a - b, p);
    }

    // Function to perform multiplication in Zp
    int multiply(int a, int b) {
        return mod(a * b, p);
    }

    // Function to perform division in Zp
    int divide(int a, int b) {
        // Division by b is equivalent to multiplication by the modular inverse of b
        return multiply(a, modInverse(b));
    }
};

int main() {
    int p;
    cout << "Enter the prime number p: ";
    cin >> p;

    ModularArithmetic Zp(p);

    int a, b;
    cout << "Enter two integers a and b: ";
    cin >> a >> b;

    // Perform field operations
    int sum = Zp.add(a, b);
    int difference = Zp.subtract(a, b);
    int product = Zp.multiply(a, b);
    int quotient = Zp.divide(a, b);

    // Output results
    cout<<"Sum(a + b) mod "<<p<<" = "<<sum<<endl;
    cout<<"Difference(a - b) mod "<<p<<" = "<<difference<<endl;
    cout<<"Product(a * b) mod "<<p<<" = "<<product<<endl;
    cout<<"Quotient(a / b) mod "<<p<<" = "<<quotient<<endl;
    cout<<Zp.modInverse(5);

    return 0;
}