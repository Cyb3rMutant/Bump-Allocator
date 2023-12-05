#include "balloc.hpp"
#include "r_balloc.hpp"
#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

inline constexpr size_t MAX = 9876;

class Benchmark {
  public:
    Benchmark() {
        num_benchmars = 0;
        num_iterations = 1;
    }

    Benchmark(int iters) {
        num_benchmars = 0;
        num_iterations = iters;
    }

    template <typename Func, typename... Args>
    void benchmark(const char *name, Func func, Args &&...args) {

        using namespace std::chrono;

        std::vector<double> elapsed_times;
        elapsed_times.reserve(num_iterations);

        for (int i = 0; i < num_iterations; ++i) {

            time_point t1 = high_resolution_clock::now();
            func(std::forward<Args>(args)...);
            time_point t2 = high_resolution_clock::now();

            duration<double, std::milli> ms_double = t2 - t1;
            elapsed_times.push_back(ms_double.count());
        }

        double result =
            std::accumulate(elapsed_times.begin(), elapsed_times.end(), 0.0) /
            num_iterations;

        names.push_back(name);
        results.push_back(result);
        num_benchmars++;
    }

    void print() {
        if (!num_benchmars)
            return;

        for (int i = 0; i < num_benchmars; i++) {
            std::cout << names[i] << "\t||\t" << results[i] << "\t||\t"
                      << relative(results[i]) << std::endl;
        }
    }

  private:
    int num_benchmars;
    int num_iterations;
    std::vector<const char *> names;
    std::vector<double> results;

    inline double relative(double runtime) {
        return 100 * (results[0] / runtime);
    }
};

void test_up() {
    BumpUp<MAX> b;

    for (int i = 0; i < 5; i++) {
        while (b.alloc<int>(1)) {
            b.alloc<char>(1);
            b.alloc<short>(1);
            b.alloc<char>(1);
        }
        b.force_dealloc();
    }
}

void test_down() {
    BumpDown<MAX> b;

    for (int i = 0; i < 5; i++) {
        while (b.alloc<int>(1)) {
            b.alloc<char>(1);
            b.alloc<short>(1);
            b.alloc<char>(1);
        }
        b.force_dealloc();
    }
}

int main() {
    Benchmark b(500);

    b.benchmark("up1", test_up);
    b.benchmark("down1", test_down);
    b.benchmark("up2", test_up);
    b.benchmark("down2", test_down);
    b.benchmark("up3", test_up);
    b.benchmark("down3", test_down);
    b.benchmark("up4", test_up);
    b.benchmark("down4", test_down);

    b.print();
    return 0;
}
