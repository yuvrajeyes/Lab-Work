#include <bits/stdc++.h>
using namespace std;
#define N 6

int rowNum[] = {-1, 0, 0, 1};
int colNum[] = {0, -1, 1, 0};

struct Point {
    int x;
    int y;
};

struct queueNode {
    Point pt;
    int dist;
    vector<Point> path;  // Store the path to the current cell
};

bool isValid(int row, int col){
    return (row >= 0) && (row < N) && (col >= 0) && (col < N);
}

void printPath(vector<Point>& path) {
	int mat[N][N] = {0};
    for (const Point& p : path)
		mat[p.x][p.y] = 1;
    
    cout << "Solution : " << endl;
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
			cout<< mat[i][j]<<" ";
		cout<<endl;
	}
}

int BFS(int mat[][N], Point src, Point dest) {
    if (!mat[src.x][src.y] || !mat[dest.x][dest.y])
        return -1;

    bool visited[N][N];
    memset(visited, false, sizeof visited);

    visited[src.x][src.y] = true;

    queue<queueNode> q;
    queueNode s = {src, 0, {src}};  // Initialize path with the source
    q.push(s);

    while (!q.empty()) {
        queueNode curr = q.front();
        Point pt = curr.pt;

        if (pt.x == dest.x && pt.y == dest.y && mat[dest.x][dest.y]!=0) {
            printPath(curr.path);
            return curr.dist;
        }

        q.pop();

        for (int i = 0; i < 4; i++) {
            int row = pt.x + rowNum[i];
            int col = pt.y + colNum[i];

            if (isValid(row, col) && mat[row][col] && !visited[row][col]) {
                visited[row][col] = true;
                vector<Point> newPath = curr.path;
                newPath.push_back({row, col});  // Append the new cell to the path
                queueNode Adjcell = { {row, col}, curr.dist + 1, newPath };
                q.push(Adjcell);
            }
        }
    }

    return -1;
}

int main() {
    int mat[N][N] = { {1, 0, 1, 1, 1, 1},
                     {1, 0, 1, 0, 1, 1},
                     {1, 1, 1, 0, 1, 1},
                     {0, 0, 0, 0, 1, 0},
                     {1, 1, 1, 0, 1, 1},
                     {1, 0, 1, 1, 1, 1}
    };

    Point source = {0, 0};
    Point dest = {5, 5};

    int dist = BFS(mat, source, dest);

    if (dist != -1)
        cout << "Shortest Path is : " << dist<<endl;
    else
        cout << "Shortest Path doesn't exist"<<endl;

    return 0;
}
