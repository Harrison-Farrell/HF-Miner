#include <SHA256/sha256.h>
#include <benchmark/benchmark.h>

#include <cstring>
#include <string>
#include <vector>

// Benchmark: compute hex digest for small input
static void BM_sha256_hex_small(benchmark::State &state) {
  const std::string input = "hello world";
  char out[SHA256_HEX_SIZE];

  for (auto _ : state) {
    SHA256::sha256_hex(input.data(), input.size(), out);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(BM_sha256_hex_small);

// Benchmark: compute hex digest for larger input (1 KiB)
static void BM_sha256_hex_1k(benchmark::State &state) {
  std::vector<uint8_t> input(1024);
  for (size_t i = 0; i < input.size(); ++i)
    input[i] = static_cast<uint8_t>(i & 0xff);

  char out[SHA256_HEX_SIZE];
  for (auto _ : state) {
    SHA256::sha256_hex(input.data(), input.size(), out);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(BM_sha256_hex_1k);

// Benchmark: compute raw bytes digest (small)
static void BM_sha256_bytes_small(benchmark::State &state) {
  const std::string input = "benchmark-test";
  uint8_t out[SHA256_BYTES_SIZE];

  for (auto _ : state) {
    SHA256::sha256_bytes(input.data(), input.size(), out);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(BM_sha256_bytes_small);

// Benchmark: streaming - append one byte at a time
static void BM_sha256_stream_per_byte(benchmark::State &state) {
  std::vector<uint8_t> input(4096);
  for (size_t i = 0; i < input.size(); ++i)
    input[i] = static_cast<uint8_t>(i & 0xff);

  char out[SHA256_HEX_SIZE];

  for (auto _ : state) {
    SHA256::sha256 sha;
    SHA256::sha256_init(&sha);
    for (uint8_t b : input) {
      // call append with single-byte chunks to simulate per-byte feeding
      SHA256::sha256_append(&sha, &b, 1);
    }
    SHA256::sha256_finalize_hex(&sha, out);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(BM_sha256_stream_per_byte);

// Benchmark: streaming - append bulk
static void BM_sha256_stream_bulk(benchmark::State &state) {
  std::vector<uint8_t> input(4096);
  for (size_t i = 0; i < input.size(); ++i)
    input[i] = static_cast<uint8_t>(i & 0xff);

  char out[SHA256_HEX_SIZE];

  for (auto _ : state) {
    SHA256::sha256 sha;
    SHA256::sha256_init(&sha);
    SHA256::sha256_append(&sha, input.data(), input.size());
    SHA256::sha256_finalize_hex(&sha, out);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(BM_sha256_stream_bulk);

// Benchmark: hashStringToArray (parsing hex -> bytes)
static void BM_hashStringToArray(benchmark::State &state) {
  // a valid 64-char hex string (all zeros -> "00" * 32)
  std::string hex(64, '0');
  for (auto _ : state) {
    auto arr = SHA256::hashStringToArray(hex);
    benchmark::DoNotOptimize(arr);
  }
}
BENCHMARK(BM_hashStringToArray);

// Benchmark: hashArrayToString (bytes -> hex string)
static void BM_hashArrayToString(benchmark::State &state) {
  SHA256::Hash arr;
  arr.fill(0xff);
  for (auto _ : state) {
    auto s = SHA256::hashArrayToString(arr);
    benchmark::DoNotOptimize(s);
  }
}
BENCHMARK(BM_hashArrayToString);

BENCHMARK_MAIN();
