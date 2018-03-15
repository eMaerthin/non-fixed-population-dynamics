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
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using FunctionalStrategy = std::function<float(float)>;
enum Strategy { QuadraticGrowing, LinearGrowing, ConstantPopulation,
    FunctionalDependency, ExponentialGrowing };

struct StrategyInterpreter{
    StrategyInterpreter(const size_t n0, const Strategy strategy);
    StrategyInterpreter(const size_t n0, const Strategy strategy,
                        const std::vector<float> parameters);
    StrategyInterpreter(const size_t n0, const FunctionalStrategy function);
    size_t ComputePopulationSize(const size_t t) const;
    float ComputeApproxPopulationSize(const float t) const;
    const Strategy GetStrategy() const;
private:
    const size_t n0_;
    const Strategy strategy_;
    const FunctionalStrategy function_;
    std::vector<float> parameters_;
};

#endif /* defined(____strategy_interpreter__) */
