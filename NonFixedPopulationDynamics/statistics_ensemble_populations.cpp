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
std::shared_ptr<StrategyInterpreter> strategy_interpreter, const size_t N0,
const size_t T, const size_t iterations, const bool print_state):
N0_(N0), T_(T), iterations(iterations), print_state(print_state),
current_iteration(0), strategy_interpreter_(std::move(strategy_interpreter)),
experiment(std::make_unique<PoissonExperiment>(N0_,T_,strategy_interpreter)),
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
const std::shared_ptr<Population>& population, const size_t t){
    std::unique_ptr<ColorDistribution> color_distribution =
    population->ComputeColorDistribution();
    float colors = static_cast<float>(color_distribution->GetColors());
    avg_colors_per_timestamp[t]=
    RunningAverage::ComputeAverage(avg_colors_per_timestamp[t],
                                   colors, current_iteration);
    for(size_t i=0; i<N0_; i++){
        avg_distribution_per_timestamp[t][i]=
        RunningAverage::ComputeAverage(avg_distribution_per_timestamp[t][i],
                                       color_distribution->GetNthColor(i),
                                       current_iteration);
    }
}
float StatisticsEnsemblePopulations::ComputePopulationRatio(const size_t t){
    return static_cast<float>(strategy_interpreter_->ComputePopulationSize(t))/
    static_cast<float>(strategy_interpreter_->ComputePopulationSize(t-1));
}
float StatisticsEnsemblePopulations::PrintAnalyticsResult(const size_t t){
    double partial_res = 1.0;
    double partial_exp = 0.0;
    for(size_t tick = t; tick >= 1; tick--){
        partial_exp = exp(-ComputePopulationRatio(tick)*partial_res);
        partial_res = 1.0-partial_exp;
    }
    double ret_val = std::max(1.0,(1.0 - partial_exp ) * N0_);
    std::cout << ret_val << "\t";
    return static_cast<float>(ret_val);
}
void StatisticsEnsemblePopulations::PrintStatistics(
const bool run_poisson_experiment, const bool print_poisson_details,
const bool print_details) {
    std::cout << "t\tNt\t";
    if(run_poisson_experiment){
        std::cout << "Analytic-Simul\t";
    }
    std::cout << "Analytic\t";
    std::cout << "Simulation\t";
    std::cout <<"Diff\t";
    std::cout << std::endl;
    float analytic_drop_fraction = 0.0;
    for(size_t t=0; t<=T_;t++){
        float poisson_ret_val = 0.0;
        if(t>0){
            analytic_drop_fraction = expf(-1)*expf(analytic_drop_fraction);
        }
        if(run_poisson_experiment){
            if(t>0){
                experiment->IterateExperiment(t);
            }
            poisson_ret_val = experiment->PrintState(t, print_poisson_details);
        }
        else{
            std::cout << t << "\t";
            std::cout << strategy_interpreter_->ComputePopulationSize(t) << "\t";
        }
        ///TODO
        // Below line should works fine ONLY for Strategy::ConstantPopulation
        //std::cout << (1.0-analytic_drop_fraction)*N0_ << "\t";
        float ret_analytic_result = PrintAnalyticsResult(t);
        std::cout << avg_colors_per_timestamp[t];
        if(run_poisson_experiment){
            std::cout << "\t" << avg_colors_per_timestamp[t] - poisson_ret_val;
        }
        else {
            std::cout << "\t" << avg_colors_per_timestamp[t] - ret_analytic_result;
        }
        if(print_details){
            for(size_t i=0; i<N0_; i++){
                std::cout << "\t" << avg_distribution_per_timestamp[t][i];
            }
        }
        std::cout << std::endl;
    }
}
void StatisticsEnsemblePopulations::RunSingleRun(const bool print_state){
    std::shared_ptr<Population> population(std::make_shared<Population>(N0_));
    if(print_state){
        population->PrintState(0);
    }
    for(size_t t=1;t<=T_;t++){
        population->PopulateNextGeneration(strategy_interpreter_->
                                          ComputePopulationSize(t));
        UpdateStatistics(population, t);
        if(print_state){
            population->PrintState(t);
        }
    }
}