#include <bits/stdc++.h>
using namespace std;


void dfs(int node, stack<int> &st, vector<int> &vis, vector<int> adj[]) {
    vis[node] = 1; 
    for(auto it: adj[node])
        if(!vis[it])
            dfs(it, st, vis, adj);
    st.push(node); 
}

void revDFS(int node, vector<int> &vis, vector<int> transpose[]) {
    cout << node << " "; 
    vis[node] = 1; 
    for(auto it: transpose[node])
        if(!vis[it])
            revDFS(it, vis, transpose); 
}


int main() {
    int N=6;
	vector<int> adj[N+1]; 
	adj[1].push_back(3);
	adj[2].push_back(1);
	adj[3].push_back(2);
	adj[3].push_back(5);
	adj[4].push_back(6);
	adj[5].push_back(4);
	adj[6].push_back(5);
	
	stack<int> st;
	vector<int> vis(N+1, 0); 
	for(int i=1; i<=N; i++)
	    if(!vis[i])
	        dfs(i, st, vis, adj); 
	
	vector<int> transpose[N+1]; 
	for(int i=1; i<=N; i++) {
	    vis[i] = 0;
	    for(auto it: adj[i])
	        transpose[it].push_back(i); 
	}
	
	while(!st.empty()) {
	    int node = st.top();
	    st.pop();
	    if(!vis[node]) {
	        cout << "SCC: ";
	        revDFS(node, vis, transpose); 
	        cout << endl; 
	    }
	}
	return 0;
}