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

// Number of individuals in each generation 
#define POPULATION_SIZE 20

int n, m;
vector<int> graf[MAXN];

double koeficijentiOdabiraVrha[MAXN];

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

//Jedna od opcija generiranja pocetnih rjesenja.
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
Ideja: Za trenutni cvor gledamo sve njegove neposjecene susjede. Nastavljamo
izgradnju puta tako da biramo iduci vrh u putu, tj. susjeda. Taj odabir ide 
proporcionalno stupnjevima susjeda. Npr ako cvor ima dva susjeda, jednog s 3 a jednog
sa stupnjom 1, odabir prvog je 75% slucajeva a drugog u 25%. 

Ideja za poboljsanje:
Mogli bismo racunat stupnjeve susjeda tako da brojimo bridove iz susjeda, ali samo
one prema neposjecenim susjedima. No, taj bi pristup mozda bio spor. (Posto se ne generira
puno puta pocetno rjesenje, mozemo i to imeplemtirati).
*/
void pretrazi(int cvor, vector<int> &put, vector<int> &bio) {

    put.push_back(cvor);
    bio[cvor] = 1;

    int sumaStupnjevaNeposjecenih = 0;
    for(int susjed : graf[cvor]){
        if(!bio[susjed]){
            sumaStupnjevaNeposjecenih += graf[susjed].size();
        }
    }
    double gornjaGranica = 0;
    for(int i = 0; i < graf[cvor].size(); ++i){
        int susjed = graf[cvor][i];
        if(!bio[susjed]){
            //Da ne bi doslo do grasaka kod zaokruzivanja, kod zadnjeg trebamo postaviti gornju granicu na 1.
            //Npr da ne bi bila 0.9991 a generator da 0.9995
            if(i + 1 != graf[cvor].size())
                gornjaGranica += (double)(graf[susjed].size()) / sumaStupnjevaNeposjecenih;
            else 
                gornjaGranica = 1;

            koeficijentiOdabiraVrha[susjed] = gornjaGranica;
        }
    }
  
    //Sluzi za slucajni odabir jednog od susjeda vrha cvor.
    double y = (rand() % 10000) / 10000;
    //Pretrazujemo susjede onim redom kojim smo zadavali gornjuGranicu. Npr, u opisu s pocetka funkcije,
    //za prvi vrh imali bi interval [0, 0.75] a za drugi <0.75, 1]. 
    for(int i = 0; i < graf[cvor].size(); ++i){
        int susjed = graf[cvor][i];
        if(!bio[susjed] && koeficijentiOdabiraVrha[susjed] >= y){
            //Odabiremo jednog susjeda (na opisani slucajni nacin, proprcionalno stupnjevima).
            //Posto ne zelimo pretrazivati ostale susjede, tj siriti potragu dalje, izlazimo s break.
            pretrazi(susjed, put, bio);
            break;
        }
    }
    
}


// Function to generate random numbers in given range  
int random_num(int start, int end) 
{ 
    int range = (end-start)+1; 
    int random_int = start+(rand()%range); 
    return random_int; 
}

// Class representing individual in population 
class Individual 
{ 
public: 
    vector<int> put;
    //Radi lakseg pronalaska koji su vrhovi u putu. Brzi pristup nego pretraga po vektoru.
    unordered_set<int> vrhoviSet;
    //Fitness je mjera dobrote puta, u ovom slucaju velicina.    
    int fitness;        
    Individual(int i); 
    Individual(vector<int> path);
    vector<Individual> krizanje(Individual parent2); 
    void mutacija();
}; 
  
Individual::Individual(int i) 
{ 
    vector<int> bio(n + 1, 0);

    //Pretrazi metoda
    vector<int> nastavakPuta;
    pretrazi(i, put, bio);
    pretrazi(i, nastavakPuta, bio);
    //Imamo put: A <-> B <-> C, nastavakPuta A <-> B' <-> C' (neusmjeren).
    //Zelimo put: C <-> B <-> A <-> B' <-> C'.
    reverse(put.begin(), put.end());
    put.pop_back();
    put.insert(put.end(), nastavakPuta.begin(), nastavakPuta.end());
    
    //DFS pocetna metoda
    //Mozemo nju ukljuciti posto ne daje odmah idealna rjesnja, pa moze
    //posluziti da vidimo napreduje li nas algoritam.
    //dfs(i, put, bio);

    //Sigurno postoji pametniji nacin za odradit ovo. U struktiru Individual mozemo
    //direktno cuvati i vector<int> bio.
    for(int i = 1; i <= n; ++i)
        if(bio[i] == 1)
            vrhoviSet.insert(i);
    
    fitness = put.size(); 
}; 

Individual::Individual(vector<int> path) : put (path) {

    for(int vrh : path)
        vrhoviSet.insert(vrh);

    fitness = path.size();
} 
  
// Perform mating and produce new offspring 
vector<Individual> Individual::krizanje(Individual par2) 
{ 
    vector<Individual> result;
    result.push_back(put);
    return result;
}; 

void Individual::mutacija(){

}
  
  
// Overloading < operator 
bool operator<(const Individual &ind1, const Individual &ind2) 
{ 
	return ind1.fitness > ind2.fitness; 
} 


//Za sad se ne brinemo oko toga ako imamo vise istih jedinki u populaciji.
//Kasnije dorade.
vector<Individual> populacija;


int main() {
    srand(101);
    unos();
    /*
    //Ovaj dio je sluzio za tesiranje jedne jedinke metode1, tj pocetne populacije.
    vector<int> put;
    vector<int> nastavakPuta;
    vector<int> bio(n + 1, 0);

    int vrhNajvecegStupnja = -1;
    int maxStupanj = 0;
    for(int i = 1; i <= n; ++i){
        if(graf[i].size() > maxStupanj){
            maxStupanj = graf[i].size();
            vrhNajvecegStupnja = i;
        }
    }

    pretrazi(vrhNajvecegStupnja, put, bio);
    pretrazi(vrhNajvecegStupnja, nastavakPuta, bio);

    //Imamo put: A <-> B <-> C, nastavakPuta A <-> B' <-> C' (neusmjeren).
    //Zelimo put: C <-> B <-> A <-> B' <-> C'.
    reverse(put.begin(), put.end());
    put.pop_back();
    put.insert(put.end(), nastavakPuta.begin(), nastavakPuta.end());
    */
    for(int i = 0; i < POPULATION_SIZE; ++i){
        int pocetniVrh = random_num(1, n);
        populacija.push_back( Individual(pocetniVrh) );
    }

    sort(populacija.begin(), populacija.end());

    /*
    Tu dolazi kod za genetski algoritam. 
    Trenutno, uzeli smo samo pocetnu populaciju i vratili najbolje rjesnje.
    Pocetnu populaciju biramo na random. Mozemo mozda dodati da uvijek ubaci
    pocetnu jedinku koja krece iz cvora s najvecim stupnjem.
    */
    int last = populacija[0].fitness;
	//U varijabli last cuvamo vrijednost najduzeg puta u zadnjoj populaciji.
	//Brojac nepoboljsanja mjeri koliko se zadnjih iteracija nismo poboljsali.
	//Sluzi za odredivanje uvijeta zaustavljanja.
	int nepoboljsanja = 0;
	while(nepoboljsanja < 5){

		vector<Individual> novaGeneracija;

	}


    vector<int> put = populacija[0].put;
	
    printf("%d\n", (int) put.size());
    REP(i, (int) (int) put.size())
        printf("%d ", put[i]);
    printf("\n");
    return 0;
}
