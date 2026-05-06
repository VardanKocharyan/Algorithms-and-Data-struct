#include <iostream>
#include "UnorderedAssociativeContainers.hpp"

int main() {

    my_unordered_map<int, std::string> def;

    my_unordered_map<int, int> param(13);

    my_unordered_map<int, int> copy(param);

    std::cout << copy << std::endl;

    my_unordered_map<int, int> move(std::move(param));



    return 0;
}
