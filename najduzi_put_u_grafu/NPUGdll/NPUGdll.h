#pragma once

#define DllExport __declspec( dllexport )

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <fstream>

extern "C" DllExport void runMain(const char* fileName);
extern "C" DllExport void getMessage(char* str, int len);
extern "C" DllExport int getGenerationNumber();
extern "C" DllExport int getMaxGenerations();
extern "C" DllExport void reset();


const int MAXN = 1000010;

class Individual {
public:

    std::vector<int> put;

    //Radi lakseg pronalaska koji su vrhovi u putu. Brzi pristup nego pretraga po vektoru.
    std::unordered_set<int> vrhoviSet;
    std::unordered_map<int, int> vrhoviMap;

    //Fitness je mjera dobrote puta, u ovom slucaju duzina puta. 
    int fitness = 0;

    //Mozempo i izbaciti jer je jednostavna funkcija. Pazimo samo da je pozivamo
    //pri svakoj promjeni jedinke.
    void calculateIFitness();

    //Konstruktori.
    Individual(int i, int metoda);
    Individual(std::vector<int>& path);

    //Krizanje.
    std::vector<Individual> crossover(Individual& parent2);

    //Mutacija.
    void mutation();
};

// Overloading < operator. Sluzi nam za sortiranje svih jedinki. 
bool operator<(const Individual& ind1, const Individual& ind2);

class Population {
public:

    int POPULATION_SIZE;

    //Cuva trenutne jedinke iz populacije.
    std::vector<Individual> individuals;

    //Pomocni vektor koji sluzi za stvaranje nove generacije,
    //tj. skupa jedinki.
    std::vector<Individual> newIndividuals;

    //Konstruktor.
    Population(int pop_size);

    //Vraca najbolji fitness, tj duljinu najboljeg puta u generaciji.
    int getTheBestFitness();

    //Vraca najbolji put u generaciji.
    std::vector<int> getTheBestPath();

    //Sluzi za obnavljanje, racunanje fitness vrijednsti jedinki.
    void calculateFitness();

    //Obavlja sva krizanja i jedinke sprema u newIndividuals.
    void doCrossovers();

    //Obavlja mutacije u novoj populaciji.
    void doMutations();

    //Prvih eliterate posto populacije diretno ide u iducu generaciju.
    void performElitism(double eliteRate);

    //Prebacuje novu generaciju u vektor individuals. Sortira ga.
    void finishNewGeneration();
};