//
//  color_distribution.cpp
//  
//
//  Created by Marcin Bodych on 22.01.2018.
//
//

#include "color_distribution.h"
#include <iostream>

ColorDistribution::ColorDistribution(const std::vector<size_t>&
                                     color_distribution):
color_distribution_(color_distribution){ }

const size_t ColorDistribution::GetNthColor(const size_t i) const noexcept {
    if(i>=color_distribution_.size()){
        return 0;
    }
    return color_distribution_[i];
}

const std::vector<size_t>& ColorDistribution::GetColorDistribution() const {
    return color_distribution_;
}

const size_t ColorDistribution::GetColors() const {
    return color_distribution_.size();
}