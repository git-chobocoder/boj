#include <bits/stdc++.h>
#define x first
#define y second
using namespace std;

typedef long long ll;
typedef pair<ll, ll> p;

ll area(p a, p b, p c, bool flag = 0){
    ll res = a.x*b.y + b.x*c.y + c.x*a.y;
    res -= b.x*a.y + c.x*b.y + a.x*c.y;
    if(flag) return res;
    return abs(res);
}

int ccw(p a, p b, p c){
    ll res = area(a, b, c, 1);
    if(res > 0) return 1;
    if(res) return -1;
    return 0;
}

ll dst(p a, p b){
	ll dx = a.x - b.x;
	ll dy = a.y - b.y;
	return dx*dx + dy*dy;
}

istream& operator >> (istream& is, p &a){
    is >> a.x >> a.y;
    return is;
}

ostream& operator << (ostream& os, p &a){
    os << a.x << " " << a.y;
    return os;
}

int n;
p p1, p2; //up down
vector<p> v;

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0);
    cin >> n >> p1 >> p2;
    v.push_back(p1); v.push_back(p2);
    for(int i=0; i<n; i++){
        ll y, s, e; cin >> y >> s >> e;
        int l = ccw(p2, p1, {s, y});
		int r = ccw(p2, p1, {e, y});
		if(r == 1) v.push_back({e, y});
		else if(l == -1) v.push_back({s, y});
    }

	if(v.size() <= 2){
		cout << 0; return 0;
	}

	swap(v[0], *min_element(v.begin(), v.end()));
	sort(v.begin()+1, v.end(), [&](p &a, p &b){
		int cw = ccw(v[0], a, b);
		if(cw) return cw > 0;
		return dst(v[0], a) < dst(v[0], b);
	});

	vector<p> hull;
	for(auto i : v){
		while(hull.size() >= 2 && ccw(hull[hull.size()-2], hull.back(), i) <= 0) hull.pop_back();
		hull.push_back(i);
	}

	n = hull.size(); v = hull;

	int i = 0, j = 1, pt = 1;
	ll ans = 0, now = 0;
	while(i != n){
		while(i != pt){
			if(area(v[i], v[j], v[(pt+1)%n]) < now) j = (j + 1) % n;
			else pt = (pt + 1) % n;
			now = area(v[i], v[j], v[pt]);
			ans = max(ans, now);
		}
		i++; j = (i + 1) % n; pt = (j + 1) % n;
	}
	if(ans & 1) cout << ans/2 << ".5";
	else cout << ans/2;
}