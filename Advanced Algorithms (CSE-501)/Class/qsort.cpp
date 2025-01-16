#include <bits/stdc++.h>
using namespace std;

int Partition(int A[], int p, int r) {
    int x = A[r]; // Pivot element
    int i = p - 1; // Index of the smaller element

    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[r]); // Place the pivot in its correct position
    return i + 1;
}

void IterativeQuickSort(int A[], int n) {
    stack<pair<int, int>> s;
    s.push({0, n - 1});

    while (!s.empty()) {
        int p = s.top().first;
        int r = s.top().second;
        s.pop();

        if (p < r) {
            int q = Partition(A, p, r);
            s.push({p, q - 1}); // Sort the left subarray
            s.push({q + 1, r}); // Sort the right subarray
        }
    }
}

void RecursiveQuickSort(int A[], int p, int r) {
    if (p < r) {
        int q = Partition(A, p, r);
        RecursiveQuickSort(A, p, q - 1); // Sort the left subarray
        RecursiveQuickSort(A, q + 1, r); // Sort the right subarray
    }
}

int main() {
    int A[] = {12, 4, 5, 6, 7, 3, 1, 15, 2, 10};
    int n = sizeof(A) / sizeof(A[0]);

    RecursiveQuickSort(A, 0, n-1);

    cout << "Sorted Array: ";
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    cout << endl;

    return 0;
}
