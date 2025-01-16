#include <bits/stdc++.h>
using namespace std;

void print(vector<int> &low) {
    for (auto x: low) {
        cout<<x<<" ";
    }
    cout<<endl;
}

void DFS(int p, const vector<vector<int>>& adj_matrix, vector<int>& discovery_time, vector<int>& low, 
        vector<int>& parent, vector<bool>& is_articulation_point) {
    int children = 0;
    static int time = 0;
    discovery_time[p] = low[p] = ++time;
    cout<<"low : "; print(low);
    for (int c = 0; c < adj_matrix.size(); c++)
        if (adj_matrix[p][c] == 1)
            if (discovery_time[c] == -1) {
                children++;
                parent[c] = p;
                DFS(c, adj_matrix, discovery_time, low, parent, is_articulation_point);
                low[p] = min(low[p], low[c]);
 
                if (low[c] >= discovery_time[p] && parent[p] != -1)
                    is_articulation_point[p] = true;
            } 
            else if (c != parent[p])
                low[p] = min(low[p], discovery_time[c]);
    
    if (parent[p] == -1 && children > 1)
        is_articulation_point[p] = true;
}

bool isBiconnected(const vector<vector<int>>& adj_matrix) {
    int n = adj_matrix.size();

    vector<int> discovery_time(n, -1), low(n, -1), parent(n, -1);
    vector<bool> is_articulation_point(n, false);

    for (int p = 0; p < n; p++)
        if (discovery_time[p] == -1)
            DFS(p, adj_matrix, discovery_time, low, parent, is_articulation_point);

    cout<<"discovery_time : "; print(discovery_time);
    cout<<"low : "; print(low);
    
    for (int p = 0; p < n; p++)
        if (is_articulation_point[p])
            return false; // Graph is not biconnected

    return true; // Graph is biconnected
}

int main() {
    // vector<vector<int>> adj_matrix = {
    //     {0, 1, 1, 1, 0},
    //     {1, 0, 1, 0, 0},
    //     {1, 1, 0, 1, 1},
    //     {1, 0, 1, 0, 1},
    //     {0, 0, 1, 1, 0}
    // };

    // isBiconnected(adj_matrix) ? cout << "Yes, the graph is biconnected.\n" : cout << "No, the graph is not biconnected.\n";

    vector<vector<int>> adj_matrix = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 1, 1},
        {0, 0, 1, 0, 1},
        {0, 0, 1, 1, 0}
    };

    isBiconnected(adj_matrix) ? cout << "Yes, the graph is biconnected.\n" : cout << "No, the graph is not biconnected.\n";
    return 0;
}