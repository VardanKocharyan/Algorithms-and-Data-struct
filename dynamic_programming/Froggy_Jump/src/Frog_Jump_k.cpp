#include "../include/Frog_Jump_k.hpp"

int Frog_Jump_k::FJ_Top_down() {
    std::vector<int> memo(cost.size(), -1);
    memo[0] = 0;
    memo[1] = std::abs(cost[1] - cost[0]);
    return top_down(n, memo);
}

int Frog_Jump_k::FJ_Botton_up() {
    return botton_up(i);
}

int Frog_Jump_k::top_down(int i, std::vector<int>& dp) {
    if (!i || dp[i] != -1) return dp[i];

    int ans = std::numeric_limits<int>::max();
    for (int step{1}; step <= std::min(k, i); ++step) {
        ans = std::min(ans, top_down(i - step) + std::abs(cost[i] - cost[step]));
    }

    return dp = ans;
}

int Frog_Jump_k::botton_up() {
    int n = cost.size();
    std::vector<int> dp(n, -1);
    dp[0] = 0;

    for (int i{}; i < n - 1; ++i) {
        for (int step{1}; step <= std::min(k, n - i - 1); ++step) {
            dp[i] = std::min(dp[step + i], std::abs(cost[step + i] + cost[i]) + dp[i]);
        }
    }
    return dp[n - 1];
}

