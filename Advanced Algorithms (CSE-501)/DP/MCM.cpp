#include<bits/stdc++.h>
using namespace std;

void printMatrix(vector<vector<int>> matrix) {
    int max_width = 0;
    for (const auto& row : matrix) {
        for (int num : row) {
            int width = to_string(num).length();
            max_width = max(max_width, width);
        }
    }

    int N = matrix.size();
    for (int i=1; i<N; i++) {
        for (int j=1; j<N; j++)
            cout<<setw(max_width)<<matrix[i][j]<<" ";
        cout<<endl;
    }
}

void showOrder(vector<vector<int>> &factor, int i, int j, string &s) {
    if (i==j) {
        s += "M";
        s += to_string(i);
        return;
    }
    else {
        int k = factor[i][j];
        s += "(";
        showOrder(factor, i, k, s);
        s += "*";
        showOrder(factor, k+1, j, s);
        s += ")";
    }
}

int divideAndConquer(vector<int>& arr, int i, int j, vector<vector<int>>& factor){
    if(i == j)
        return 0;
    
    int mn = INT_MAX;
    for(int k=i; k<j; k++){
        int ans = divideAndConquer(arr, i, k, factor) + divideAndConquer(arr, k+1, j, factor) + arr[i-1]*arr[k]*arr[j];
        if (mn > ans) {
            mn = ans;
            factor[i][j] = k;
        }
    }
    return mn;
}

int divideAndConquerWithMemoization(vector<int>& arr, int i, int j, vector<vector<int>>& dp, vector<vector<int>>& factor){
    if(i == j)
        return 0;
    if(dp[i][j]!=-1)
        return dp[i][j];
    
    int mn = INT_MAX;
    for(int k=i; k<j; k++){
        int ans = divideAndConquerWithMemoization(arr, i, k, dp, factor) + divideAndConquerWithMemoization(arr, k+1, j, dp, factor) + arr[i-1]*arr[k]*arr[j];
        if (mn > ans) {
            mn = ans;
            factor[i][j] = k;
        }
    }
    return dp[i][j]=mn;
}

int mcmDP(vector<int> &R, vector<vector<int>> &dp, vector<vector<int>> &factor, int N) {
    for(int i=N; i>=1; i--){
        for(int j=i+1; j<N+1; j++){
            int mn = INT_MAX;
            for(int k = i; k<= j-1; k++){
                int ans = dp[i][k]+ dp[k+1][j] + R[i-1]*R[k]*R[j];
                if (mn > ans) {
                    mn = ans;
                    factor[i][j] = k;
                }
            }
            dp[i][j] = mn;
        }
    }
    return dp[1][N];
}

int main() {
    // int N=50;
    // vector<int> R = {10, 15, 20, 15, 10, 20, 25, 30, 35, 10, 25, 15, 30, 10, 15, 20, 15, 10, 20, 25, 30, 35, 10, 25, 15, 30, 
    //                  40, 25, 30, 25, 30, 45, 15, 35, 15, 30, 20, 10, 15, 25, 10, 25, 35, 50, 10, 15, 20, 25, 30, 40, 10};

    int N=4;
    vector<int> R = {20, 50, 3, 50, 2};
    char m = 'M';
    cout<<"The Matrices are:\n";
    for (int i=1; i<=N; i++) {
        cout<<m<<i<<": "<<R[i-1]<<"x"<<R[i]<<endl;
    }
    
    cout<<"\nThe minimum number of multplication required(D&C): ";
    vector<vector<int>> factor(N+1, vector<int> (N+1, 0));
    cout<<divideAndConquer(R, 1, N, factor)<<endl;
    string str = "";
    showOrder(factor, 1, N, str);
    cout<<"Optimal Parenthesization: ";
    cout<<str<<endl;

    // vector<vector<int>> dp(N+1, vector<int> (N+1, -1));
    // cout<<divideAndConquerWithMemoization(R, 1, N, dp, factor)<<endl;

    cout<<"\nThe minimum number of multplication required(DP): ";
    vector<vector<int>> dp1(N+1, vector<int> (N+1, 0));
    cout<<mcmDP(R, dp1, factor, N)<<endl;

    // printMatrix(factor);
    str = "";
    showOrder(factor, 1, N, str);
    cout<<"Optimal Parenthesization: ";
    cout<<str<<endl;

    return 0;
}