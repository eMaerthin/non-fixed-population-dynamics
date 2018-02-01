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
#include "running_average.h"
#include "strategy_interpreter.h"

struct StatisticsEnsemblePopulations {
    StatisticsEnsemblePopulations(const Strategy strategy, const size_t N0,
                                  const size_t T, const size_t iterations,
                                  const bool print_state=false);
    void RunSimulation(const bool print_statistics = true,
                       const bool run_poisson_experiment = false,
                       const bool print_details = false);
    void UpdateStatistics(const std::shared_ptr<Population>& population,
                          const size_t t);
    float PrintAnalyticsResult(const size_t t);
    void PrintStatistics(const bool run_poisson_experiment,
                         const bool print_poisson_details,
                         const bool print_details);
    void RunSingleRun(const bool print_state = false);
private:
    const size_t N0_;
    const size_t T_;
    const size_t iterations;
    const bool print_state;
    
    size_t current_iteration;
    
    std::unique_ptr<StrategyInterpreter> strategy_interpreter;
    std::unique_ptr<PoissonExperiment> experiment;
    
    std::vector<float> avg_colors_per_timestamp;
    std::vector< std::vector<float> > avg_distribution_per_timestamp;
};
#endif /* defined(____statistics_ensemble_populations__) */
