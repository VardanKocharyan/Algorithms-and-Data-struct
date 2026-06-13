#pragma once
#include <iostream>
#include <vector>
#include <string>

class Knights_Tour {
private: 
    std::vector<std::vector<std::string>> board;
    std::vector<std::pair<int, int>> steps;

    bool GoAround(int i, int j, int timer);
    bool isValid(int i, int j);

public:
    Knights_Tour(int n = 5);
    ~Knights_Tour() = default;

    bool GoAround();
    void printBoard();

};

Knights_Tour::Knights_Tour(int n) : board(n, std::vector<std::string>(n, "￼")) {
    if (n < 5) {
        board.clear();
        std::cerr << "Haven't Solution!!!" << std::endl;
    }

    steps = {   {-2, 1}, 
                {-2, -1},
                {-1, -2},
                {-1, 2},
                {2, 1},
                {2, -1},
                {1, 2},
                {1, -2}
            };
}

bool Knights_Tour::GoAround() {
    return GoAround(0, 0, 1);
}

bool Knights_Tour::GoAround(int i, int j, int timer) {
    if (timer / board.size() == board.size()) {
        board[i][j] = "♘";
        printBoard();
        return true;
    }

    board[i][j] = "♘";
    printBoard();

    for (const auto& [n_i, n_j] : steps) {
        board[i][j] = std::to_string(timer);

        if (isValid(i + n_i, j + n_j) && GoAround(i + n_i, j + n_j, timer + 1)) return true;
        board[i][j] = "♘";
        printBoard();
    }
   
    board[i][j] = "￼";
    
    return false;
}

bool Knights_Tour::isValid(int i, int j) {
    int n = board.size();
    return ((i < n && i >=0) && (j < n && j >= 0)) && board[i][j] == "￼";
}


void Knights_Tour::printBoard() {
    for (const auto& v : board) {
        for (const auto& s : v) {
            std::cout << s << " ";
        }
        std::cout << '\n';
    }
    std::cout << "\n\n";
}












