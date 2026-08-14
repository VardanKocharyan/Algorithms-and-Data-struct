#pragma once
#include <vector>

class Frog_Jump_k {
public:
    Frog_Jump_k() = default;
    Frog_Jump_k(const std::vector<int>& v, int m_k = 2) : cost(v), k(m_k) {}
    Frog_Jump_k(std::vector<int>&& v, int m_k = 2) : cost(std::move(v)), k(m_k) {}
    Frog_Jump_k(std::initializer_list<int> ilist, int m_k = 2) : cost(ilist), k(m_k) {}

    int FJ_Top_down(); 
    int FJ_Botton_up();

private:
    std::vector<int> cost;
    int k;

    int top_down(int i, std::vector<int>& dp);
    int botton_up();
};
