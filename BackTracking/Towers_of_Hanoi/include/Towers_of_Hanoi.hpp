#pragma once 
#include <iostream>

class Towers_of_Hanoi {
private:
    int count;

public:
    Towers_of_Hanoi(int c = 3) : count(c) {};
    ~Towers_of_Hanoi() = default;

    void startGame();
    void toh(char A, char B, char C, int n);
};

void Towers_of_Hanoi::startGame() {
    char A = 'A';
    char B = 'B';
    char C = 'C';
    
    toh(A, B, C, count);
}

void Towers_of_Hanoi::toh(char A, char B, char C, int n) {
    if (!n) return; 
    toh(A, C, B, n - 1);
    std::cout << A << "->" << C << '\n';
    toh(B, A, C, n - 1);
}

