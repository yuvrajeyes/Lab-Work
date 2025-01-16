#include <iostream>
#include <cmath>
using namespace std;


class Bounds {
public:
    double markovBound(int n, int x, int y) {
        double mean = static_cast<double>(x * n) / 3; 
        return static_cast<double>(n) / (y * mean);  
    }
    double chebyshevBound(int n, int x, int y) {
        double variance = static_cast<double>(x * n) * (2 * n) / 9; 
        double mean = static_cast<double>(x * n) / 3;               
        double stdDev = sqrt(variance);                            
        double k = (mean - static_cast<double>(y * n)) / stdDev;   
        return 1 / (1 + pow(k, 2));                                
    }

    double chernoffBound(int n, int x, int y, double delta) {
        double mean = static_cast<double>(x * n) / 3; 
        double k = (mean - static_cast<double>(y * n)) / mean; 
        return exp(-k * delta) / pow(1 + delta, (1 + delta)*mean); 
    }
};

int main() {
    int n, x, y;
    double delta;
        
    cout << "Enter the value of n, x, y: ";
    cin >> n >> x >> y;
        
    cout << "Enter the value of delta (for Chernoff bound): ";
    cin >> delta;

    Bounds bounds;
    
    cout<<"Markov's Upper Bound: "<<bounds.markovBound(n, x, y)<<endl;
    cout<<"Chebyshev's Upper Bound: "<<bounds.chebyshevBound(n, x, y)<<endl;
    cout<<"Chernoff's Upper Bound with delta = "<<delta<<": "<<bounds.chernoffBound(n, x, y, delta)<<endl;

    return 0;
}
