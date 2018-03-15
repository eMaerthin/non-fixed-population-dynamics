//
//  inhomogeneous_random_walk.cpp
//  
//
//  Created by Marcin Bodych on 08/02/18.
//
//

#include "inhomogeneous_random_walk.h"


InhomogeneousRandomWalk::InhomogeneousRandomWalk(const size_t T,
std::shared_ptr<StrategyInterpreter> strategy_interpreter):
Tmax_(T), t_(0.0), current_n_(1), strategy_interpreter_(strategy_interpreter)
{ }

float InhomogeneousRandomWalk::DrawWaitingTime() const {
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed1);
    
    std::exponential_distribution<float> waiting_time(current_n_);
    return waiting_time(generator);
}

size_t InhomogeneousRandomWalk::DrawPoissonOffsprings() const {
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed1);
    
    std::poisson_distribution<size_t> poisson_offsprings(strategy_interpreter_->ComputeApproxPopulationSize(t_)/strategy_interpreter_->ComputeApproxPopulationSize(t_-1));
    return poisson_offsprings(generator);
}

void InhomogeneousRandomWalk::UpdateEvents() {
    events_list_.emplace_back(t_, current_n_);
}
void InhomogeneousRandomWalk::PrintEvents() const {
    std::cout << "t\tn\n";
    for(auto& x : events_list_){
        std::cout << x.first << "\t" << x.second << "\n";
    }
}
void InhomogeneousRandomWalk::RunExperiment(bool print_events) {
    while(t_ < static_cast<float>(Tmax_) && current_n_ > 0){
        float waiting_time = DrawWaitingTime();
        t_+=waiting_time;
        size_t poisson_offsprings = DrawPoissonOffsprings();
        current_n_ += poisson_offsprings - 1;
        UpdateEvents();
    }
    if(print_events){
        PrintEvents();
    }
}