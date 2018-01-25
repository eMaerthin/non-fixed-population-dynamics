//
//  statistics_ensemble_populations.h
//  
//
//  Created by Marcin Bodych on 22.01.2018.
//
//

#ifndef ____statistics_ensemble_populations__
#define ____statistics_ensemble_populations__
#include <iostream>
#include <stdio.h>
#include <vector>

#include "color_distribution.h"
#include "poisson_experiment.h"
#include "population.h"
#include "strategies.h"

struct StatisticsEnsemblePopulations {
    Strategy strategy_;
    int N0_;
    int T_;
    int iterations;
    int current_iteration;
    bool print_state;
    
    PoissonExperiment experiment_;
    std::vector<float> avg_colors_per_timestamp;
    std::vector< std::vector<float> > avg_distribution_per_timestamp;
    StatisticsEnsemblePopulations(Strategy strategy, int N0, int T,
                                  int iterations, bool print_state=false):
    strategy_(strategy), N0_(N0), T_(T), iterations(iterations),
    current_iteration(0), print_state(print_state),
    experiment_(PoissonExperiment(N0_,T_,strategy_)){
        avg_colors_per_timestamp.resize(T_+1);
        avg_distribution_per_timestamp.resize(T_+1);
        for(int t=0; t<=T_;t++){
            avg_distribution_per_timestamp[t].resize(N0_);
        }
        for(int i=0;i<N0_;i++){
            avg_distribution_per_timestamp[0][i]=1.0;
        }
        avg_colors_per_timestamp[0]=N0_;
        for(int t=1; t<=T_;t++){
            avg_colors_per_timestamp[t]=0.0;
            for(int i=0;i<N0_;i++){
                avg_distribution_per_timestamp[t][i]=0.0;
            }
        }
    }
    void RunSimulation(bool print_statistics = true,
                       bool run_poisson_experiment = false,
                       bool print_details = false){
        for(current_iteration = 1; current_iteration <= iterations;
            current_iteration++){
            std::cout << "Iteration" << current_iteration << "/" << iterations;
            std::cout << "...\n";
            RunSingleRun(print_state);
        }
        if(print_statistics){
            PrintStatistics(run_poisson_experiment, print_details);
        }
    }
    void UpdateStatistics(Population population, int t){
        ColorDistribution color_distribution =
        population.ComputeColorDistribution();
        float colors = static_cast<float>(color_distribution.GetColors());
        avg_colors_per_timestamp[t]=
        (avg_colors_per_timestamp[t]*(current_iteration-1)+colors)
        /static_cast<float>(current_iteration);
        for(int i=0; i<N0_; i++){
            avg_distribution_per_timestamp[t][i]=
            (avg_distribution_per_timestamp[t][i]*(current_iteration-1)
             +color_distribution.GetNthColor(i))
            /static_cast<float>(current_iteration);
        }
    }
    void PrintStatistics(bool run_poisson_experiment, bool print_details){
        for(int t=0; t<=T_;t++){
            if(run_poisson_experiment){
                if(t>0){
                    experiment_.IterateExperiment(t);
                }
                experiment_.PrintState(t, print_details);
            }
            else{
                std::cout << t << "\t";
            }
            std::cout << avg_colors_per_timestamp[t];
            if(print_details){
                for(int i=0; i<N0_; i++){
                    std::cout << "\t" << avg_distribution_per_timestamp[t][i];
                }
            }
            std::cout << std::endl;
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
#endif /* defined(____statistics_ensemble_populations__) */
