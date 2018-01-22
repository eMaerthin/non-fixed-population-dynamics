//
//  poisson_experiment.h
//  
//
//  Created by Marcin Bodych on 22.01.2018.
//
//

#ifndef ____poisson_experiment__
#define ____poisson_experiment__

#include <algorithm>
#include <iostream>
#include <random>
#include <stdio.h>
#include <vector>

#include "strategies.h"

struct PoissonExperiment{
    PoissonExperiment(int n0, int T, Strategy strategy):
    n0_(n0), T_(T), strategy_(strategy) {
        last_n_ = n0;
        state.resize(1);
        state[0] = std::pair<double,int>(1.0,n0);
    }
    void RunExperiment(){
        for(int t=1; t<=T_; t++){
            IterateExperiment(t);
            PrintState(t);
        }
    }
private:
    void PrintState(int t){
        std::cout << t << "\t" << std::accumulate(state.begin(), state.end(), 0,
                                                [](int acc,
                                                   const std::pair<int, int>& l
                                                   )
                                                  {
                                                      return l.second*l.first+acc;
                                                  }) << "\t";
        std::reverse(state.begin(), state.end());
        for(const std::pair<int, int>& pair : state){
            std::cout << pair.first << ":" << pair.second << "\t";
        }
        std::cout << std::endl;
    }
    void IterateExperiment(int t){
        std::vector<std::pair<int, int>> new_state;
        int n=n0_;
        switch(strategy_){
            case QuadraticGrowing:
                n+=t*t;
                break;
            case LinearGrowing:
                n+=t;
                break;
            case ConstantPopulation:
                break;
            default:
                throw std::invalid_argument("not handled errror");
        }
        std::vector<int> generated_values, unique_values;
        generated_values.reserve(n);
        for(auto &pair : state){
            double lambda = pair.first * n/static_cast<double>(last_n_);
            std::poisson_distribution<int> distribution(lambda);
            for(int i=0; i<pair.second*static_cast<int>(round(n/static_cast<double>(last_n_))); i++){
                int number = distribution(generator);
                generated_values.push_back(number);
            }
        }
        std::sort(generated_values.begin(), generated_values.end());
        unique_values = generated_values;
        unique_values.resize(std::distance(unique_values.begin(),
                                           std::unique(unique_values.begin(),
                                                       unique_values.end())));
        for(auto &value : unique_values){
            std::pair<int, int> v = std::pair<int, int>(value,
                                                        std::count(
                                                        generated_values.begin(),
                                                        generated_values.end(),
                                                        value));
            new_state.push_back(v);
        }
        state.clear();
        for(auto pair : new_state){
            state.push_back(pair);
        }
        last_n_=n;
    }
    int n0_;
    int T_;
    int strategy_;
    int last_n_;
    std::vector<std::pair<double, int>> state;
    
    std::default_random_engine generator;
};

#endif /* defined(____poisson_experiment__) */
