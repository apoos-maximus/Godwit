#include <iostream>
#include <atomic>

int main(){
    std::atomic<int> a(23);
    int b = 23;

    std::cout << a << std::endl;

    a.compare_exchange_weak(b, 12);

    std::cout << a << std::endl;
}