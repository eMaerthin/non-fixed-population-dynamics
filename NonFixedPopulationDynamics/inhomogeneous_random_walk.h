//
//  inhomogeneous_random_walk.h
//  
//
//  Created by Marcin Bodych on 08/02/18.
//
//

#ifndef ____inhomogeneous_random_walk__
#define ____inhomogeneous_random_walk__

#include <chrono>
#include <list>
#include <random>
#include <stdio.h>
#include "strategy_interpreter.h"

struct InhomogeneousRandomWalk{
    InhomogeneousRandomWalk(const size_t T,
    std::shared_ptr<StrategyInterpreter> strategy_interpreter);
    float DrawWaitingTime() const;
    size_t DrawPoissonOffsprings() const;
    void UpdateEvents();
    void RunExperiment(bool print_events = false);
    void PrintEvents() const;
    size_t current_n_;
    const size_t Tmax_;
    float t_;
    std::shared_ptr<StrategyInterpreter> strategy_interpreter_;
    std::list<std::pair<float, size_t>> events_list_;
};
#endif /* defined(____inhomogeneous_random_walk__) */
