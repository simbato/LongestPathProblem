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

void unos() {
  scanf("%d %d", &n, &m);
  REP(i, m) {
    int a, b;
    scanf("%d %d", &a, &b);
    graf[a].push_back(b);
    graf[b].push_back(a);
    if(a == 0 || b == 0) exit(0);
  }
}

void dfs(int cvor, vector<int> &put, vector<int> &bio) {
  put.push_back(cvor);
  bio[cvor] = 1;
  REP(i, (int) graf[cvor].size())
    if(!bio[graf[cvor][i]]) {
      dfs(graf[cvor][i], put, bio);
      break;
    }
}

int main() {
  unos();
  vector<int> put;
  vector<int> bio(n + 1, 0);
  dfs(1, put, bio);
  printf("%d\n", (int) put.size());
  REP(i, (int) (int) put.size())
    printf("%d ", put[i]);
  printf("\n");
  return 0;
}
