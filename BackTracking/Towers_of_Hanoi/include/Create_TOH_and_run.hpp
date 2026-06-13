#pragma once
#include "Towers_of_Hanoi.hpp"

void create_and_run(int n) {
    Towers_of_Hanoi game1(n);
    game1.startGame();
}

