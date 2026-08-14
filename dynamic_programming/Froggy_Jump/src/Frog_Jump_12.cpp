#include "../include/Frog_Jump_12.hpp"

int Frog_Jump_12::FJ_Top_down() {
    std::vector<int> memo(cost.size(), -1);
    memo[0] = 0;
    memo[1] = std::abs(cost[1] - cost[0]);
    return top_down(2, memo);
}

int Frog_Jump_12::FJ_Botton_up() {
    return botton_up();
}

int Frog_Jump_12::FJ_Botton_up_Space_Optimization() {
    return botton_up_Space_Optimization();
}

int Frog_Jump_12::top_down(int i, std::vector<int>& dp) {
    if (i < 2 || dp[i] != -1) return dp[i];
    return dp[i] = std::min(top_down(i - 1, dp) + std::abs(cost[i] - cost[i - 1]),
                            top_down(i - 2, dp) + std::abs(cost[i] - cost[i - 2]) );
}

int Frog_Jump_12::botton_up() {
    std::vector<int> dp(cost.size(), -1);
    dp[0] = 0;
    dp[1] = std::abs(cost[1] - cost[0]);

    for (int i{2}; i < cost.size(); ++i) {
        dp[i] = std::min(dp[i - 1] + std::abs(cost[i] - cost[i - 1]),
                         dp[i - 2] + std::abs(cost[i] - cost[i - 2]) );
    }
    return dp[cost.size() - 1];
}

int Frog_Jump_12::botton_up_Space_Optimization() {
    if (cost.empty() || cost.size() == 1) return 0;

    int prev2 = 0;
    int prev1 = std::abs(cost[1] - cost[0]);

    for (int i{2}; i < cost.size(); ++i) {
        int one_step = prev1 + std::abs(cost[i] - cost[i - 1]);
        int two_step = prev2 + std::abs(cost[i] - cost[i - 2]);

        prev2 = prev1;
        prev1 = std::min(one_step, two_step);
    }

    return prev1;
}
