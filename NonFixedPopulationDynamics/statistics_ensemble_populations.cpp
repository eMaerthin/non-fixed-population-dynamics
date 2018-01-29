//
//  statistics_ensemble_populations.cpp
//
//
//  Created by Marcin Bodych on 22.01.2018.
//
//

#include "statistics_ensemble_populations.h"
#include <chrono>
#include <iomanip>
StatisticsEnsemblePopulations::StatisticsEnsemblePopulations(
                                const Strategy strategy, const size_t N0,
                                const size_t T, const size_t iterations,
                                const bool print_state):
N0_(N0), strategy_interpreter_(N0, strategy), T_(T), iterations(iterations),
current_iteration(0), print_state(print_state),
experiment_(PoissonExperiment(N0_,T_,strategy)),
avg_colors_per_timestamp(T+1, 0.0),
avg_distribution_per_timestamp(T+1){
    std::for_each(avg_distribution_per_timestamp.begin(),
                  avg_distribution_per_timestamp.end(),
                  [N0](std::vector<float> &v){ v.resize(N0, 0.0); });
    for(size_t i=0;i<N0_;i++){
        avg_distribution_per_timestamp[0][i]=1.0;
    }
    avg_colors_per_timestamp[0]=static_cast<float>(N0_);
}
void StatisticsEnsemblePopulations::RunSimulation(
                                     const bool print_statistics,
                                     const bool run_poisson_experiment,
                                     const bool print_details){
    
    for(current_iteration = 1; current_iteration <= iterations;
        current_iteration++){
        auto t0 = std::chrono::high_resolution_clock::now();
        std::cout << "Iteration" << current_iteration;
        std::cout << "/" << iterations << "..." << std::flush;
        RunSingleRun(print_state);
        auto t1 = std::chrono::high_resolution_clock::now();
        auto dt = 1.e-9*std::chrono::duration_cast<std::chrono::nanoseconds>
        (t1-t0).count();
        std::cout << " took " << dt << "s." << std::endl;
    }
    if(print_statistics){
        PrintStatistics(run_poisson_experiment, print_details, false);
    }
}
void StatisticsEnsemblePopulations::UpdateStatistics(
                                                    const Population population,
                                                    const size_t t){
    const ColorDistribution color_distribution =
    population.ComputeColorDistribution();
    float colors = static_cast<float>(color_distribution.GetColors());
    avg_colors_per_timestamp[t]=
    RunningAverage::ComputeAverage(avg_colors_per_timestamp[t],
                                   colors, current_iteration);
    for(size_t i=0; i<N0_; i++){
        avg_distribution_per_timestamp[t][i]=
        RunningAverage::ComputeAverage(avg_distribution_per_timestamp[t][i],
                                       color_distribution.GetNthColor(i),
                                       current_iteration);
    }
}
void StatisticsEnsemblePopulations::PrintStatistics(
const bool run_poisson_experiment, const bool print_poisson_details,
const bool print_details) {
    for(size_t t=0; t<=T_;t++){
        if(run_poisson_experiment){
            if(t>0){
                experiment_.IterateExperiment(t);
            }
            experiment_.PrintState(t, print_poisson_details);
        }
        else{
            std::cout << t << "\t";
        }
        std::cout << avg_colors_per_timestamp[t];
        if(print_details){
            for(size_t i=0; i<N0_; i++){
                std::cout << "\t" << avg_distribution_per_timestamp[t][i];
            }
        }
        std::cout << std::endl;
    }
}
void StatisticsEnsemblePopulations::RunSingleRun(const bool print_state){
    Population population = Population(N0_);
    if(print_state){
        population.PrintState(0);
    }
    for(size_t t=1;t<=T_;t++){
        population.PopulateNextGeneration(strategy_interpreter_
                                          .ComputePopulationSize(t));
        UpdateStatistics(population, t);
        if(print_state){
            population.PrintState(t);
        }
    }
}