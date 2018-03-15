//
//  strategy_interpreter.cpp
//
//
//  Created by Marcin Bodych on 29.01.2018.
//
//

#include "strategy_interpreter.h"

StrategyInterpreter::StrategyInterpreter(const size_t n0,
                                         const Strategy strategy):
n0_(n0), strategy_(strategy), parameters_(1, 1.0) {}

StrategyInterpreter::StrategyInterpreter(const size_t n0,
                                         const Strategy strategy,
                                         const std::vector<float> parameters):
n0_(n0), strategy_(strategy), parameters_(parameters) {}

StrategyInterpreter::StrategyInterpreter(const size_t n0,
                                         const FunctionalStrategy function):
n0_(n0), strategy_(FunctionalDependency), function_(function) {}

float StrategyInterpreter::ComputeApproxPopulationSize(const float t) const {
    float n = static_cast<float>(n0_);
    switch(strategy_){
        case QuadraticGrowing:
            n+=parameters_[0]*t*t;
            break;
        case LinearGrowing:
            n+=parameters_[0]*t;
            break;
        case ConstantPopulation:
            break;
        case ExponentialGrowing:
            n=powf(parameters_[0],t)*n;
            break;
        case FunctionalDependency:
            try{
                n = function_(t);
            }catch(const std::bad_function_call& ex){
                std::cout << ex.what() << std::endl;
            }
            break;
        default:
            throw std::invalid_argument("not handled errror");
    }
    if (n < 1) n = 1;
    return n;
}
size_t StrategyInterpreter::ComputePopulationSize(const size_t t) const {
    return
    static_cast<size_t>(ComputeApproxPopulationSize(static_cast<float>(t)));
}

const Strategy StrategyInterpreter::GetStrategy() const {
    return strategy_;
}
