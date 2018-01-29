//
//  running_average.cpp
//
//
//  Created by Marcin Bodych on 29.01.2018.
//
//

#include "running_average.h"

RunningAverage::RunningAverage() : current_average_(0.0), counter_(0) {}

float RunningAverage::UpdateValue(const float new_value){
    counter_++;
    float scale = 1.0/counter_;
    current_average_ = current_average_ * (1.0 - scale) + new_value * scale;
    return current_average_;
}

float RunningAverage::ComputeAverage(const float current_average,
                                     const float update_value,
                                     const size_t counter){
    float scale = 1.0 / (counter+1.0);
    return current_average * (1.0 - scale) + update_value * scale;
}