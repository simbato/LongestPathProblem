#include <bits/stdc++.h>
using namespace std;

#define TRACE(x) cerr << #x << " = " << x << endl
#define FOR(i, a, b) for(int i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)

typedef long long int llint;

typedef pair<int, int> par;

#define X first
#define Y second

int rnd(int a, int b) {
  return a + (rand() % (b - a + 1));
}

int n, m;

void ispisi_vektor(const vector<int> &v) {
  cerr << "debug.. vektor" << endl;
  for(auto x : v)
    cerr << x << " ";
  cerr << endl;
}

void ispisi_graf(const vector<vector<int> > &g) {
  cerr << "debug.. graf" << endl;
  for(int i = 0; i < n; i++) {
    cerr << i << ": ";
    for(int j = 0; j < (int) g[i].size(); j++)
      cerr << g[i][j] << " ";
    cerr << endl;
  }
  cerr << endl;
}

vector<int> rand_perm(int l) {
  vector<int> ret;
  for(int i = 0; i < l; i++)
    ret.push_back(i);
  random_shuffle(ret.begin(), ret.end());
  return ret;
}

void dfs_najdalji(const vector<vector<int> > &graf, int cvor, 
         vector<int> &udaljenost, vector<int> &posjeceni, int d) {
  udaljenost[cvor] = d;
  posjeceni.push_back(cvor);
  vector<int> perm = rand_perm(graf[cvor].size());
  for(auto i : perm) { 
    int x = graf[cvor][i];
    if(!udaljenost[x])
      dfs_najdalji(graf, x, udaljenost, posjeceni, d + 1);
  }
}

vector<int> put_do_najdaljeg(const vector<vector<int> > &graf, int pocetak, vector<int> udaljenost = vector<int> (n)) {
  vector<int> posjeceni;
  dfs_najdalji(graf, pocetak, udaljenost, posjeceni, 1);
  int kraj = pocetak;
	vector<int> perm = rand_perm(posjeceni.size());
  for(auto i : perm) {
    int x = posjeceni[i];
		if(udaljenost[x] > udaljenost[kraj])
      kraj = x;
	}
  vector<int> ret;
  int sad = kraj;
  ret.push_back(sad);
  do {
		perm = rand_perm(graf[sad].size());
    for(auto i : perm) {
			int t = graf[sad][i];
      if(udaljenost[t] == udaljenost[sad] - 1) {
        sad = t;
        break;
      }
		}
    ret.push_back(sad);
  } while(sad != pocetak);
  reverse(ret.begin(), ret.end());
  return ret;
}

vector<int> najdulji_put_stablo(const vector<vector<int> > &stablo) {
  vector<int> t = put_do_najdaljeg(stablo, 0);
  vector<int> put = put_do_najdaljeg(stablo, t.back());
  return put;
}

void dfs_stablo(const vector<vector<int> > &graf, int cvor, 
                vector<int> &bio, vector<vector<int> > &stablo, vector<int> &poredak) {
  poredak.push_back(cvor);
  assert(!bio[cvor]);
  bio[cvor] = 1;
  vector<int> perm = rand_perm(graf[cvor].size());
  auto cmp = [&graf](int x, int y) { return graf[x].size() > graf[y].size(); };
  sort(perm.begin(), perm.end(), cmp);
  for(auto i : perm) {
    int x = graf[cvor][i];
    if(!bio[x]) {
      stablo[cvor].push_back(x);
      stablo[x].push_back(cvor);
      dfs_stablo(graf, x, bio, stablo, poredak);
    }
  }
}

void kombinacija_dfs_i_bfs_generiranja_stabla(const vector<vector<int> > &graf, int cvor,
                                             vector<int> &bio, vector<vector<int> > &stablo, vector<int> &poredak) {
  deque<par> dq;
  dq.push_back(par(cvor, -1));
  while(!dq.empty()) {
    par p = dq.front(); 
    int x = p.X;
    int prosli = p.Y;
    dq.pop_front();
    if(bio[x]) continue;
    bio[x] = 1;
    poredak.push_back(x);
    if(prosli != -1) {
      stablo[prosli].push_back(x);
      stablo[x].push_back(prosli);
    }
    vector<int> perm = rand_perm(graf[x].size());
    reverse(perm.begin(), perm.end());
    for(auto i : perm) {
      int t = graf[x][i];
      if(bio[t]) continue;
      dq.push_front(par(t, x));
    }
  }
}

vector<vector<int> > generiraj_stablo(const vector<vector<int> > &graf) {
  int x = rnd(0, n - 1);
  vector<vector<int> > stablo(n, vector<int> ());
  vector<int> bio(n);
  vector<int> poredak; 
  dfs_stablo(graf, x, bio, stablo, poredak); 
  //kombinacija_dfs_i_bfs_generiranja_stabla(graf, x, bio, stablo, poredak);
  //vector<vector<int> > _stablo(n, vector<int> ());
  //vector<int> _bio(n);
  //vector<int> _poredak;
  //kombinacija_dfs_i_bfs_generiranja_stabla(graf, x, _bio, _stablo, _poredak);
  return stablo;
}


vector<int> generiraj_put(const vector<vector<int> > &graf) {
  vector<vector<int> > stablo = generiraj_stablo(graf);
  vector<int> put = najdulji_put_stablo(stablo);
  return put;
}

vector<int> najjaci_u_populaciji(const vector<vector<int> > &populacija) {
  int t = 0;
  REP(i, (int) populacija.size())
    if(populacija[i].size() > populacija[t].size())
      t = i;
  return populacija[t];
}

vector<int> mutiraj(const vector<vector<int> > &graf, vector<int> put) {
  if(rnd(1, 2) == 1) 
    reverse(put.begin(), put.end());
  int x = rnd(0, ((int) put.size())/5);
  vector<int> udaljenost(n);
  for(int i = 0; i <= x; i++)
    udaljenost[put[i]] = -1;
  vector<int> produzetak = put_do_najdaljeg(graf, put[x], udaljenost);
  vector<int> ret;
  for(int i = 0; i <= x; i++)
    ret.push_back(put[i]);
  for(int i = 1; i < (int) produzetak.size(); i++)
    ret.push_back(produzetak[i]);
  return put;
}

vector<vector<int> > nova_generacija(const vector<vector<int> > &graf, const vector<vector<int> > &populacija) {
  vector<vector<int> > ret;
  for(int i = 0; i < (int) populacija.size(); i++)
    ret.push_back(mutiraj(graf, populacija[i]));
  return ret;
}

vector<int> genetski(const vector<vector<int> > &graf) {
  const int velicina_populacije = 30;
  const int broj_generacija = 5;
  vector<vector<int> > populacija;
  REP(i, velicina_populacije)
    populacija.push_back(generiraj_put(graf));
  vector<int> ret = najjaci_u_populaciji(populacija);
  REP(i, broj_generacija) {
    populacija = nova_generacija(graf, populacija);
    vector<int> t = najjaci_u_populaciji(populacija);
    //REP(i, 5) cerr << populacija[i].size() << " ";
    //cerr << endl;
    if(t.size() > ret.size())
      ret = t;
  }
  cerr << endl;
  return ret;
}

int main() {
  srand(3137);
  scanf("%d %d", &n, &m); 
  vector<vector<int> > graf(n, vector<int>());
  REP(i, m) {
    int a, b;
    scanf("%d %d", &a, &b);
    a--, b--;
    graf[a].push_back(b);
    graf[b].push_back(a);
  }
  vector<int> put = genetski(graf);
  cout << put.size() << endl;
  REP(i, (int) put.size())
    cout << put[i] + 1 << " ";
  cout << endl;
  return 0;
}

