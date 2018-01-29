//
//  running_average.h
//
//
//  Created by Marcin Bodych on 29.01.2018.
//
//

#ifndef ____running_average__
#define ____running_average__

#include <stdio.h>
struct RunningAverage{
    RunningAverage();
    float UpdateValue(const float new_value);
    static float ComputeAverage(const float current_average,
                                const float update_value,
                                const size_t counter);
private:
    
    float current_average_;
    size_t counter_;
};
#endif /* defined(____running_average__) */
