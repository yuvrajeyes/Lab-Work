#include <iostream>
#include <vector>
using namespace std;

class ModularArithmetic {
private:
    int p;
    int mod(int a, int b) {
        int result = a % b;
        if (result < 0) result += b; 
        return result;
    }

    int modExp(int base, int exponent) {
        long long result = 1;
        while (exponent > 0) {
            if (exponent & 1)
                result = (result * base) % p;
            exponent >>= 1;
            base = (base * base) % p;
        }
        return static_cast<int>(result);;
    }

    int modInverse(int a) {
        return modExp(a, p - 2);
    }

    int add(int a, int b) {
        return mod(a + b, p);
    }

    int subtract(int a, int b) {
        return mod(a - b, p);
    }

    int multiply(int a, int b) {
        return mod(a * b, p);
    }

    int divide(int a, int b) {
        return multiply(a, modInverse(b));
    }

public:
    ModularArithmetic(int prime) : p(prime) {}

    void gaussianElimination(vector<vector<int>>& A, vector<int>& B) {
        int m = A.size();
        int n = A[0].size();
        int lead = 0;
        for (int r = 0; r < m; ++r) {
            if (lead >= n) {
                break;
            }
            int i = r;
            while (A[i][lead] == 0) {
                ++i;
                if (i == m) {
                    i = r;
                    ++lead;
                    if (lead == n) {
                        break;
                    }
                }
            }
            swap(A[i], A[r]);
            int lv = A[r][lead];
            for (int j = 0; j < n; ++j) {
                A[r][j] = divide(A[r][j], lv);
            }
            B[r] = divide(B[r], lv);
            for (int i = 0; i < m; ++i) {
                if (i != r) {
                    int lv = A[i][lead];
                    for (int j = 0; j < n; ++j) {
                        A[i][j] = subtract(A[i][j], multiply(lv, A[r][j]));
                    }
                    B[i] = subtract(B[i], multiply(lv, B[r]));
                }
            }
            ++lead;
        }
    }

    bool isInRangeSpace(const vector<vector<int>>& A, const vector<int>& B) {
        vector<vector<int>> RRE_A = A;
        vector<int> RRE_B = B;
        gaussianElimination(RRE_A, RRE_B);
        for (int i = 0; i < RRE_A.size(); ++i) {
            bool allZero = true;
            for (int j = 0; j < RRE_A[i].size(); ++j) {
                if (RRE_A[i][j] != 0) {
                    allZero = false;
                    break;
                }
            }
            if (allZero && RRE_B[i] != 0) return false;
        }
        return true;
    }

    void backSubstitution(const vector<vector<int>>& A, const vector<int>& B) {
        int n = A[0].size();
        vector<int> solution(n);
        for (int i = n - 1; i >= 0; --i) {
            solution[i] = B[i];
            for (int j = i + 1; j < n; ++j) {
                solution[i] = subtract(solution[i], multiply(A[i][j], solution[j]));
            }
        }
        cout << "Solutions for AX = B:" << endl;
        for (int i = 0; i < n; ++i) {
            cout << "x" << i + 1 << " = " << solution[i] << endl;
        }
    }
};

int main() {
    int p, m, n;
    cout << "Enter the prime number p: ";
    cin >> p;
    cout << "Enter the dimensions of matrix A (m x n): ";
    cin >> m >> n;
    
    vector<vector<int>> A(m, vector<int>(n));
    cout << "Enter the elements of matrix A:" << endl;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            cin >> A[i][j];

    vector<int> B(m);
    cout << "Enter the elements of matrix B:" << endl;
    for (int i = 0; i < m; ++i) cin >> B[i];

    ModularArithmetic mod(p);

    // Check if B belongs to the range space of A
    if (mod.isInRangeSpace(A, B)) {
        cout <<"Matrix B belongs to the range space of matrix A."<<endl;
        // Perform back substitution and print solutions
        mod.backSubstitution(A, B);
    } else {
        cout <<"Matrix B does not belong to the range space of matrix A."<<endl;
    }
    return 0;
}