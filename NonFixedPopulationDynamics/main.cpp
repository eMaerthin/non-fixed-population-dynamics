//
//  main.cpp
//  NonFixedPopulationDynamics
//
//  Created by Marcin Bodych on 14.01.2018.
//  Copyright (c) 2018 Marcin Bodych. All rights reserved.
//
#include "poisson_experiment.h"
#include "strategies.h"
#include "statistics_ensemble_populations.h"

int main(int argc, const char * argv[]) {
    
    /* initialize random seed: */
    srand (time(NULL));
    int N0 = 1000;
    int T = 2000;
    Strategy strategy = Strategy::LinearGrowing;//  LinearGrowing;
    //single run case:
    //StatisticsEnsemblePopulations statistics =
    //StatisticsEnsemblePopulations(strategy, N0, T, 1, true);
    //statistics.RunSimulation(false);
    int iterations = 2;
    //StatisticsEnsemblePopulations statistics =
    //StatisticsEnsemblePopulations(strategy, N0, T, iterations);
    //statistics.RunSimulation();
    PoissonExperiment experiment(N0, T, strategy);
    experiment.RunExperiment();
    return 0;
}
