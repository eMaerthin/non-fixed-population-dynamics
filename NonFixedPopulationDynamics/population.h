//
//  population.h
//
//
//  Created by Marcin Bodych on 22.01.2018.
//
//

#ifndef ____population__
#define ____population__
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <vector>

#include "color_distribution.h"

struct Population {
    Population(const size_t N0);
    void PopulateNextGeneration(const size_t Nt);
    void PrintState(const size_t t) const;
    const ColorDistribution ComputeColorDistribution() const;
    void PrintCurrentColorsDistribution() const;
    const size_t CountColors() const;
private:
    const size_t colors_;
    
    std::vector<size_t> nodes;
    size_t size_;
    std::vector<bool> color_exists;
};
#endif /* defined(____population__) */
