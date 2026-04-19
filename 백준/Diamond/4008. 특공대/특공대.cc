#include <iostream>

using namespace std;

typedef long long int ll;
typedef long double ld;

const int MAX_N = 1000000;

// <문제>
// 전체 병사 수 N, 계수 a,b,c
int N, a, b, c;
// 병사 전투력 X[i]
int X[MAX_N];

// 누적합 S[i]
ll S[MAX_N];
// dp[i]
ll dp[MAX_N];

// 직선의 방정식
// f(x) = ax + b, x >= s;
struct LinearFunc { 
    ll a, b;
    ld s;
    LinearFunc(): LinearFunc(1, 0){}
    LinearFunc(ll _a, ll _b) : a(_a), b(_b), s(0) {}
};

// 두 직선의 교점
inline ld cross(LinearFunc &f, LinearFunc &g) {
    return 1.0 * (g.b - f.b)/(f.a - g.a);
}

LinearFunc f[MAX_N];

void input() {
    cin >> N;
    cin >> a >> b >> c;
    
    cin >> X[0]; S[0] = X[0];
    for(int i=1; i<N; i++) {
        cin >> X[i];
        S[i] = S[i-1] + X[i];
    }
}

void solve() {
    int top = 0;
    
    dp[0] = a*X[0]*X[0] + b*X[0] + c;
    for(int i=1; i<N; i++) {
        ll _a = -2*a*S[i-1];
        ll _b = a*S[i-1]*S[i-1] - b*S[i-1] + dp[i-1];
        LinearFunc g(_a, _b);
        
        while(top > 0) {
            g.s = cross(f[top-1], g);
            if(g.s > f[top-1].s)
                break;
            --top;
        }
        f[top++] = g;
        
        ll x = S[i];
        int fpos = top-1;
        if(x < f[top-1].s) {
            int lo = 0, hi = top - 1;
            while(lo + 1 < hi) {
                int mid = (lo+hi)/2;
                (x < f[mid].s ? hi : lo) = mid;
            }
            fpos = lo;
        }
        dp[i] = max(f[fpos].a * x + f[fpos].b, (ll)0) + a*S[i]*S[i] + b*S[i] + c;
    }
    
    cout << dp[N-1];
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    
    input();
    
    solve();
    
    return 0;
}