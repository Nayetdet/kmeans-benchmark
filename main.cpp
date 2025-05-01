#include <benchmark/benchmark.h>
#include <chrono>
#include <iostream>
#include <stdexcept>

#include "kmeans.h"
#include "pgm.h"

constexpr unsigned K = 10;
constexpr unsigned KMEANS_MAX_ITERATIONS = 100;
constexpr unsigned BENCHMARK_NUM_ITERATIONS = 250;

static void PerformKMeans(const std::string& imagePath) {
    PGM *pgm = readPGM(imagePath.c_str());
    if (!pgm) {
        throw std::invalid_argument("Failed to read PGM file");
    }

    KMeansData kd = {
        .size = pgm->width * pgm->height,
        .maxValue = pgm->maxValue,
        .data = pgm->data
    };

    if (!applyKMeans(&kd, K, KMEANS_MAX_ITERATIONS)) {
        freePGM(pgm);
        throw std::runtime_error("KMeans application failed");
    }

    freePGM(pgm);
}

static void BM_PerformKMeans(benchmark::State& state, const std::string& imagePath) {
    auto start = std::chrono::high_resolution_clock::now();
    for (auto _ : state) {
        PerformKMeans(imagePath);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.counters["TotalTimeSec"] = benchmark::Counter(elapsed_seconds.count(), benchmark::Counter::kDefaults);
    state.counters["IterationsPerSec"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path:string>" << std::endl;
        return 1;
    }

    std::string imagePath = argv[1];
    benchmark::RegisterBenchmark("BM_PerformKMeans", BM_PerformKMeans, imagePath)
        ->Iterations(BENCHMARK_NUM_ITERATIONS)
        ->Unit(benchmark::kMillisecond);

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}
