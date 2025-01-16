#include<stdio.h>
using namespace std;

int gcd(int a, int b) {
    if (a==0) return b;
    if (b % a == 0) return a;
    return gcd(b % a, a);
}

int gcd(int a, int b, int &x, int &y) {
    if (a == 0) {
        x = 1;
        y = 0;
        return b;
    }
    int x1, y1;
    int d = gcd(b % a, a, x1, y1);
    x = y1;
    y = x1 - y1 * (b / a);
    return d;
}

int main() {
    int a, b, x, y;
    bool f_a = false, f_b = false;
    printf("Please Enter two numbers (a & b) : ");
    scanf("%d %d", &a, &b);

    if (a < 0) f_a = true, a = -a;
    if (b < 0) f_b = true, b = -b;

    if (a>b) {
        int temp = a;
        a = b;
        b = temp;
    }

    printf("gcd(%d, %d) = %d\n", a, b, gcd(a, b, x, y));

    if (f_a) x = -x;
    if (f_b) y = -y;
    printf("x = %d, y = %d\n", y, x);

    return 0;
}