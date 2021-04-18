#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "m_genetic.h"

using namespace std;

#define TRACE(x) cerr << #x << " = " << x << endl
#define FOR(i, a, b) for(int i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)

int n, m;
vector<int> graf[MAXN];

double koeficijentiOdabiraVrha[MAXN];

void setGraph(int n_, int m_, vector<int> graf_ []){
    n = n_;
    m = m_;
    for(int i = 0; i < MAXN; ++i)
        graf[i].insert( graf[i].begin(), graf_[i].begin(), graf_[i].end() );
}

/*
Ideja: Za trenutni cvor gledamo sve njegove neposjecene susjede. Nastavljamo
izgradnju puta tako da biramo iduci vrh u putu, tj. susjeda. Taj odabir ide 
proporcionalno stupnjevima susjeda. Npr ako cvor ima dva susjeda, jednog s 3 a jednog
sa stupnjom 1, odabir prvog je 75% slucajeva a drugog u 25%. 

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

void floodFill(int cvor, vector<int> &udaljenost, int d) {
  if(udaljenost[cvor]) return;
  udaljenost[cvor] = d;
  for(auto x : graf[cvor])
    if(udaljenost[x] == 0)
        floodFill(x, udaljenost, d + 1);
}


vector<int> put_do_najdaljeg(int x, vector<int>& popisZabranjenihVrhova) {
  vector<int> udaljenost(n + 1);
  //Vrhove indeksiramo od 1..n. Postavljanjem udaljenosti na -1 eliminiramo vrh
  //iz pretrage.
  udaljenost[0] = -1;
  for(int vrh : popisZabranjenihVrhova)
    udaljenost[vrh] = -1;

  floodFill(x, udaljenost, 1);
  int kraj = 1;
  FOR(i, 2, n + 1)
    if(udaljenost[i] > udaljenost[kraj])
      kraj = i;
  vector<int> ret;
  int sad = kraj;
  ret.push_back(sad);
  
  if(kraj == x) return ret;

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


// Funkcija za generiranje slucajnog broja izmedu start i end 
int random_num(int start, int end) 
{ 
    int range = (end-start)+1; 
    int random_int = start+(rand()%range); 
    return random_int; 
}


//##############################################
//Implementacija funkcija iz klase Individual.

void Individual::calculateIFitness(){

    fitness = put.size();
}

Individual::Individual(int i, int metoda) 
{
    if(metoda == 1){
        //Pretrazi metoda 
        vector<int> bio(n + 1, 0);

        vector<int> nastavakPuta;
        pretrazi(i, put, bio);
        pretrazi(i, nastavakPuta, bio);
        //Imamo put: A <-> B <-> C, nastavakPuta A <-> B' <-> C' (neusmjeren).
        //Zelimo put: C <-> B <-> A <-> B' <-> C'.
        reverse(put.begin(), put.end());
        put.pop_back();
        put.insert(put.end(), nastavakPuta.begin(), nastavakPuta.end());        
    }
    else if(metoda == 2){
        vector<int> popisZabranjenihVrhova;
        //Ideja: kreni od vrha i, pronadi put do najudaljenijeg mu vrha A.
        vector<int> tmpA = put_do_najdaljeg(i, popisZabranjenihVrhova);
        //Ponovi to za i = A. Tj. za vrh A pronadi najudaljeniji put do nekog vrha B.
        put = put_do_najdaljeg(tmpA.back(), popisZabranjenihVrhova);
    }

    //Sigurno postoji pametniji nacin za odradit ovo. U struktiru Individual mozemo
    //direktno cuvati i vector<int> bio.
    
    for(int k = 0; k < put.size(); ++k)
        vrhoviMap[ put[k] ] = k;

    /*for(int vrhNaPutu : put)
        vrhoviSet.insert(vrhNaPutu);*/
    
    fitness = put.size(); 
};

// Overload konstruktora
Individual::Individual(vector<int>& path) : put (path) {

    for (int i = 0; i < path.size(); i++)
    {
        if (vrhoviMap.find(path[i]) != vrhoviMap.end()){
            put.clear();
            vrhoviMap.clear();
            fitness = 0;
            return;
        }
        int ii = i;
        vrhoviMap[put[i]] = i;
    }
    

    fitness = path.size();
} 

void ubaci(vector<int> &res, vector<int> &put, int i, int j){
    if (i < j){
        for (; i < j; i++)
            res.push_back(put[i]);
    }
    else { //(j < i)
        for (; i > j; i--)
            res.push_back(put[i]);
    }
}

// Krizanje
vector<Individual> Individual::crossover(Individual& parent2) 
{ 
    //Implementacija.

    vector<Individual> result;
    vector<int> res;

    int j = put.size() - 1;
    int i = 0, prvi;
    std::unordered_map<int,int>::iterator it, jt;

    /*
    Pronalazimo  prvu i zadnju zajednicku tocku na prvom putu (this.put)
    Na taj nacin dobivamo 2^3 - 2 = 6 djece:
    spajamo segmente putova prije, izmedu i poslje zajednicih tocki
    Također cemo kao djecu poslati i klonove roditelja
    Na ovaj nacin se mogu dobiti neispravni putovi pa svaki put to moramo provjeriti 
    i ako je neispravan izbaciti tu jedinku iz result.
    */
    
    for (; i < put.size(); i++){
        it = parent2.vrhoviMap.find(put[i]);
        if ( it != parent2.vrhoviMap.end() )
            break;
    } 
    if (it == parent2.vrhoviMap.end()){
        // Znaci da nemaju zajednickih tocaka
        result.push_back(put);
        return result;
    }  

    for (; j >= 0; j--){
        jt = parent2.vrhoviMap.find(put[j]);
        if ( jt != parent2.vrhoviMap.end() )
            break;
    } 
    //
    int ii = it->second, jj = jt->second;

    if (i == j){
        // Znaci da se putevi sijeku u samo jednoj točki
        // Dobivamo 4 nova puta
        // Ovaj slucaj se gotovo nikad nece dogoditi ali isto ga stavljamo za svaki slucaj
        ubaci(res, put, 0, i);
        ubaci(res, parent2.put, ii, 0);
        res.push_back(parent2.put[0]);
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();

        ubaci(res, put, 0, i);
        ubaci(res, parent2.put, ii, parent2.put.size());

        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();

        ubaci(res, parent2.put, 0, ii);
        ubaci(res, put, i, put.size());
      
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();

        ubaci(res, parent2.put, parent2.put.size() - 1, ii);
        ubaci(res, put, i, put.size());
        
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
    }
    // Koristimo ovaj uvijet za svaki slucaj, premda to mora vrijediti iz prethodnog
    else if (put[i] == parent2.put[ii] && put[j] == parent2.put[jj] ){
        // Inace imamo tocke i, ii, takve da put1[i] == put2[ii]
        // j, jj, takve da put1[j] == put2[jj]
        ubaci(res, put, 0, i);
        ubaci(res, parent2.put, ii, jj);
        ubaci(res, put, j, put.size());
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();

        ubaci(res, put, 0, i);
        ubaci(res, parent2.put, ii, (ii > jj)? -1 : parent2.put.size());
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();

        /*
        * Ova spajanja rijetko rezultiraju putom, a uvelike usporavaju izvodenje
        * pa ih brisemo

        ubaci(res, put, 0, j);
        ubaci(res, parent2.put, jj, (ii > jj)? -1 : parent2.put.size());
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();

        ubaci(res, parent2.put, (ii > jj)? parent2.put.size() - 1 : 0, ii);
        ubaci(res, put, i, put.size());
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();

        ubaci(res, parent2.put, (ii > jj)? parent2.put.size() - 1 : 0, ii);
        ubaci(res, put, i, j);
        ubaci(res, parent2.put, jj, (ii > jj)? -1 : parent2.put.size());
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();
        */

        ubaci(res, parent2.put, (ii > jj)? parent2.put.size() - 1 : 0, jj);
        ubaci(res, put, j, put.size());
        result.push_back(res);
        if (result[result.size() - 1].fitness == 0)
            result.pop_back();
        res.clear();

    }
    
    // Takoder vracamo roditelje kao djecu
    result.push_back(put);
    
    return result;
}; 

void Individual::mutation(){
    /*
    * Ideja: Uzmimo na slucajan nacin jedan cvor iz trenutne jedinke, tj s puta.
    * Veci dio puta (do tog cvora, ili od tog cvora) ostaje i u mutiranoj jedinci.
    * S druge strane, pokusamo se prosiriti sto je vise moguce u drugom smjeru.
    * Za to koristimo algoritam kojim smo generirali pocetnu populaciju. Moramo paziti
    * da u tom prosirenju puta ne posjetimo vec posjecen vrh, tj. neki vrh koji se 
    * nalazi u dijelu puta kojeg zadrzavamo. 
    */
    
    int indexPrelomnogVrha = random_num(0, put.size() - 1);
    
    if(indexPrelomnogVrha >= put.size() - 1 - indexPrelomnogVrha){
        //Zadrzavamo "lijevi" dio puta. Do odabranog vrha.
        
        vector<int> popisZabranjenihVrhova;
        for(int i = 0; i < indexPrelomnogVrha; ++i)
            popisZabranjenihVrhova.push_back( put[i] );
        
        vector<int> produzetakPuta = put_do_najdaljeg(put[indexPrelomnogVrha], popisZabranjenihVrhova);
        
        put.erase(put.begin() + indexPrelomnogVrha, put.end());
        put.insert(put.end(), produzetakPuta.begin(), produzetakPuta.end());
    }
    else{
        //Zadrzavamo "desni" dio puta. Od odabranog vrha.

        vector<int> popisZabranjenihVrhova;
        for(int i = indexPrelomnogVrha + 1; i < put.size(); ++i)
            popisZabranjenihVrhova.push_back( put[i] );
        
        vector<int> produzetakPuta = put_do_najdaljeg(put[indexPrelomnogVrha], popisZabranjenihVrhova);
        reverse(produzetakPuta.begin(), produzetakPuta.end());
        produzetakPuta.insert(produzetakPuta.end(), put.begin() + indexPrelomnogVrha + 1, put.end());
        
        put = produzetakPuta;
    }
    

    vrhoviMap.clear();
    for(int i = 0; i < put.size(); i++)
        vrhoviMap[put[i]] = i;
    

    fitness = put.size(); 
}

// Overloading < operator. 
bool operator<(const Individual &ind1, const Individual &ind2) 
{ 
	return ind1.fitness > ind2.fitness; 
} 


//##############################################
//Implementacija funkcija iz klase Population.

Population::Population(int pop_size) : POPULATION_SIZE(pop_size){

    //Stvaramo novih pop_size jedinki s jednom od metoda za generiranje pocetne populacije.
    for(int i = 0; i < pop_size; ++i){
        int pocetniVrh = random_num(1, n);
        individuals.push_back( Individual(pocetniVrh, 1) );
    }

    //Stvaramo novih pop_size jedinki s drugom metodom za generiranje pocetne populacije.
    for(int i = 0; i < pop_size; ++i){
        int pocetniVrh = random_num(1, n);
        individuals.push_back( Individual(pocetniVrh, 2) );
    }

    //Zelimo imati sortiran vektor jedinki.
    sort(individuals.begin(), individuals.end());

    //Generirali smo duplo jedinki od velicine populacije.
    //Biramo najboljih POPULATION_SIZE jedinki koje cine pocetnu populaciju.
    //Ovaj dio je ispravan posto je individuals.size() = 2 * POPULATION_SIZE
    individuals.erase(individuals.begin() + POPULATION_SIZE, individuals.end());

}

//Vraca najbolji fitness, tj duljinu najboljeg puta u generaciji.
int Population::getTheBestFitness(){

    return individuals[0].fitness;
}

//Vraca najbolji put u generaciji.
std::vector<int> Population::getTheBestPath(){

    return individuals[0].put;
}

//Sluzi za obnavljanje, racunanje fitness vrijednsti jedinki.
void Population::calculateFitness(){

    for(int i = 0; i < POPULATION_SIZE; ++i)
        individuals[i].calculateIFitness();
}

//Obavlja sva krizanja i jedinke sprema u newIndividuals.
void Population::doCrossovers(){
    
    //Vrsimo krizanja nad 70% najboljih jedinki
    int k =  POPULATION_SIZE * 0.7;
    int top = 0.8 * POPULATION_SIZE; 

    /*
    //Fitness Proportionate Selection
    vector<int> cdf_fitness;
    cdf_fitness.push_back(individuals[0].fitness);
    for(int i = 1; i < individuals.size(); ++i)
        cdf_fitness.push_back(cdf_fitness.back() + individuals[i].fitness);
    */
    
    for (int i = 0; i < top; i++){
        
        /*
        //Daje slabije rezultate Fitness Proportionate Selection
        vector<int> parents;
        while(parents.size() != 2){
            int rnd_n = random_num(0, cdf_fitness.back());
            //Ovo bi se moglo binary search-om, no kako je velicina populacije
            //relativno mala, proalzi i ovakva implementacija.
            for(int ii = 0; ii < cdf_fitness.size(); ++ii){
                if(rnd_n <= cdf_fitness[ii]){
                    //Zelimo razlicite roditelje.
                    if( !(parents.size() == 1 && parents[0] == ii) )
                        parents.push_back(ii);
                
                    break;
                }
            }
            
        }
        int a = parents[0];
        int b = parents[1];
        */

        int a = random_num(0, k);
        int b = random_num(0, k);
        // Zelimo da b bude razlicit od a
        while (b == a)
            b = random_num(0, k);
        
        //Ubacimo njihovu dijecu u newIndividuals
        vector<Individual> vec;

        vec = individuals[a].crossover(individuals[b]);
        newIndividuals.insert(newIndividuals.end(), vec.begin(), vec.end());

        vec.clear();
        vec = individuals[b].crossover(individuals[a]);
        newIndividuals.insert(newIndividuals.end(), vec.begin(), vec.end());
    }
}

//Obavlja mutacije u novoj populaciji.
void Population::doMutations(){

    //Vrsimo mutaciju nad 20 posto novostvorenih jedinki
    int k = 0.3 * newIndividuals.size();
    int len = newIndividuals.size() / k;

    //unordered_set<int> izasli;
    for (int i = 0; i < k; i++){
        int br = random_num(0, newIndividuals.size() - 1);
        newIndividuals[br].mutation();
    }
}

//Prvih eliterate posto populacije diretno ide u iducu generaciju.
void Population::performElitism(double eliteRate = 0.1){

    int eliteNumber = eliteRate * POPULATION_SIZE;
    for(int i = 0; i < eliteNumber; ++i)
        newIndividuals.push_back( individuals[i] );
}

//Prebacuje novu generaciju u vektor individuals. Sortira ga.
void Population::finishNewGeneration(){

    sort(newIndividuals.begin(), newIndividuals.end());

    //Moguce da je nastalo previse jedinki u newIndividuals.
    //Zanemarujemo najslabiji visak.
    if(newIndividuals.size() > POPULATION_SIZE)
        newIndividuals.erase(newIndividuals.begin() + POPULATION_SIZE, newIndividuals.end());

    individuals = newIndividuals;
}