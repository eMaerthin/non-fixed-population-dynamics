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
n0_(n0), strategy_(strategy) {}

StrategyInterpreter::StrategyInterpreter(const size_t n0,
                                         const FunctionalStrategy function):
n0_(n0), strategy_(FunctionalDependency), function_(function) {}

size_t StrategyInterpreter::ComputePopulationSize(const size_t t) const {
    size_t n = n0_;
    switch(strategy_){
        case QuadraticGrowing:
            n+=t*t;
            break;
        case LinearGrowing:
            n+=t;
            break;
        case ConstantPopulation:
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
    return n;
}
