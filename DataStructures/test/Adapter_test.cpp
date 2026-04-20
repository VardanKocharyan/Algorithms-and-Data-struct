#include <iostream>
#include <string>
#include "Adapter.hpp"

int main() {
    std::cout << "\tStack testing\n";
    my_stack<int> st;

    std::cout << "pushing 3, 4\n";
    st.push(3);
    st.push(4);

    std::cout << "call pop(): " <<  st.pop() << '\n';

    std::cout << "call pop(): " <<  st.pop() << '\n';


    //std::cout << "call pop() for now exception working:\n";
    //st.pop();


    std::cout << "size of stack: " << st.size() << "\nis empty stack? : " <<  st.empty() << std::endl;

    //Queue
    std::cout << "\n\tQueue testing\n";
    my_queue<std::string> read;

    std::cout << "pushing \"World\" and \"Hello\" and \"brats\"\n";
    read.push("World");
    read.push("Hello");
    read.push("brats");

    std::cout << "call pop() : delete World\n";
    read.pop();

    std::cout << "calling front(): " << read.front() << '\n';
    std::cout << "calling back(): " << read.back() << '\n';

    std::cout << "size: " << read.size() << ", is queue empty? : " << read.empty() << std::endl;
    return 0;
}
