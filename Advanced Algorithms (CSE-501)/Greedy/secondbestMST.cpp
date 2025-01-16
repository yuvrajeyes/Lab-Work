#include <bits/stdc++.h>
using namespace std;

class UnionFind {
private:
    vector<int> parent;

public:
    UnionFind(int n) {
        parent.resize(n + 1);
        for (int i = 1; i <= n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    void unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x != y)
            parent[x] = y;
    }
};

struct Edge {
    int src, dest, weight; 
};

bool cmp(const Edge& x, const Edge& y) {
    return x.weight < y.weight;
}

class MinimumSpanningTree {
private:
    vector<Edge> edges;
    vector<int> present;
    int V, E;
    int cost;

public:
    MinimumSpanningTree(int v, int e, const vector<Edge>& graphEdges) {
        V = v, E = e, cost = 0;
        for (int i = 0; i < E; i++)
            edges.push_back(graphEdges[i]);
        
        // Time Complexity: O(ElogE) = O(ElogV^2) = O(2ElogV) =  O(ElogV)
        sort(edges.begin(), edges.end(), cmp);
    }

    // Time Complexity: O(E)
    int findMST(vector<Edge>& mstEdges) {
        UnionFind uf(V);
        for (int i = 0; i < E; i++) {
            int x = edges[i].src, y = edges[i].dest;
            if (uf.find(x) != uf.find(y)) {
                uf.unite(x, y);
                present.push_back(i);
                cost += edges[i].weight;
                mstEdges.push_back(edges[i]);
            }
        }
        return cost;
    }

    // Time Complexity: O(VE)
    int findSecondBestMST(vector<Edge>& secondMstEdges) {
        int sec_best_mst = INT_MAX;
        for (int j = 0; j < present.size(); j++) {
            int curcost = 0, edg=0;
            UnionFind uf(V);
            vector<Edge> second;
            for (int i = 0; i < E; i++) {
                if (i == present[j])
                    continue;
                int x = edges[i].src, y = edges[i].dest;
                if (uf.find(x) != uf.find(y)) {
                    uf.unite(x, y);
                    curcost += edges[i].weight;
                    second.push_back(edges[i]);
                    edg++;
                }
            }
            if (sec_best_mst > curcost)
                if (curcost != cost) {
                    sec_best_mst = curcost;
                    secondMstEdges = second;
                }
        }
        return sec_best_mst;
    }
};

int main() {
    int V = 6, E = 10;
    vector<Edge> graphEdges = {{1, 2, 2}, {1, 5, 13}, {1, 6, 2}, {2, 3, 7}, {2, 6, 2}, 
                            {3, 4, 36}, {3, 6, 14}, {4, 5, 39}, {4, 6, 34}, {5, 6, 27} };

    MinimumSpanningTree mst(V, E, graphEdges);

    cout << "Minimum Spanning Tree (MST):" << endl;
    vector<Edge> mstEdges;
    int mstcost = mst.findMST(mstEdges);
    for (const Edge& edge : mstEdges)
        cout << edge.src << " - " << edge.dest << " : " << edge.weight << "\n";
    
    cout << "MST Weight: " << mstcost << endl << endl;

    cout << "Second Best Minimum Spanning Tree (MST):" << endl;
    vector<Edge> secondMstEdges;
    int secondMstcost = mst.findSecondBestMST(secondMstEdges);
    for (const Edge& edge : secondMstEdges) {
        cout << edge.src << " - " << edge.dest << " : " << edge.weight << "\n";
    }
    cout << "Second MST Weight: " << secondMstcost << "\n";

    return 0;
}