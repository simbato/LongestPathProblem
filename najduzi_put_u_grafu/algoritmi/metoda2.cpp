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

//Cuvamo u globalnom polju "tezine/vrijednosti" vrhova.
double score[MAXN];

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

/*
Primitivna implementacija: score vrha je stupanj vrha.
*/
void postaviScore(){
    
    for(int i = 1; i <= n; ++i)
        score[i] = graf[i].size();
}

void postaviPraviPoredakSusjeda(){

    for(int i = 1; i <= n; ++i){
        sort(graf[i].begin(), graf[i].end(), [](int susjed1, int susjed2) -> bool{
            return score[susjed1] > score[susjed2];
        });
    }
}

// https://www.geeksforgeeks.org/binary-search/
// A recursive binary search function. It returns 
// location of x in given array arr[l..r] is present, 
// otherwise -1 
int binarySearch(vector<int> const & arr, int l, int r, int x) 
{ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
  
        // If the element is present at the middle 
        // itself 
        if (arr[mid] == x) 
            return mid; 
  
        // If element is smaller than mid, then 
        // it can only be present in left subarray 
        if (arr[mid] > x) 
            return binarySearch(arr, l, mid - 1, x); 
  
        // Else the element can only be present 
        // in right subarray 
        return binarySearch(arr, mid + 1, r, x); 
    } 
  
    // We reach here when element is not 
    // present in array 
    return -1; 
} 

// Ako je v = 0: vratimo prvi po prioritetu susjed kranjeng cvora u putu.
// Inace: vratimo prvog susjeda od zadnjeg cvora u putu, koji ima manji 
// prioritet od v.
int nextUnexploredVertex(vector<int> &put, vector<int> &bio,  int v = 0){

    if(put.size() < 1)
        return -1;
    int zadnjiCvor = put[put.size() - 1];

    int indexSusjedaV;
    //Ovo smo mogli i preskociti posto score[0] = 0 (vrhove brojimo od 1).
    if(v == 0)
        indexSusjedaV = -1;
    else
        indexSusjedaV = binarySearch(graf[zadnjiCvor], 0, graf[zadnjiCvor].size() - 1, v);

    for(int i = indexSusjedaV + 1; i < graf[zadnjiCvor].size(); ++i){
        /*
        auto it = find(put.begin(), put.end(), graf[zadnjiCvor][i]);
        if(it == put.end())
            return i;
        */
       if(!bio[i])
        return i;
    }

    return -1;
}

/*
    true = done
    false = not done
*/
bool nextPathInDFSManner(vector<int> &put, vector<int> &bio){

    if(put.size() < 1)
        return -1;
    int zadnjiCvor = put[put.size() - 1];
    int noviCvor = nextUnexploredVertex(put, bio, 0);

    while(noviCvor == -1 && put.size() > 1){
        zadnjiCvor = put[put.size() - 1];
        put.pop_back();
        bio[zadnjiCvor] = 0;

        int noviZadnjiCvor = put[put.size() - 1];
        noviCvor = nextUnexploredVertex(put, bio, zadnjiCvor);
    }

    if(noviCvor == -1)
        return true;

    put.push_back(noviCvor);
    bio[noviCvor] = 1;
    return false;
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

/*
Nedovrseno, stoga pozivamo pocetnu metodu dfs. Parmutacija susjeda je promijenjena
s obzirom na score, pa imamo drugacije krajnje rjesneje.
*/
int main() {
    unos();
    postaviScore();
    postaviPraviPoredakSusjeda();

    vector<int> put;
    vector<int> bio(n + 1, 0);
    dfs(1, put, bio);
    printf("%d\n", (int) put.size());
    REP(i, (int) (int) put.size())
        printf("%d ", put[i]);
    printf("\n");
    return 0;
}
