#include <bits/stdc++.h>
using namespace std;

void dfs(int node, vector<int> &visited, stack<int> &st, vector<int> adj[]) {
    visited[node] = 1;
    for (int i: adj[node])
        if (!visited[i])
            dfs(i, visited, st, adj);
    st.push(node);
}

int main() {
    int N=8;
    vector<int> adj[N+1];
    adj[1].push_back(2);
    adj[1].push_back(5);
    adj[2].push_back(3);
    adj[2].push_back(6);
    adj[3].push_back(4);
    adj[4].push_back(7);
    adj[5].push_back(6);
    adj[5].push_back(8);
    adj[6].push_back(3);
    adj[6].push_back(7);
    adj[6].push_back(8);

    stack<int> st;
    vector<int> visited(N+1, 0);

    for (int i=1; i<=N; i++)
        if (visited[i]==0)
            dfs(i, visited, st, adj);

    while (!st.empty()) {
        cout<<st.top()<<" ";
        st.pop();
    }
    return 0;
}