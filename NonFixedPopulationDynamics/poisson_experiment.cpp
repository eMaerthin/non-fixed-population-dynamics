//
//  poisson_experiment.cpp
//
//
//  Created by Marcin Bodych on 22.01.2018.
//
//

#include "poisson_experiment.h"

PoissonExperiment::PoissonExperiment(const size_t n0, const size_t T,
                                     const Strategy strategy):
n0_(n0), T_(T),
strategy_interpreter(std::make_unique<StrategyInterpreter>(n0, strategy)) {
    last_n_ = n0;
    state.emplace_back(std::make_pair(static_cast<float>(n0),1));
}

float PoissonExperiment::PoissonPmf(const size_t k, const float lambda) {
    // https://en.wikipedia.org/wiki/Poisson_distribution#Definition
    return pow(M_E, k * log(lambda) - lambda - lgamma(k + 1.0));
}

void PoissonExperiment::RunExperiment(){
    for(size_t t=1; t<=T_; t++){
        IterateExperiment(t);
        PrintState(t);
    }
}

float PoissonExperiment::PrintState(const size_t t, const bool print_details,
                                   const float correction) const {
    std::cout << t << "\t" <<
    std::accumulate(state.begin(), state.end(), 0.0,
                    [](float acc, const std::pair<float, size_t>& l){
                        return static_cast<float>(l.second)*l.first+acc;
                    });
    float ret_val = std::accumulate(state.begin(), state.end(), 0.0,
                                    [](float acc,
                                       const std::pair<float, size_t>& l){
                                        return l.first+acc;
                                    });
    std::cout << "\t" << ret_val + correction << "\t";
    
    if(print_details){
        std::vector<std::pair<float, size_t>> filtered_states;
        filtered_states.resize(20, std::make_pair(0.0,0));
        std::partial_sort_copy(state.begin(), state.end(),
                               filtered_states.begin(),
                               filtered_states.end(),
                               [](const std::pair<float, size_t>& l,
                                  const std::pair<float, size_t>& r){
                                   return l.second < r.second;
                               });
        std::for_each(filtered_states.begin(), filtered_states.end(),
                      [](const std::pair<float, size_t> &elem) {
                          std::cout << elem.first << ":" << elem.second << "\t";
                      });
    }
    return ret_val;
}
void PoissonExperiment::IterateExperiment(const size_t t){
    const size_t n=strategy_interpreter->ComputePopulationSize(t);
    std::vector<float> generated_values;
    constexpr float std_multiplier = 6.0;
    generated_values.resize(n,0.0); //here we count from 1 to n
    for(const std::pair<float, size_t> &pair : state){
        if(pair.second < std::min(last_n_,n)){
            const float lambda = pair.second * n / static_cast<float>(last_n_);
            //std::min<float>(static_cast<float>(n),
            //                pair.second * n/static_cast<float>(last_n_));
            
            std::vector<size_t> state_values;
            size_t state_values_length =
            std::ceil(2.0*std_multiplier*sqrt(lambda)+1.0);
            state_values.resize(state_values_length);
            std::iota(state_values.begin(), state_values.end(),
                      std::max(1.0,
                               lambda - std::ceil(std_multiplier*sqrt(lambda))
                               )
                      );
            for(const size_t &state_value : state_values){
                generated_values[std::min<size_t>(n,state_value)-1]
                += PoissonPmf(state_value, lambda) * pair.first;
            }
        }
        else{ //absorbing boundary
            generated_values[n-1] += pair.first;
        }
    }
    float weights_sum =
    std::accumulate(generated_values.begin(),generated_values.end(), 0.0,
                    [counter = 1](float acc, float weight) mutable
                    { return acc+weight*static_cast<float>(counter++);});
    //should be n;
    state.clear();
    state.resize(generated_values.size());
    state.shrink_to_fit();
    float correction = static_cast<float>(n)/weights_sum;
    std::transform(generated_values.begin(), generated_values.end(),
                   state.begin(),
                   [correction, counter = 1](float weight) mutable
                   { return std::make_pair(correction*weight,counter++); });
    last_n_=n;
}