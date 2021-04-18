#include <bits/stdc++.h>
using namespace std;

#define TRACE(x) cerr << #x << " = " << x << endl
#define FOR(i, a, b) for(int i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)

typedef long long int llint;

typedef pair<int, int> par;

#define X first
#define Y second

const int MAXN = 1e6 + 10;

int n, m;

vector<int> graf[MAXN];

void dfs(int cvor, vector<int> &udaljenost, int d) {
  if(udaljenost[cvor]) return;
  udaljenost[cvor] = d;
  for(auto x : graf[cvor])
    dfs(x, udaljenost, d + 1);
}

vector<int> put_do_najdaljeg(int x) {
  vector<int> udaljenost(n);
  dfs(x, udaljenost, 1);
  int kraj = 1;
  REP(i, n)
    if(udaljenost[i] > udaljenost[kraj])
      kraj = i;
  vector<int> ret;
  int sad = kraj;
  ret.push_back(sad);
  do {
    for(auto t : graf[sad])
      if(udaljenost[t] == udaljenost[sad] - 1) {
        sad = t;
        break;
      }
    ret.push_back(sad);
  } while(sad != x);
  reverse(ret.begin(), ret.end());
  return ret;
}

int main() {
  scanf("%d %d", &n, &m);
  REP(i, m) {
    int a, b;
    scanf("%d %d", &a, &b);
    a--, b--;
    graf[a].push_back(b);
    graf[b].push_back(a);
  }
  vector<int> v = put_do_najdaljeg(1);
  vector<int> put = put_do_najdaljeg(v.back());
  cout << put.size() << endl;
  REP(i, (int) put.size())
    cout << put[i] + 1 << " ";
  cout << endl;
  return 0;
}

