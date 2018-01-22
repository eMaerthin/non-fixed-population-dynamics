//
//  color_distribution.h
//  
//
//  Created by Marcin Bodych on 22.01.2018.
//
//

#ifndef ____color_distribution__
#define ____color_distribution__

#include <stdio.h>
#include <vector>

struct ColorDistribution {
    int colors_;
    std::vector<int> color_distribution_;
    ColorDistribution(int colors, std::vector<int> color_distribution):
    colors_(colors), color_distribution_(color_distribution){
    }
    int GetColors(){
        return colors_;
    }
    int GetNthColor(int i){
        if(i>color_distribution_.size()){
            return 0;
        }
        return color_distribution_[i];
    }
};

#endif /* defined(____color_distribution__) */
