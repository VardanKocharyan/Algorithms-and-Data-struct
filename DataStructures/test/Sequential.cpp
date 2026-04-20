#include <iostream>
#include <vector>
#include "Sequantial.hpp"

int main() {

    std::cout << "\tTesting forward_list\n";

    my_forward_list<int> l{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "my_forward_list<int> l(4): " << l << '\n';
   
    my_forward_list<int> r(4, 6);
    std::cout << "my_forward_list<int> r(4, 6): " << r << '\n';

    my_forward_list<int> copyctor(r);
    std::cout << "my_forward_list<int> copyctor(r): " << copyctor  << "  r: " << r << '\n';

    my_forward_list<int> movector(std::move(r));
    std::cout << "my_forward_list<int> movector(r): " << movector << "  r: " << r <<  '\n';

    my_forward_list<int> init({1, 2, 3, 4, 5, 6});
    std::cout << "my_forward_list<int> init({1, 2, 3, 4, 5, 6}): " << init << '\n';

//  l.erase(l.begin()); // Error: no function erase()
    
    l.erase_after(l.befor_begin()); // Removes first element
    
    for (auto n : l)
        std::cout << n << ' ';
    std::cout << '\n';

    auto fi = std::next(l.begin());
    auto la = std::next(fi, 3);
    
    l.erase_after(fi, la);
    
    for (auto n : l)
        std::cout << n << ' ';
    std::cout << '\n';

    my_forward_list<int> ints{1, 2, 3, 4, 5};

    // insert_after (2)
    auto beginIt = ints.begin();
    ints.insert_after(beginIt, -6);
    std::cout << ints << '\n';

    // insert_after (3)
    auto anotherIt = beginIt;
    ++anotherIt;
    anotherIt = ints.insert_after(anotherIt, 2, -7);
    std::cout << ints << '\n';

    // insert_after (4)
    const std::vector<int> v = {-8, -9, -10};
    anotherIt = ints.insert_after(anotherIt, v.cbegin(), v.cend());
    std::cout << ints << '\n';

    ints.insert_after(anotherIt, {-11, -12, -13, -14});
    std::cout << ints << '\n';

    ints.sort();
    std::cout << "ascending: " << ints << '\n';

    ints.reverse();
    std::cout << "descending: " << ints << '\n';


    return 0;
}
