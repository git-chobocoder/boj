#include <iostream>
#include <vector>
#include <algorithm>
#define pii pair <double,double>
using namespace std;

int CCW(pii A, pii B, pii C)
{
	double ax = A.first, ay = A.second, bx = B.first, by = B.second, cx = C.first, cy = C.second;
	double ccw = ax * (by - cy) + bx * (cy - ay) + cx * (ay - by);
	if (ccw < 0) return -1;
	if (ccw == 0) return 0;
	else return 1;
}
bool cross(pii A, pii B, pii C, pii D)
{
	if (A > B) swap(A, B);
	if (C > D) swap(C, D);

	double first_ccw = CCW(A, B, C) * CCW(A, B, D), second_ccw = CCW(C, D, A) * CCW(C, D, B);
	if (first_ccw == 0 && second_ccw == 0) return (A <= D) && (C <= B);
	return (first_ccw <= 0) && (second_ccw <= 0);
}

pii cross_dot(pii A, pii B, pii C, pii D)
{
	double x1 = A.first, x2 = B.first, x3 = C.first, x4 = D.first, y1 = A.second, y2 = B.second, y3 = C.second, y4 = D.second;
	pii ans = { ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - x4 * y3)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)),((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - x4 * y3)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)) };
	return ans;
}

bool IN(vector <pii> v, pii dot)
{
	pii dot2 = { dot.first + 1,dot.second + 2e9 };
	int cnt = 0;

	int N = v.size(), i;
	for (i = 0; i < N - 1; i++)
	{
		if (cross(dot, dot2, v[i], v[i + 1])) //교차한다면
			cnt++;
	}
	if (cnt % 2 == 1) //안에 있다면
		return true;
	return false;
}

int dist(pii A, pii B)
{
	return (A.first - B.first) * (A.first - B.first) + (A.second - B.second) * (A.second - B.second);
}
bool ycmp(pii A, pii B)
{
	if (A.second != B.second) return A.second < B.second;
	return A.first < B.first;
}
pii low_dot;
bool cmp(pii A, pii B)
{
	if (CCW(low_dot, A, B) > 0) return true;
	if (CCW(low_dot, A, B) == 0) return dist(low_dot, A) < dist(low_dot, B);
	return false;
}


vector <pii> make_convex(vector <pii> v)
{
	int N = v.size();
	if (N <= 2) return v;
	sort(v.begin(), v.end(), ycmp);
	low_dot = v[0];
	sort(v.begin() + 1, v.end(), cmp);

	int stk[111] = { 0 }, top = 2;
	stk[0] = 0;
	stk[1] = 1;
	for (int i = 2; i < N; i++)
	{
		while (top >= 2 && CCW(v[i], v[stk[top - 2]], v[stk[top - 1]]) <= 0)
		{
			top--;
		}
		stk[top++] = i;
	}

	vector <pii> ans;
	for (int i = 0; i < top; i++)
		ans.push_back(v[stk[i]]);
	ans.push_back(v[stk[0]]);

	return ans;
}

double area(vector <pii> v)
{
	int N = v.size();
	double ans = 0;

	for (int i = 0; i < N - 1; i++)
		ans += (v[i].first + v[i + 1].first) * (v[i].second - v[i + 1].second);
	return abs(ans) / 2;
}

int main(void)
{
	ios_base::sync_with_stdio(false); cin.tie(NULL);
	int N, M, i, a, b;
	vector <pii> v1, v2;

	cin >> N >> M;
	for (i = 0; i < N; i++)
	{
		cin >> a >> b;
		v1.push_back({ a,b });
	}
	for (i = 0; i < M; i++)
	{
		cin >> a >> b;
		v2.push_back({ a,b });
	}
	v1 = make_convex(v1);
	v2 = make_convex(v2);

	vector <pii> v3;
	for (pii x : v1)
	{
		if (IN(v2, x))
			v3.push_back(x);
	}
	for (pii x : v2)
	{
		if (IN(v1, x))
			v3.push_back(x);
	}


	for (i = 0; i < v1.size() - 1; i++)
	{
		for (int j = 0; j < v2.size() - 1; j++)
		{
			if (cross(v1[i], v1[i + 1], v2[j], v2[j + 1]))
			{
				v3.push_back({ cross_dot(v1[i],v1[i + 1],v2[j],v2[j + 1]) });
			}
		}
	}
	v3 = make_convex(v3);

	cout << fixed;
	cout.precision(10);
	cout << area(v3);
}