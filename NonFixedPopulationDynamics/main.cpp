//
//  main.cpp
//  NonFixedPopulationDynamics
//
//  Created by Marcin Bodych on 14.01.2018.
//  Copyright (c) 2018 Marcin Bodych. All rights reserved.
//
#include <algorithm>
#include <iostream>
#include <vector>

bool IsTrue(bool value){return value;}

struct Population {
    std::vector<int> nodes;
    int size_;
    int colors_;
    std::vector<bool> colorExists;
    Population(int N0): size_(N0), colors_(N0) {
        nodes.resize(N0);
        colorExists.resize(N0);
        for(int i=0; i<N0; i++){
            nodes[i]=i;
            colorExists[i]=true;
        }
    }
    void PopulateNextGeneration(int Nt){
        Population new_population = Population(Nt);
        for(int i=0; i < colors_; i++)
        {
            colorExists[i] = false;
        }
        //draw:
        for(int i=0;i<Nt;i++){
            new_population.nodes[i] = nodes[rand()%size_];
            colorExists[new_population.nodes[i]] = true;
        }
        size_ = Nt;
        nodes.resize(Nt);
        nodes = new_population.nodes;
    }
    void PrintState(int t){
        std::cout << t << "\t" << CountColors() << "\n";
    }
    long CountColors(){
        long colors = count_if(colorExists.begin(), colorExists.end(), IsTrue);
        return colors;
    }
};
enum Strategy { QuadraticGrowing, LinearGrowing, ConstantPopulation };

struct StatisticsEnsemblePopulations {
    
};
int main(int argc, const char * argv[]) {
    
    /* initialize random seed: */
    srand (time(NULL));
    int N0 = 1000;
    int T = 2000;
    Strategy strategy = Strategy::LinearGrowing;
    Population population = Population(N0);
    population.PrintState(0);
    for(int t=1;t<=T;t++){
        switch(strategy){
            case QuadraticGrowing:
                population.PopulateNextGeneration(N0+t*t);
                break;
            case LinearGrowing:
                population.PopulateNextGeneration(N0+t);
                break;
            case ConstantPopulation:
                population.PopulateNextGeneration(N0);
                break;
            default:
                throw std::invalid_argument("not handled errror");
        }
        population.PrintState(t);
    }
    return 0;
}
