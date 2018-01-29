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
    ColorDistribution(const std::vector<size_t>& color_distribution);
    const size_t GetNthColor(const size_t i) const noexcept;
    const std::vector<size_t>& GetColorDistribution() const;
    const size_t GetColors() const;
private:
    std::vector<size_t> color_distribution_;
};

#endif /* defined(____color_distribution__) */
