//
//  population.cpp
//
//
//  Created by Marcin Bodych on 22.01.2018.
//
//

#include "population.h"

Population::Population(const size_t N0): colors_(N0), size_(N0),
nodes(N0), color_exists(N0, true){
    std::iota(nodes.begin(), nodes.end(), 0);
}
void Population::PopulateNextGeneration(const size_t Nt){
    std::vector<size_t> next_generation_nodes(Nt);
    color_exists.clear();
    color_exists.resize(colors_, false);
    color_exists.shrink_to_fit();
    
    std::for_each(next_generation_nodes.begin(), next_generation_nodes.end(),
                  [this](size_t& individual_state) mutable {
                      size_t color = nodes[rand()%size_];
                      individual_state = color;
                      color_exists[color] = true;
                  });
    size_ = Nt;
    nodes.reserve(Nt);
    nodes = next_generation_nodes;
    nodes.shrink_to_fit();
}
void Population::PrintState(const size_t t) const {
    std::cout << t << "\t" << CountColors() << "\n";
}
std::unique_ptr<ColorDistribution>
Population::ComputeColorDistribution() const {
    std::vector<size_t> colors(colors_, 0);
    std::for_each(colors.begin(), colors.end(),
                  [this, i = 0](size_t& color) mutable {
                      color = std::count(nodes.begin(), nodes.end(), i++);
                  });
    std::sort(colors.begin(), colors.end(), std::greater<size_t>());
    colors.erase(colors.begin()+CountColors(), colors.end());
    colors.shrink_to_fit();
    return std::make_unique<ColorDistribution>(colors);
}
void Population::PrintCurrentColorsDistribution() const {
    const std::vector<size_t>& color_distribution_vec =
    ComputeColorDistribution()->GetColorDistribution();
    std::cout << color_distribution_vec.size() << " colors:";
    std::for_each(color_distribution_vec.begin(), color_distribution_vec.end(),
                  [](const size_t& current_color){
                      std::cout << "\t" << current_color;
                  });
    std::cout << "\n";
}
const size_t Population::CountColors() const {
    return std::count_if(color_exists.begin(), color_exists.end(),
                         [](const bool& v){ return v; });
}