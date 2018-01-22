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
#include <stdio.h>
#include <vector>

#include "color_distribution.h"

struct Population {
    std::vector<int> nodes;
    int size_;
    int colors_;
    std::vector<bool> colorExists;
    Population(int N0): size_(N0), colors_(N0) {
        nodes.resize(N0);
        colorExists.resize(N0);
        for(int i=0; i<N0; i++){
            nodes[i]=i;
            colorExists[i]=true;
        }
    }
    void PopulateNextGeneration(int Nt){
        Population new_population = Population(Nt);
        for(int i=0; i < colors_; i++)
        {
            colorExists[i] = false;
        }
        //draw:
        for(int i=0;i<Nt;i++){
            new_population.nodes[i] = nodes[rand()%size_];
            colorExists[new_population.nodes[i]] = true;
        }
        size_ = Nt;
        nodes.resize(Nt);
        nodes = new_population.nodes;
    }
    void PrintState(int t){
        std::cout << t << "\t" << CountColors() << "\n";
    }
    ColorDistribution ComputeColorDistribution(){
        std::vector<int> colors;
        colors.resize(colors_);
        for(int i=0; i<colors_;i++){
            colors[i] = 0;
        }
        for(int i=0; i<colors_; i++){
            colors[i] = static_cast<int>(
                                         std::count(nodes.begin(), nodes.end(), i));
        }
        std::sort(colors.begin(), colors.end());
        std::reverse(colors.begin(), colors.end());
        int non_zero_colors = static_cast<int>(CountColors());
        ColorDistribution color_distribution =
        ColorDistribution(non_zero_colors, colors);
        return color_distribution;
    }
    void PrintCurrentColorsDistribution(){
        ColorDistribution color_distribution = ComputeColorDistribution();
        int colors = color_distribution.GetColors();
        std::cout << colors << " colors: \t";
        for(int i = 0 ; i < colors; i++){
            std::cout << color_distribution.GetNthColor(i);
            if(i != colors-1){
                std::cout << "\t";
            }
        }
        std::cout << "\n";
    }
    long CountColors(){
        long colors = count_if(
                               colorExists.begin(),
                               colorExists.end(),
                               [](const bool& v){ return v; }
                               );
        return colors;
    }
};
#endif /* defined(____population__) */
