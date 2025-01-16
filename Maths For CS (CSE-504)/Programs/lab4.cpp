#include <bits/stdc++.h>
using namespace std;

// calculate the greatest common divisor (GCD) using Euclid's Algorithm
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

// find the modular inverse of 'a' modulo 'm'
long long inverse(long long a, long long m) {
    long long x, y;
    long long g = gcd(a, m, x, y);

    if (g != 1)  return -1; // Modular inverse doesn't exist
    else return (x % m + m) % m; // Ensure 'x' is positive
}

// solve the linear congruence AX ≡ B (mod N)
void solve_linear_congruence(long long A, long long B, long long N) {
    long long A_inv = inverse(A, N);  // modular inverse of A modulo N

    if (A_inv == -1) {
        cout << -1 ; // No solution
        return;
    }

    // Find a particular solution X0 using the modular inverse
    long long X0 = (A_inv * B) % N;

    // Print all possible solutions in the range [0, k*N-1]
    set<long long> X;
    for (long long k = 0; k < N; ++k)
        X.insert((X0 + k * (N / gcd(A, N))) % N);

    for (auto x: X) cout<<x<<" ";
    cout << endl;
}

int main() {
    // long long A, B, N;
    // cout << "Enter values for A, B, and N: ";
    // cin >> A >> B >> N;

    // solve_linear_congruence(A, B, N);
    long long a = inverse(11, 1e9+7);
    cout<<a;

    return 0;
}
