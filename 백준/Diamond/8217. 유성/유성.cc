#define MAX 1000000007
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

int lazy[1048577];

void update(int L, int R, int qL, int qR, int qVal, int sI) {
	if (R < qL || qR < L) return;
	if (qL <= L && R <= qR) lazy[sI] = min(lazy[sI] + qVal, MAX);
	else update(L, (L + R) / 2, qL, qR, qVal, sI * 2), update((L + R) / 2 + 1, R, qL, qR, qVal, sI * 2 + 1);
}

int pointval(int qI) {
	int L = 1, R = 524288, sI = 1;
	while (L < R) {
		if (lazy[sI]) {
			lazy[sI * 2] = min(lazy[sI * 2] + lazy[sI], MAX), lazy[sI * 2 + 1] = min(lazy[sI * 2 + 1] + lazy[sI], MAX);
			lazy[sI] = 0;
		}
		int mid = (L + R) / 2;
		if (mid < qI) L = mid + 1, sI = sI * 2 + 1;
		else R = mid, sI = sI * 2;
	}
	return lazy[sI];
}

struct comet {
	int L, R, qVal;
	comet() {};
	comet(int L, int R, int qVal) {
		this->L = L, this->R = R, this->qVal = qVal;
	}
};

int C;
comet comets[300000];

int N; // sector 수
vector<int> sectors[300001]; // [q]: q번 쿼리의 섹터

struct query {
	int idx;
	int low, high, mid;
	int goal;
};

bool qcomp(query& q1, query& q2) {
	return q1.mid < q2.mid;
}
bool qcomp2(query& q1, query& q2) {
	return q1.idx < q2.idx;
}

int Q;
query queries[300000];

void PBS() {
	memset(lazy, 0, sizeof(lazy));
	sort(queries, queries + Q, qcomp);

	int ci = 0, qi = 0;
	while (ci < C && qi < Q) {
		if (ci <= queries[qi].mid) {
			if (comets[ci].L <= comets[ci].R) {
				update(1, 524288, comets[ci].L, comets[ci].R, comets[ci].qVal, 1);
			}
			else {
				update(1, 524288, comets[ci].L, 524288, comets[ci].qVal, 1), update(1, 524288, 1, comets[ci].R, comets[ci].qVal, 1);
			}
			
			ci++;
		}
		else {
			if (queries[qi].low < queries[qi].high) {
				int total = 0;
				for (auto& x : sectors[queries[qi].idx]) {
					total = min(total + pointval(x), MAX);
				}

				if (total < queries[qi].goal) queries[qi].low = queries[qi].mid + 1;
				else queries[qi].high = queries[qi].mid;

				queries[qi].mid = (queries[qi].low + queries[qi].high) / 2;
			}
			qi++;
		}
	}
	while (qi < Q) {
		if (queries[qi].low < queries[qi].high) {
			int total = 0;
			for (auto& x : sectors[queries[qi].idx]) {
				total = min(total + pointval(x), MAX);
			}

			if (total < queries[qi].goal) queries[qi].low = queries[qi].mid + 1;
			else queries[qi].high = queries[qi].mid;

			queries[qi].mid = (queries[qi].low + queries[qi].high) / 2;
		}
		qi++;
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> Q >> N;
	for (int i = 1; i <= N; i++) {
		int q; cin >> q;
		sectors[q].emplace_back(i);
	}

	for (int i = 0; i < Q; i++) {
		query& q = queries[i];
		q.idx = i + 1;
		cin >> q.goal;
	}

	cin >> C;
	for (int i = 0; i < C; i++) {
		comet& c = comets[i];
		cin >> c.L >> c.R >> c.qVal;
	}
	for (int i = 0; i < Q; i++) {
		query& q = queries[i];
		q.low = 0, q.high = C;
		q.mid = (q.low + q.high) / 2;
	}

	for (int i = 0; i < 19; i++) {
		PBS();
	}

	sort(queries, queries + Q, qcomp2);
	for (int i = 0; i < Q; i++) {
		if (queries[i].low == C) cout << "NIE" << '\n';
		else cout << queries[i].low + 1 << '\n';
	}
}