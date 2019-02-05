# vectorize_test
This is a small test program that tests the performance difference between vectorized
and non-vectorized code. When comparing the worst with the best case, the test can
run as much as 40x faster.

## Run the code
This application requires the bake build system. To install bake, see:
https://github.com/SanderMertens/bake

After bake is installed, do:
```
bake clone SanderMertens/vectorize_test
bake run vectorize_test --cfg release
```

## Description
Four different scenarios are tested, each adding a floating point `speed` 
value to an `x` and `y` value. The scenarios are only different in the way that
the data is stored in memory.

Each test is ran twice, to show the difference between a "cold" and "warm" test.
The second time the test is ran, data is already in the CPU cache, and as a
result, the test runs significantly faster.

### Attributes in separate arrays
In this scenario, each attribute (`x`, `y`, `speed`) is in its own separate array.

### Position attributes in same struct
In this scenario, the `x` and `y` attributes are in a `Position` struct. The
`Position` struct and `speed` data are in separate arrays.

### All attributes in the same struct
In this scenario the `x`, `y` and `speed` data are all in an `Entity` struct. To
mimic actual OOP-style applications, this struct also has additional members
which are not evaluated by the test. All entities are stored in the same array.

### All attributes in the same struct, each entity its on heap alloc
This scenario uses the same `Entity` struct, but instead of storing all entities
in the same array, all entities are allocated separately on the heap. To mimic
actual applications, small chunks of "garbage" data is allocated inbetween the
entities, to more accurately simulate OOP applications where objects of the same
kind are typically scattered across the heap.

## Benchmarks
Here are the benchmarks as measured on a 15-inch 2018 Macbook with a 2.6Ghz 
Intel i7, on MacOS 10.14.1. While I was testing, I used these compilation options
to verify if code was being vectorized:

```
-Rpass=loop-vectorize -Rpass-missed=loop-vectorized -Rpass-analysis=loop-vectorize -fsave-optimization-record
```

The used compiler is clang:

```
$ clang --version
Apple LLVM version 10.0.0 (clang-1000.10.44.4)
Target: x86_64-apple-darwin18.2.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
```

### Graph (10 million entities):
![benchmarks](images/benchmark.png)

### Raw numbers:
```
-- Preparing data for 100000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.000488 (V)
Attributes in separate arrays, warm:       0.000041 (V)
Position attributes in struct, cold:       0.000515 (V)
Position attributes in struct, warm:       0.000051 (V)
Attributes in one struct, cold:            0.001522
Attributes in one struct, warm:            0.000397
Attributes in one alloc'd struct, cold:    0.005085
Attributes in one alloc'd struct, warm:    0.000788
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 1000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.005154 (V)
Attributes in separate arrays, warm:       0.000744 (V)
Position attributes in struct, cold:       0.005031 (V)
Position attributes in struct, warm:       0.000676 (V)
Attributes in one struct, cold:            0.014226
Attributes in one struct, warm:            0.003126
Attributes in one alloc'd struct, cold:    0.047704
Attributes in one alloc'd struct, warm:    0.007410
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 1000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.001332 (V)
Attributes in separate arrays, warm:       0.000645 (V)
Position attributes in struct, cold:       0.001396 (V)
Position attributes in struct, warm:       0.000755 (V)
Attributes in one struct, cold:            0.005490
Attributes in one struct, warm:            0.002581
Attributes in one alloc'd struct, cold:    0.045237
Attributes in one alloc'd struct, warm:    0.006892
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 10000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.051307 (V)
Attributes in separate arrays, warm:       0.006719 (V)
Position attributes in struct, cold:       0.051314 (V)
Position attributes in struct, warm:       0.007922 (V)
Attributes in one struct, cold:            0.147165
Attributes in one struct, warm:            0.026485
Attributes in one alloc'd struct, cold:    0.453954
Attributes in one alloc'd struct, warm:    0.079158
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 50000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.250698 (V)
Attributes in separate arrays, warm:       0.034717 (V)
Position attributes in struct, cold:       0.263129 (V)
Position attributes in struct, warm:       0.049825 (V)
Attributes in one struct, cold:            0.772695
Attributes in one struct, warm:            0.169159
Attributes in one alloc'd struct, cold:    2.792307
Attributes in one alloc'd struct, warm:    3.565040
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 100000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.514042 (V)
Attributes in separate arrays, warm:       0.092840 (V)
Position attributes in struct, cold:       0.596283 (V)
Position attributes in struct, warm:       0.108926 (V)
Attributes in one struct, cold:            1.678798
Attributes in one struct, warm:            0.374491
Attributes in one alloc'd struct, cold:    6.911441
Attributes in one alloc'd struct, warm:    10.208863
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 200000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       1.064743 (V)
Attributes in separate arrays, warm:       0.194111 (V)
Position attributes in struct, cold:       1.069347 (V)
Position attributes in struct, warm:       0.197115 (V)
Attributes in one struct, cold:            4.501781
Attributes in one struct, warm:            6.131327
Attributes in one alloc'd struct, cold:    15.995863
Attributes in one alloc'd struct, warm:    23.364604
-- Cleaning up data
-- Benchmarks done
```
