#include <bits/stdc++.h>
using namespace std;

bool isSafe(int v, vector<vector<int>>& G, vector<int>& colors, int c) {
    for (int u : G[v])
        if (colors[u] == c)
            return false;
    return true;
} 
bool graphColoringUtil(vector<vector<int>>& G, int k, vector<int>& colors, int v) {
    int n = G.size();
    if (v == n)
        return true; // All vertices are colored
    for (int c = 1; c <= k; ++c)
        if (isSafe(v, G, colors, c)) {
            colors[v] = c;
            // Recur for the next vertex
            if (graphColoringUtil(G, k, colors, v + 1))
                return true;
            // If assigning color c doesn't lead to a solution, backtrack
            colors[v] = 0;
        }
    return false; // No color can be assigned to this vertex
}
bool graphColoring(vector<vector<int>>& G, int k) {
    int n = G.size();
    vector<int> colors(n, 0); // Initialize colors to 0 (no color)
    if (graphColoringUtil(G, k, colors, 0)) {
        cout << "YES, the graph can be colored with at most " << k << " colors." << endl;
        return true;
    } else {
        cout << "NO, the graph cannot be colored with at most " << k << " colors." << endl;
        return false;
    }
}

int main() {
    int n = 5; // Number of vertices
    vector<vector<int>> G = {{1, 4}, {0, 2, 3}, {1, 3, 5}, {1, 2, 4}, {0, 2, 3}}; // Adjacency list
    int k = 3; // Maximum number of colors
    graphColoring(G, k);

    return 0;
}
