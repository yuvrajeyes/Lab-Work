#include <bits/stdc++.h>
using namespace std;

bool isSafe(int row, int col, vector < string > board, int n) {
    // check for upper diagonal
    int duprow = row, dupcol = col;
    while (row >= 0 && col >= 0) {
        if (board[row][col] == 'Q')
            return false;
        row--; col--;
    }

    // check for row
    col = dupcol;
    row = duprow;
    while (col >= 0) {
        if (board[row][col] == 'Q')
            return false;
        col--;
    }

    // check for lower diagonal
    row = duprow;
    col = dupcol;
    while (row < n && col >= 0) {
        if (board[row][col] == 'Q')
            return false;
        row++; col--;
    }
    return true;
}

void solve(int col, vector < string > & board, vector < vector < string >> & ans, int n) {
    if (col == n) {
    ans.push_back(board);
    return;
    }
    for (int row = 0; row < n; row++) {
    if (isSafe(row, col, board, n)) {
        board[row][col] = 'Q';
        solve(col + 1, board, ans, n);
        board[row][col] = '.';  // backtrack
    }
    }
}

vector < vector < string >> solveNQueens(int n) {
    vector < vector < string >> ans;
    vector < string > board(n);
    string s(n, '.');
    for (int i = 0; i < n; i++) {
        board[i] = s;
    }
    solve(0, board, ans, n);
    return ans;
}

int main() {
  int n = 4; 
  vector < vector < string >> ans = solveNQueens(n);

  if (ans.size()==0) cout<<"No arrangement possible.";

  for (int i = 0; i < ans.size(); i++) {
    cout << "Arrangement " << i + 1 << "\n";
    for (int j = 0; j < 4; j++) {
      cout << ans[i][j] << endl;
    }
    cout << endl;
  }
  return 0;
}