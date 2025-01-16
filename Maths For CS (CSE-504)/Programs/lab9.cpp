#include <iostream>
#include <vector>

using namespace std;

// Class to represent a matrix
class Matrix {
private:
    vector<vector<int>> data; // Data of the matrix
    int p; // Prime modulus

    // Function to perform modulo operation
    int mod(int a, int b) {
        int result = a % b;
        if (result < 0) result += b;      
        return result;
    }

    // Function to perform subtraction in Zp
    int subtract(int a, int b) {
        return mod(a - b, p);
    }

    int multiply(int a, int b) {
        return mod(a * b, p);
    }

    // Function to calculate modular exponentiation (a^b mod p)
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

    // Function to calculate modular inverse (a^(-1) mod p)
    int modInverse(int a) {
        return modExp(a, p - 2);
    }

public:
    // Constructor to initialize the matrix with given dimensions and modulus
    Matrix(int m, int n, int prime) : data(m, vector<int>(n)), p(prime) {}

    // Method to set the element at specified row and column
    void setElement(int row, int col, int value) {
        data[row][col] = value % p; // Ensure element belongs to Z_p
    }

    // Method to get the element at specified row and column
    int getElement(int row, int col) const {
        return data[row][col];
    }

    // Method to print the matrix
    void printMatrix() const {
        for (const auto& row : data) {
            for (int elem : row)
                cout << elem << " ";
            cout << endl;
        }
    }

    void rowReduce() {
        int n = data.size();
        for (int col = 0; col < data.size(); ++col) {
            // // Make the pivot element 1
            int pivot = data[col][col];
            int inv = modInverse(pivot);
            for (int j = col; j < n + 1; ++j) 
                data[col][j] = multiply(data[col][j], inv);

            // Make other elements of the current column 0
            for (int row = col+1; row < n; ++row) {
                int factor = data[row][col];
                for (int j = col; j < n + 1; ++j) {
                    int prod = multiply(factor, data[col][j]);
                    data[row][j] = subtract(data[row][j], prod);
                }
            }
        }
    }
};

int main() {
    int p;
    cout << "Enter the prime number p: ";
    cin >> p;

    int m, n;
    cout << "Enter the dimensions of the matrix (m x n): ";
    cin >> m >> n;

    Matrix A(m, n, p); // Create a matrix with prime modulus p

    cout << "Enter the elements of the matrix A (mod " << p << "):\n";
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int value;
            cin >> value;
            A.setElement(i, j, value);
        }
    }

    cout << "\nOriginal matrix A:\n";
    A.printMatrix(); // Output the original matrix

    A.rowReduce(); // Perform row reduction

    cout << "\nRow Reduced Echelon Form of matrix A:\n";
    A.printMatrix(); // Output the row-reduced echelon form

    return 0;
}