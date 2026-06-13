#pragma once
#include <iostream>
#include <vector>

class Bobs_and_Alise {
private:
    std::vector<char> members;

public:
    Bobs_and_Alise() : members({'1', '2', 'A'}) {}
    void AllPositions();
    void dfs(int pos, std::vector<bool>& used, std::vector<char>& curr);
    void print(std::vector<char>& curr);
    bool isNotValid(int i, int pos, std::vector<bool>& used);

};

void Bobs_and_Alise::AllPositions() {
    std::vector<bool> used(3, false);
    std::vector<char> curr;
    int pos{};

    dfs(pos, used, curr);
}

void Bobs_and_Alise::dfs(int pos, std::vector<bool>& used, std::vector<char>& curr) {
    if (pos == 3) {
        print(curr);
        return;
    }

    for (int i{}; i < 3; ++i) {
        if (isNotValid(i, pos, used)) continue;

        used[i] = true;
        curr.push_back(members[i]);

        dfs(pos + 1, used, curr);

        used[i] = false;
        curr.pop_back();
    }
}

bool Bobs_and_Alise::isNotValid(int i, int pos, std::vector<bool>& used) {
    return used[i] || (pos == 1 && members[i] == 'A');
}

void Bobs_and_Alise::print(std::vector<char>& curr) {
    for (char c : curr) {
        switch (c) {
            case '1' : std::cout << "Bob1 "; break;
            case '2' : std::cout << "Bob2 "; break;
            default  : std::cout << "Alise ";
        }
    }
    std::cout << std::endl;
}



