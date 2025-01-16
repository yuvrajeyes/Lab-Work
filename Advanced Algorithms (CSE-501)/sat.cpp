#include<bits/stdc++.h>
using namespace std;

int n=6;
vector<vector<int>> graph(n), transpose_graph(n);
vector<bool> used;
vector<int> order, comp;
vector<bool> assignment;

void dfs1(int v) {
   used[v] = true;
   for (int u : graph[v]) {
       if (!used[u])
           dfs1(u);
   }
   order.push_back(v);
}

void dfs2(int v, int cl) {
   comp[v] = cl;
   for (int u : transpose_graph[v]) {
       if (comp[u] == -1)
           dfs2(u, cl);
   }
}

bool solve_2SAT() {
   order.clear();
   used.assign(n, false);
   for (int i = 0; i < n; ++i) {
       if (!used[i])
           dfs1(i);
   }

   comp.assign(n, -1);
   for (int i = 0, j = 0; i < n; ++i) {
       int v = order[n - i - 1];
       if (comp[v] == -1)
           dfs2(v, j++);
   }

   assignment.assign(n / 2, false);
   for (int i = 0; i < n; i += 2) {
       if (comp[i] == comp[i + 1])
           return false;
       assignment[i / 2] = comp[i] > comp[i + 1];
   }
   return true;
}

void add_disjunction(int a, bool na, int b, bool nb) {
   a = 2*a ^ na;
   b = 2*b ^ nb;
   int neg_a = a ^ 1;
   int neg_b = b ^ 1;
   graph[neg_a].push_back(b);
   graph[neg_b].push_back(a);
   transpose_graph[b].push_back(neg_a);
   transpose_graph[a].push_back(neg_b);
}

int main() {
    // Add clause (A OR NOT B)
    add_disjunction(0, false, 1, true); // A -> B and B -> A
    // Add clause (NOT A OR B)
    add_disjunction(0, true, 1, false); // B -> NOT A and NOT A -> B
    // Add clause (A OR NOT C)
    add_disjunction(0, false, 2, false); // NOT A -> NOT B and NOT B -> NOT A
    // Add clause (NOT A OR NOT B)
    add_disjunction(0, true, 1, true); // NOT A -> NOT B and NOT B -> NOT A

   if (solve_2SAT()) {
       cout << "YES" << endl;
       for (int i = 0; i < n / 2; ++i) {
           cout << (assignment[i] ? "true" : "false") << " ";
       }
   } else {
       cout << "No" << endl;
   }

   return 0;
}