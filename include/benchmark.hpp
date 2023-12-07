#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

/**
 * @brief A simple benchmarking class for measuring the runtime of functions.
 */
class Benchmark {
  public:
    /**
     * @brief Default constructor for Benchmark.
     */
    Benchmark() {
        num_benchmars = 0;
        num_iterations = 1;
    }

    /**
     * @brief Constructor for Benchmark with a specified number of iterations.
     * @param iters The number of iterations for each benchmark.
     */
    Benchmark(int iters) {
        num_benchmars = 0;
        num_iterations = iters;
    }

    /**
     * @brief Measures the runtime of a given function over multiple iterations.
     * @tparam Func The type of the function to be benchmarked.
     * @tparam Args The types of arguments for the function.
     * @param name A descriptive name for the benchmark.
     * @param func The function to be benchmarked.
     * @param args The arguments to be passed to the function.
     */
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

    /**
     * @brief Prints the benchmark results in a tabular format.
     */
    void print() {
        if (!num_benchmars)
            return;

        // Set the width for each column
        const int column_width = 15;

        std::cout << std::left << std::setw(column_width) << "Name"
                  << std::setw(column_width) << "Result (ms)"
                  << "Relative (%)" << std::endl;

        // Print each row in the table
        for (int i = 0; i < num_benchmars; i++) {
            std::cout << std::left << std::setw(column_width) << names[i]
                      << std::fixed << std::setw(column_width)
                      << std::setprecision(8) << results[i]
                      << std::setprecision(8) << relative(results[i])
                      << std::endl;
        }
        std::cout << std::endl;
    }

  private:
    int num_benchmars;  /**< Number of benchmarks performed. */
    int num_iterations; /**< Number of iterations for each benchmark. */
    std::vector<const char *> names; /**< Names of benchmarks. */
    std::vector<double> results;     /**< Results of benchmarks. */

    /**
     * @brief Calculates the relative percentage difference from the first
     * benchmark result.
     * @param runtime The runtime of the current benchmark.
     * @return The relative percentage difference.
     */
    inline double relative(double runtime) {
        return 100 * (results[0] - runtime) / results[0];
    }
};
