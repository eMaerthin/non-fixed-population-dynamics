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
        state[0] = std::pair<float,int>(static_cast<float>(n0),1);
    }
    
    float PoissonPmf(int k, float lambda) {
        // https://en.wikipedia.org/wiki/Poisson_distribution#Definition
        return pow(M_E, k * log(lambda) - lambda - lgamma(k + 1.0));
    }
    void RunExperiment(/*float threshold = 0.5*/){
        for(int t=1; t<=T_; t++){
            IterateExperiment(t);
            PrintState(t/*, threshold*/);
        }
    }
    void PrintState(int t, bool print_details = false, float correction = 0.0){
        
        std::cout << t << "\t" << std::accumulate(state.begin(), state.end(), 0.0,
                                                [](float acc,
                                                   const std::pair<float, int>& l
                                                   )
                                                  {
                                                      return static_cast<float>(l.second)*l.first+acc;
                                                  }) << "\t" <<
        std::accumulate(state.begin(), state.end(), 0.0,
                        [](float acc,
                           const std::pair<float, int>& l
                           )
                        { return l.first+acc;
                        })+correction << "\t";
        
        if(print_details){
            std::vector<std::pair<float, int>> filtered_states;
            filtered_states.resize(state.size(), std::make_pair(0.0,0));
            float threshold = 0.5;
            // copy only numbers above given threshold:
            auto it = std::copy_if (state.begin(), state.end(),
                                    filtered_states.begin(),
                                    [threshold](const std::pair<float, int>& pair)
                                    {return pair.first>threshold;} );
            filtered_states.resize(std::distance(filtered_states.begin(),it));  // shrink container to new size
            
            //alternatively:
            //filtered_states.resize(10, std::make_pair(0.0,0));
            //std::partial_sort_copy(state.begin(), state.end(),
            //                       filtered_states.begin(),
            //                       filtered_states.end(),
            //                       [](const std::pair<float, int>& l,
            //                          const std::pair<float, int>& r){
            //                           return l.first > r.first;
            //                      });
            //std::reverse(filtered_states.begin(), filtered_states.end());
            //std::cout << t << "\t";
            for(const std::pair<float, int>& pair : filtered_states){
                std::cout << pair.first << ":" << pair.second << "\t";
            }
            std::cout << std::endl;
            filtered_states.erase(filtered_states.begin(),filtered_states.end());
            filtered_states.clear();
        }
    }
    void IterateExperiment(int t){
        //std::vector<std::pair<int, int>> new_state;
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
        //std::vector<int> state_values;//, unique_values;
        std::vector<float> generated_values;
        float std_multiplier = 6.0;
        generated_values.resize(n+std::ceil(std_multiplier*sqrt(n))+1,0.0);
        for(auto &pair : state){
            if(pair.second<last_n_){
                float lambda = std::min<float>( static_cast<float>(n),
                                               pair.second * n/static_cast<float>(last_n_));
                std::vector<int> state_values;
                int state_values_length = std::ceil(2*std_multiplier*sqrt(lambda)+1);
                state_values.resize(state_values_length);
                std::iota(state_values.begin(), state_values.end(),
                          std::max(1.0,lambda - std::ceil(std_multiplier*sqrt(lambda))));
                for( auto &state_value : state_values){
                    float weighted_probability =
                    PoissonPmf(state_value, lambda) * pair.first;
                    generated_values[std::min<int>(n,state_value)] += weighted_probability;
                }
            }
            else{
                generated_values[n] += pair.first;
            }
        }
        //state_values.clear();
        generated_values.erase(generated_values.begin());
        float weights_sum =
        std::accumulate(generated_values.begin(),generated_values.end(), 0.0,
                        [counter = 1](float acc, float x) mutable
                        { return acc+x*static_cast<float>(counter++);});
        //should be n;
        state.clear();
        state.resize(generated_values.size());
        float correction = static_cast<float>(n)/weights_sum;
        std::transform(generated_values.begin(), generated_values.end(),
                       state.begin(),
                       [correction, counter = 1](float x) mutable
                       { return std::make_pair(correction*x,counter++); });
        //std::reverse(state.begin(),state.end());
        //state = std::move(generated_values);
        /*
         for(auto &pair : state){
            double lambda = pair.first * n/static_cast<double>(last_n_);
            std::poisson_distribution<int> distribution(lambda);
            for(int i=0; i<pair.second*static_cast<int>(round(n/static_cast<double>(last_n_))); i++){
                int number = distribution(generator);
                generated_values.push_back(number);
            }
        }
        */
        /*
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
         */
        last_n_=n;
    }
    int n0_;
    int T_;
    int strategy_;
    int last_n_;
    std::vector<std::pair<float, int>> state;
    
    //std::default_random_engine generator;
};

#endif /* defined(____poisson_experiment__) */
