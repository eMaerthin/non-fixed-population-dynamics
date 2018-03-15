//
//  main.cpp
//  NonFixedPopulationDynamics
//
//  Created by Marcin Bodych on 14.01.2018.
//  Copyright (c) 2018 Marcin Bodych. All rights reserved.
//
#include "inhomogeneous_random_walk.h"
#include "statistics_ensemble_populations.h"

int main(int argc, const char * argv[]) {
    
    /* initialize random seed: */
    srand (static_cast<unsigned int>(time(NULL)));
    int N0 = 10;
    int T = 100;
    Strategy strategy = Strategy::LinearGrowing;//QuadraticGrowing;
    //ExponentialGrowing;//LinearGrowing;
    int iterations = 0;
    std::vector<float> parameters(1,1.0);
    FunctionalStrategy functional_strategy = [N0](float t){return N0+N0/2*cosf(t);};
    std::shared_ptr<StrategyInterpreter>
    strategy_interpreter(std::make_shared<StrategyInterpreter>
     //                    (N0, functional_strategy));
                         (N0, strategy, parameters));
    //StatisticsEnsemblePopulations statistics = StatisticsEnsemblePopulations(strategy_interpreter, T, iterations);
    bool print_statistics = true;
    bool run_poisson_experiment = false;
    bool print_details = false;
    //statistics.RunSimulation(print_statistics, run_poisson_experiment,
    //                         print_details);
    InhomogeneousRandomWalk walk(T, strategy_interpreter);
    walk.RunExperiment(true);
    //std::cin >> T;
    return 0;
}
