#include "balloc.hpp"
#include "r_balloc.hpp"
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

template <typename Func, typename... Args>
double benchmark_once(Func func, Args &&...args) {
    using namespace std::chrono;

    auto t1 = high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;
    return ms_double.count();
}

template <typename Func, typename... Args>
void benchmark(int iterations, Func func, Args &&...args) {

    std::vector<double> elapsed_times;
    elapsed_times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        elapsed_times.push_back(
            benchmark_once(func, std::forward<Args>(args)...));
    }

    double average_time =
        std::accumulate(elapsed_times.begin(), elapsed_times.end(), 0.0) /
        iterations;

    std::cout << "Average time taken over " << iterations
              << " iterations: " << average_time << " milliseconds\n";
}

void test_up() {
    BumpUp<4294967295> b;

    while (b.alloc<int>(1)) {
        b.alloc<char>(1);
        b.alloc<short>(1);
        b.alloc<char>(1);
    }
    b.dealloc();
}
void test_down() {
    BumpDown<4294967295> b;

    while (b.alloc<int>(1)) {
        b.alloc<char>(1);
        b.alloc<short>(1);
        b.alloc<char>(1);
    }
    b.dealloc();
}

int main() {

    int iterations = 10;

    benchmark(iterations, test_up);
    benchmark(iterations, test_down);

    return 0;
}
