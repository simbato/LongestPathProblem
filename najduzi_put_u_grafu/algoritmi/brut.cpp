#include <bits/stdc++.h>
using namespace std;

#define TRACE(x) cerr << #x << " = " << x << endl
#define FOR(i, a, b) for(int i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)

typedef long long int llint;

typedef pair<int, int> par;

#define X first
#define Y second

const int MAXN = 21;
const int INF = 1e9;

vector<int> graf[MAXN];

int dp[MAXN][1 << MAXN];
par prije[MAXN][1 << MAXN];

int n, m;

int main() {
  int n, m;
  scanf("%d %d", &n, &m);
  REP(i, m) {
    int a, b;
    scanf("%d %d", &a, &b);
    a--, b--;
    graf[a].push_back(b);
    graf[b].push_back(a);
  }
  par naj(0, 0);
  REP(mask, 1 << n)
    REP(i, n) {
      if(!((1 << i) & mask)) continue;
      if(!((1 << i) ^ mask)) {
        dp[i][mask] = 1;
        prije[i][mask] = par(-1, -1);
        continue;
      }
      for(auto j : graf[i])
        if((1 << j) & mask)
          if(dp[i][mask] < dp[j][mask ^ (1 << i)] + 1) {
            dp[i][mask] = dp[j][mask ^ (1 << i)] + 1;
            prije[i][mask] = par(j, mask ^ (1 << i));
          }
      if(dp[i][mask] > dp[naj.X][naj.Y])
        naj = par(i, mask);
    }
  vector<int> put;
  while(naj.X != -1) {
    put.push_back(naj.X);
    naj = prije[naj.X][naj.Y];
  }

  cout << put.size() << endl;
  for(auto x : put)
    cout << x + 1 << " ";
  cout << endl;
  return 0;
}

