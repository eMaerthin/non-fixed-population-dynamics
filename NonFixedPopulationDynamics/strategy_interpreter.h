//
//  strategy_interpreter.h
//
//
//  Created by Marcin Bodych on 29.01.2018.
//
//

#ifndef ____strategy_interpreter__
#define ____strategy_interpreter__

#include <iostream>
#include <stdio.h>
#include <utility>

using FunctionalStrategy = std::function<size_t(size_t)>;
enum Strategy { QuadraticGrowing, LinearGrowing, ConstantPopulation,
    FunctionalDependency };

struct StrategyInterpreter{
    StrategyInterpreter(const size_t n0, const Strategy strategy);
    StrategyInterpreter(const size_t n0, const FunctionalStrategy function);
    size_t ComputePopulationSize(const size_t t) const;
    const Strategy GetStrategy() const;
private:
    const size_t n0_;
    const Strategy strategy_;
    const FunctionalStrategy function_;
};

#endif /* defined(____strategy_interpreter__) */
