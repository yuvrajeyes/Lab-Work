#include<bits/stdc++.h>
using namespace std;

bool isPrime(long long n) {
    if (n<=1) return 0;
    else if (n==2) return 1;
    else if ((n&1)==0) return 0;
    else {
        int i = 3;
        while (i*i <= n) {
            if (n % i == 0) return 0;
            i += 2;
        }
        return true;
    }
}

long long gcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    long long x1, y1;
    long long g = gcd(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return g;
}

long long inverse(long long e, long long phi) {
    long long x, y;
    long long g = gcd(e, phi, x, y);

    if (g != 1) {
        cerr << "Inverse doesn't exist\n";
        return -1;
    } else {
        // Ensure 'x' is positive
        return (x % phi + phi) % phi;
    }
}

long long mod_pow(long long base, long long exponent, long long modulus) {
    long long result = 1;
    
    while (exponent > 0) {
        if (exponent & 1)
            result = (result * base) % modulus;
        
        exponent >>= 1;
        base = (base * base) % modulus;
    }
    
    return result;
}


int main() {
    long long P, Q;
    cout<<"Please Enter the value of P: ";
    cin>>P;
    cout<<"Please Enter the value of Q: ";
    cin>>Q;
    while (!isPrime(P)) { // O(sqrt(P))
        cout<<P<<" is not prime. Please enter prime number(P): ";
        cin>>P;
    }
    while (!isPrime(Q)) { // O(sqrt(Q))
        cout<<Q<<" is not prime. Please enter prime number(Q): ";
        cin>>Q;
    }
    long long N = P * Q;
    long long phi = (P - 1) * (Q - 1);

    cout<<"Value of phi is: "<<phi<<endl;

    long long e;
    cout<<"Please Enter the value of e (2-"<<phi-1<<") that are coprime to " << phi <<" : ";
    cin>>e;

    while (e < phi) {  // O(1)
        // e must be co-prime to phi and smaller than phi.
        if (gcd(e, phi) == 1)
            break;
        else {
            e = (e + 1) % phi;
            if (e == 1) e++;
        }
    }

    long long d = inverse(e, phi);

    cout<<"Value of e is : "<<e<<endl;
    cout<<"Value of d is : "<<d<<endl;

    long long m;
    cout<<"Please Enter the value of m: ";
    cin>>m;

    // Encryption c = (msg ^ e) % n
    long long c = mod_pow(m, e, N);  // O(log(e))
    cout<<"\nCipher Text = " << c;
 
    // Decryption m = (c ^ d) % n
    long long msg = mod_pow(c, d, N); // O(log(d))
    cout<<"\nOriginal Message Sent = " << msg;

    return 0;
}