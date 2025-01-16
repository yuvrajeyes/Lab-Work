// #include<bits/stdc++.h>
// using namespace std;

// bool mark[1025] = {false};
// vector<int> primes;

// int main() {
//     for (int i = 2; i <= 1024; ++i) {
//         if (!mark[i]) {
//             primes.emplace_back(i);
//             for (int j = i * i; j <= 1024; j += i)
//                 mark[j] = true;
//         }
//     }

//     map<int, int> factors;

//     int N;
//     cout<<"Please Enter an Number (N): ";
//     cin>>N;
//     if (N==1) {
//         cout<<N<<" can not be factorized because it is neither a prime number nor a composite number."<<endl;
//         return 0;
//     }
//     if (mark[N]==false) {
//         cout<<N<<" is a prime number. Therefore can not be factorized."<<endl;
//         return 0;
//     }

//     for (auto x: primes) {
//         if (mark[N]==false) {
//             factors[N]++;
//             break;
//         }
//         while (N>1) {
//             if (N%x==0) {
//                 factors[x]++;
//                 N /= x;
//             }
//             else break;
//         }
//     }

//     string ans = "";
//     for (auto x: factors) {
//         ans += ("(" + to_string(x.first) + "^" + to_string(x.second) + ") * ");
//     }

//     ans.pop_back(); ans.pop_back();

//     cout<<ans<<endl;
//     return 0;
// }


#include<bits/stdc++.h>
using namespace std;

bool isPrime(int n) {
    if (n<=1) return 0;
    else if (n==2) return 1;
    else if ((n&1)==0) return 0;
    else {
        int i = 3;
        while (i*i <= n) {
            if (n % i == 0) return 0;
            i += 2;
        }
        return true;
    }
}

int main() {
    map<int, int> factors;

    int N;
    cout<<"Please Enter an Number (N): ";
    cin>>N;
    if (N==1) {
        cout<<N<<" can not be factorized because it is neither a prime number nor a composite number."<<endl;
        return 0;
    }
    if (isPrime(N)) {
        cout<<N<<" is a prime number. Therefore can not be factorized."<<endl;
        return 0;
    }

    while ((N&1) == 0) {
        factors[2]++;
        N>>=1;
    }

    int i =3;
    while (i <= N && !isPrime(N)) {
        if (N % i == 0) {
            factors[i]++;
            N /= i;
            continue;
        }
        i += 2;
    }
    if (N>1) factors[N] = 1;

    string ans = "";
    for (auto x: factors) {
        ans += ("(" + to_string(x.first) + "^" + to_string(x.second) + ") * ");
    }

    ans.pop_back(); ans.pop_back();

    cout<<ans<<endl;
    return 0;
}