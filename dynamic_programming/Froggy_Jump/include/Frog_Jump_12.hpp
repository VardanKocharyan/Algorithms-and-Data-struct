#pragma once
#include <vector>
#include <cstdint>

class Frog_Jump_12 {
public:
    Frog_Jump_12() = default;
    Frog_Jump_12(const std::vector<int>& v) : cost(v) {}
    Frog_Jump_12(std::vector<int>&& v) : cost(std::move(v)) {}
    Frog_Jump_12(std::initializer_list<int> ilist) : cost(ilist) {}

    int FJ_Top_down(); 
    int FJ_Botton_up();
    int FJ_Botton_up_Space_Optimization();

private:
    std::vector<int> cost;

    int top_down();
    int botton_up();
    int botton_up_Space_Optimization()
};
