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

#include "strategy_interpreter.h"

struct PoissonExperiment{
    PoissonExperiment(const size_t n0, const size_t T, const Strategy strategy);
    PoissonExperiment(const size_t n0, const size_t T,
                      std::shared_ptr<StrategyInterpreter> strategy_interpreter);
    static float PoissonPmf(const size_t k, const float lambda);
    void RunExperiment();
    float PrintState(const size_t t, const bool print_details = false,
                    const float correction = 0.0) const;
    void IterateExperiment(const size_t t);
private:
    const size_t n0_;
    const size_t T_;
    std::shared_ptr<StrategyInterpreter> strategy_interpreter_;
    
    size_t last_n_;
    std::vector<std::pair<float, size_t>> state;
    
};

#endif /* defined(____poisson_experiment__) */
