#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
using namespace std;

int lcs(string lotteryId, string winnerId, int k) {
    int n1 = lotteryId.length(), n2 = winnerId.length();
    vector<int> prev(n2+1, 0), cur(n2+1, 0);

    for (int ind1=1; ind1<=n1; ind1++) {
        for (int ind2=1; ind2<=n2; ind2++) {
            if (lotteryId[ind1-1]==winnerId[ind2-1]) {
                cur[ind2] = 1 + prev[ind2-1];
            }
            else {
                cur[ind2] = max(prev[ind2], cur[ind2-1]);
            }
        }
        prev = cur;
    }
    return k + prev[n2];
}

int main() {
    string s1 = "fpelqanxyk";
    string s2 = "hackerrank";
    int k = 6;

    cout<<lcs(s1, s2, k);
    return 0;
}