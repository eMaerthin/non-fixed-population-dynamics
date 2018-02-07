//
//  main.cpp
//  NonFixedPopulationDynamics
//
//  Created by Marcin Bodych on 14.01.2018.
//  Copyright (c) 2018 Marcin Bodych. All rights reserved.
//

#include "statistics_ensemble_populations.h"

int main(int argc, const char * argv[]) {
    
    /* initialize random seed: */
    srand (static_cast<unsigned int>(time(NULL)));
    int N0 = 200;
    int T = 3000;
    Strategy strategy = Strategy::ExponentialGrowing;//LinearGrowing;
    int iterations = 100;
    std::vector<float> parameters(1,1.0);
    std::shared_ptr<StrategyInterpreter>
    strategy_interpreter(std::make_shared<StrategyInterpreter>
                         (N0, strategy, parameters));
    StatisticsEnsemblePopulations statistics =
    StatisticsEnsemblePopulations(strategy_interpreter, N0, T, iterations);
    bool print_statistics = true;
    bool run_poisson_experiment = false;
    bool print_details = false;
    statistics.RunSimulation(print_statistics, run_poisson_experiment,
                             print_details);
    return 0;
}
