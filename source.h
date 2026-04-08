#include <bits/stdc++.h>

using namespace std;

const int N = 1e4 + 10;
const int mul = 233, Mod = 998244353;

int query(int x, int y, int z);

int A[N];

int guess(int n, int Taskid) {
    memset(A, 0, sizeof A);

    vector<int> P(n);
    iota(P.begin(), P.end(), 1);
    mt19937 rng(1337);
    shuffle(P.begin(), P.end(), rng);

    int P5[5];
    for(int i=0; i<5; ++i) P5[i] = P[i];

    int q_val[5][5][5];
    for(int i=0; i<5; ++i) {
        for(int j=i+1; j<5; ++j) {
            for(int k=j+1; k<5; ++k) {
                q_val[i][j][k] = query(P5[i], P5[j], P5[k]);
                q_val[i][k][j] = q_val[i][j][k];
                q_val[j][i][k] = q_val[i][j][k];
                q_val[j][k][i] = q_val[i][j][k];
                q_val[k][i][j] = q_val[i][j][k];
                q_val[k][j][i] = q_val[i][j][k];
            }
        }
    }

    int p[5] = {0, 1, 2, 3, 4};
    int u = -1, v = -1, M = -1;
    do {
        long long q123 = q_val[p[0]][p[1]][p[2]];
        long long q125 = q_val[p[0]][p[1]][p[4]];
        long long q345 = q_val[p[2]][p[3]][p[4]];
        
        long long sum2 = q123 + q125 - q345;
        if (sum2 <= 0 || sum2 % 2 != 0) continue;
        long long a0 = sum2 / 2;
        long long a2 = q123 - a0;
        long long a4 = q125 - a0;
        long long a3 = q_val[p[0]][p[1]][p[3]] - a0;
        long long a1 = q_val[p[1]][p[3]][p[4]] - a4;
        
        if (a0 >= a1 || a1 >= a2 || a2 >= a3 || a3 >= a4) continue;
        
        long long A_cand[5];
        A_cand[p[0]] = a0;
        A_cand[p[1]] = a1;
        A_cand[p[2]] = a2;
        A_cand[p[3]] = a3;
        A_cand[p[4]] = a4;
        
        bool ok = true;
        for(int i=0; i<5; ++i) {
            for(int j=i+1; j<5; ++j) {
                for(int k=j+1; k<5; ++k) {
                    long long mx = max({A_cand[i], A_cand[j], A_cand[k]});
                    long long mn = min({A_cand[i], A_cand[j], A_cand[k]});
                    if (mx + mn != q_val[i][j][k]) {
                        ok = false; break;
                    }
                }
                if (!ok) break;
            }
            if (!ok) break;
        }
        
        if (ok) {
            for(int i=0; i<5; ++i) A[P5[i]] = A_cand[i];
            u = P5[p[0]];
            v = P5[p[1]];
            M = P5[p[4]];
            break;
        }
    } while(next_permutation(p, p+5));

    for(int k=5; k<n; ++k) {
        int i = P[k];
        int qi = query(u, v, i);
        long long S = A[u] + A[v];
        if (qi > S) {
            A[i] = qi - A[u];
            if (A[i] > A[M]) M = i;
        } else if (qi < S) {
            A[i] = qi - A[v];
            v = u;
            u = i;
        } else {
            int q_extra = query(i, v, M);
            A[i] = q_extra - A[M];
            v = i;
        }
    }

    int ret = 0;
    for (int i = n; i; i--) ret = 1ll * (ret + A[i]) * mul % Mod;
    return ret;
}
