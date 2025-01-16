#include <bits/stdc++.h>
using namespace std;
#define N 5

struct Node {
	vector<pair<int, int>> path;
	int reducedMatrix[N][N], cost, vertex, level;
};

void printMatrix(int reducedMatrix[N][N]) {
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            cout<<reducedMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

Node* newNode(int parentMatrix[N][N], vector<pair<int, int>> const &path, int level, int i, int j) {
	Node* node = new Node;
	node->path = path;
	if (level != 0)
		node->path.push_back(make_pair(i, j));

	memcpy(node->reducedMatrix, parentMatrix,
		sizeof node->reducedMatrix);

	for (int k = 0; level != 0 && k < N; k++) {
		node->reducedMatrix[i][k] = INT_MAX;  // set outgoing edge of vertex i to INT_MAX
		node->reducedMatrix[k][j] = INT_MAX;  // set incoming edge to vertex j to INT_MAX
	}

	node->reducedMatrix[j][0] = INT_MAX;
	node->level = level;
	node->vertex = j;
	return node;
}

int rowReduction(int reducedMatrix[N][N], int row[N]) {
	fill_n(row, N, INT_MAX);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] < row[i])
				row[i] = reducedMatrix[i][j];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] != INT_MAX && row[i] != INT_MAX)
				reducedMatrix[i][j] -= row[i];
	return 0;
}

int columnReduction(int reducedMatrix[N][N], int col[N]) {
	fill_n(col, N, INT_MAX);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] < col[j])
				col[j] = reducedMatrix[i][j];

	for (int i = 0; i < N; i++) 
		for (int j = 0; j < N; j++)
			if (reducedMatrix[i][j] != INT_MAX && col[j] != INT_MAX)
				reducedMatrix[i][j] -= col[j];
	return 0;
}

int calculateCost(int reducedMatrix[N][N]) {
	int cost = 0;
	int row[N];
	rowReduction(reducedMatrix, row);
	int col[N];
	columnReduction(reducedMatrix, col);
	for (int i = 0; i < N; i++)
		cost += (row[i] != INT_MAX) ? row[i] : 0,
			cost += (col[i] != INT_MAX) ? col[i] : 0;
	
	return cost;
}

void printPath(vector<pair<int, int>> const &list) {
	for (int i = 0; i < list.size(); i++)
		cout << list[i].first + 1 << " ";
    cout << list[0].first+1;
}

struct comp {
	bool operator()(const Node* lhs, const Node* rhs) const {
		return lhs->cost > rhs->cost;
	}
};

int tsp(int costMatrix[N][N]) {
	priority_queue<Node*, vector<Node*>, comp> pq;
	vector<pair<int, int>> v;
	Node* root = newNode(costMatrix, v, 0, -1, 0);
	root->cost = calculateCost(root->reducedMatrix);
	pq.push(root);
	while (!pq.empty()) {
		Node* mn = pq.top();
		pq.pop();
		int i = mn->vertex;
		if (mn->level == N - 1) {
        	mn->path.push_back(make_pair(i, 0));
			printPath(mn->path);
			cout << "\nTotal cost is ";
			return mn->cost;
		}
        
		for (int j = 0; j < N; j++) {
			if (mn->reducedMatrix[i][j] != INT_MAX) {
				Node* child = newNode(mn->reducedMatrix, mn->path,
					mn->level + 1, i, j);
				child->cost = mn->cost + mn->reducedMatrix[i][j]
							+ calculateCost(child->reducedMatrix);
				pq.push(child);
			}
		}
		delete mn;
	}
	return 0;
}

int main() {
	int costMatrix[N][N] = {
		{ INT_MAX, 10, 8, 19, 7 },
		{ 10, INT_MAX, 10, 25, 6},
		{ 8, 10, INT_MAX, 8, 19 },
		{ 9, 25, 8, INT_MAX, 6  },
		{ 7, 6, 19, 6, INT_MAX  }
	};
	cout << "Path: " << tsp(costMatrix);
	return 0;
}