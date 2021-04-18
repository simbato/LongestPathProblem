#include <bits/stdc++.h>
#include "m_genetic.h"

using namespace std;

#define TRACE(x) cerr << #x << " = " << x << endl
#define FOR(i, a, b) for(int i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)

typedef long long int llint;

typedef pair<int, int> par;

#define X first
#define Y second

//Definirano u m_genetic.h
//const int MAXN = 1e6 + 10;

// Number of individuals in each generation 
// Probavali smo različite veličine i najbolja rješenja ispadaju s velicinom 30
#define POPULATION_SIZE 30

int n_, m_;
vector<int> mGraf[MAXN];

void unos() {
  scanf("%d %d", &n_, &m_);
  REP(i, m_) {
    int a, b;
    scanf("%d %d", &a, &b);
    mGraf[a].push_back(b);
    mGraf[b].push_back(a);
    if(a == 0 || b == 0) exit(0);
  }
}

int main(){
    srand(101);
    unos();

    //Prenosimo graf u drugu klasu. Moguca mozda bolja implementacija ili rjesenje
    //ovog problema. 
    setGraph(n_, m_, mGraf);

    //Brojac koji sluzi pracenju broja generacija u algoritmu.
    int generationCount = 0;

    //Stvaramo novu populaciju.
    Population population(POPULATION_SIZE);

    //Svakoj jedinci iz populacije potrebno izracunati vrijednost/fitness.
    population.calculateFitness();

    //Varijabla u kojoj pratimo fitness najboljeg puta generacije.
    int lastTheBest = population.getTheBestFitness();

    //Ispis indofmracija o generaciji.
    cerr << "\tGeneration " << generationCount << ": path length = " 
                            << lastTheBest << endl;


    while(generationCount < 50){
        generationCount++;
        
        if (lastTheBest == n_){
            cerr << "\tGeneration " << generationCount << ": path length = " 
                                << population.getTheBestFitness() << endl;
            break;
        }
		//Cijela logika je implementirana u klasama Population i Individual.
        //Tamo se vrsi selekcija i odreduje koliko puta treba krizati jedinke.        
        population.performElitism(0.1);
        
		population.doCrossovers();
        
		population.doMutations();
        
		//Generirali smo novu generaciju jedinki. Odraduje potrebne promjene
        //unutarnjeg stanja klase Population.
        population.finishNewGeneration();
        
		
        population.calculateFitness(); 


        //Ispis indofmracija o generaciji.
        if(generationCount % 5 == 0)
            cerr << "\tGeneration " << generationCount << ": path length = " 
                                << population.getTheBestFitness() << endl;
		
        
		lastTheBest = population.getTheBestFitness();
    }

    vector<int> resultPath = population.getTheBestPath();

    //"printf" is ambiguous 
    printf("%d\n", (int) resultPath.size());
    
    REP(i, (int) (int) resultPath.size())
        printf("%d ", resultPath[i]);
        
    printf("\n");
    
    return 0;
}