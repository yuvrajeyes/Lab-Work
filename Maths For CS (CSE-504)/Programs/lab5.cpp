#include <bits/stdc++.h>
using namespace std;

double pow_(double base, long long exponent) {
    double result = 1;
    
    while (exponent > 0) {
        if (exponent & 1)
            result = result * base;
        
        exponent >>= 1;
        base = base * base;
    }
    
    return result;
} 

// Function to calculate binomial coefficient (n choose k)
long long binomialCoefficient(long long n, long long k) {
    // if (k > n)
    //     return 0;
    // if (k == 0 || k == n)
    //     return 1;
    // return binomialCoefficient(n - 1, k - 1) + binomialCoefficient(n - 1, k);
    long long C[k+1];
    memset(C, 0, sizeof(C));
    C[0] = 1;
    for (long long i = 1; i <= n; i++) {
        for (int j = min(i, k); j > 0; j--)
            C[j] = C[j] + C[j-1];
    }
    return C[k];
}


double fairCoinProbability(long long n, long long k) {
    double p_head = 0.5;
    return binomialCoefficient(n, k) * pow_(p_head, k) * pow_(1 - p_head, n - k);
}

double biasedCoinProbability(long long n, long long k) {
    double p_head_biased = 0.3; 
    return binomialCoefficient(n, k) * pow_(p_head_biased, k) * pow_(1 - p_head_biased, n - k);
}

double statisticalDistance(vector<double>& distribution1, vector<double>& distribution2) {
    double distance = 0.0;
    for (size_t i = 0; i < distribution1.size(); i++) {
        distance += (0.5 * fabs(distribution1[i] - distribution2[i]));
    }
    return distance;
}

int main() {
    long long n; // Number of coin tosses
    cout<<"Enter the number of coin tosses: ";
    cin>>n;

    // Calculate probability distributions for fair and biased coins
    vector<double> fairDistribution(n + 1, 0.0);
    vector<double> biasedDistribution(n + 1, 0.0);

    for (long long k = 0; k <= n; k++) {
        fairDistribution[k] = fairCoinProbability(n, k);
        biasedDistribution[k] = biasedCoinProbability(n, k);
    }

    // Output probability distributions
    cout << "Fair Coin Probability Distribution:\n";
    for (long long k = 0; k <= n; ++k) 
        cout << "P(" << k << " heads) = " << fairDistribution[k] << "\n";
    
    cout << "\nBiased Coin Probability Distribution:\n";
    for (long long k = 0; k <= n; ++k) 
        cout << "P(" << k << " heads) = " << biasedDistribution[k] << "\n";
    
    // Calculate and output the statistical distance
    double distance = statisticalDistance(fairDistribution, biasedDistribution);
    cout << "\nStatistical Distance between Distributions: " << distance << "\n";

    return 0;
}
