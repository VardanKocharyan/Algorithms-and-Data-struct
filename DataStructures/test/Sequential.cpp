#include <iostream>
#include "Sequantial.hpp"

int main() {

    std::cout << "\tTesting forward_list\n";

    my_forward_list<int> l(4);
    std::cout << "my_forward_list<int> l(4): " << l << '\n';
   
    my_forward_list<int> r(4, 6);
    std::cout << "my_forward_list<int> r(4, 6): " << r << '\n';

    my_forward_list<int> copyctor(r);
    std::cout << "my_forward_list<int> copyctor(r): " << copyctor  << "  r: " << r << '\n';

    my_forward_list<int> movector(std::move(l));
    std::cout << "my_forward_list<int> movector(l): " << movector << "  l: " << l <<  '\n';


    my_forward_list<int> init({1, 2, 3, 4, 5, 6});
    std::cout << "my_forward_list<int> init({1, 2, 3, 4, 5, 6}): " << init << '\n';

    
    return 0;
}
