#include <bits/stdc++.h>
using namespace std;

bool dfs(vector<vector<int>>& graph, vector<int>& colors, int node, int color) {
    colors[node] = color;
    for (int neighbor : graph[node])
        if (colors[neighbor] == color or (colors[neighbor] == -1 && !dfs(graph, colors, neighbor, 1 - color)))
            return false; // Conflict, the graph is not bipartite
    return true;
}

bool isBipartite(vector<vector<int>>& graph) {
    int numNodes = graph.size();
    vector<int> colors(numNodes, -1); // -1 represents uncolored, 0 and 1 represent two groups

    for (int node = 0; node < numNodes; ++node)
        if (colors[node] == -1 && !dfs(graph, colors, node, 0))
            return false;

    return true;
}

int main() {
    vector<vector<int>> graph = { {1, 3}, {0, 2}, {1, 3}, {0, 2} };

    isBipartite(graph) ? cout << "The graph is bipartite." : cout << "The graph is not bipartite." << endl;

    return 0;
}
