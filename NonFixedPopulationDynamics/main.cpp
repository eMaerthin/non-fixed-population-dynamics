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
    Strategy strategy_;
    int N0_;
    int T_;
    int iterations;
    int current_iteration;
    bool print_state;
    std::vector<float> avg_colors_per_timestamp;
    StatisticsEnsemblePopulations(Strategy strategy, int N0, int T,
                                  int iterations, bool print_state=false):
    strategy_(strategy), N0_(N0), T_(T), iterations(iterations),
    current_iteration(0), print_state(print_state){
        avg_colors_per_timestamp.resize(T_+1);
        avg_colors_per_timestamp[0]=N0_;
        for(int t=1; t<=T_;t++){
            avg_colors_per_timestamp[t]=0.0;
        }
    }
    void RunSimulation(bool print_statistics = true){
        for(current_iteration = 1; current_iteration <= iterations;
            current_iteration++){
            RunSingleRun(print_state);
        }
        if(print_statistics){
            PrintStatistics();
        }
    }
    void UpdateStatistics(Population population, int t){
        float colors = static_cast<float>(population.CountColors());
        avg_colors_per_timestamp[t]=
        (avg_colors_per_timestamp[t]*(current_iteration-1)+colors)
        /static_cast<float>(current_iteration);
    }
    void PrintStatistics(){
        for(int t=0; t<=T_;t++){
            std::cout << t << "\t" << avg_colors_per_timestamp[t] << std::endl;
        }
    }
    void RunSingleRun(bool print_state = false){
        int N0 = N0_;
        int T = T_;
        Strategy strategy = strategy_;
        Population population = Population(N0);
        if(print_state){
            population.PrintState(0);
        }
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
            UpdateStatistics(population, t);
            if(print_state){
                population.PrintState(t);
            }
        }
    }
};
int main(int argc, const char * argv[]) {
    
    /* initialize random seed: */
    srand (time(NULL));
    int N0 = 1000;
    int T = 2000;
    Strategy strategy = Strategy::LinearGrowing;
    //single run case:
    //StatisticsEnsemblePopulations statistics =
    //StatisticsEnsemblePopulations(strategy, N0, T, 1, true);
    //statistics.RunSimulation(false);
    int iterations = 1000;
    StatisticsEnsemblePopulations statistics =
    StatisticsEnsemblePopulations(strategy, N0, T, iterations);
    statistics.RunSimulation();
    return 0;
}
