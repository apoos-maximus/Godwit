#include <iostream>
#include <thread>
#include <atomic>

#define INC 10000 

void doWork (std::atomic<int> *com){
    for (int i = 0; i < INC; i++){
        *com += 1;
    }
}

int main (int argc, char *argv[] ) {

    std::atomic<int> common(0);
    std::thread t1(&doWork, &common);
    std::thread t2(&doWork, &common);

    t1.join();
    t2.join();

    std::cout << common;


    return 0;
}