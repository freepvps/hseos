// g++ main.cpp -o main -O1 -pedantic -Wall -lpthread --std=c++1z
#include <future>

int main() {
    std::future<int> r = std::async(std::launch::deferred, [] () { return 0; });
    return r.get();
}
