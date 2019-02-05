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
Intel i7.

```
-- Preparing data for 100000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.000825
Attributes in separate arrays, warm:       0.000304
Position attributes in struct, cold:       0.000790
Position attributes in struct, warm:       0.000320
Attributes in one struct, cold:            0.001730
Attributes in one struct, warm:            0.000318
Attributes in one alloc'd struct, cold:    0.005215
Attributes in one alloc'd struct, warm:    0.000986
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 1000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.007096
Attributes in separate arrays, warm:       0.002616
Position attributes in struct, cold:       0.006967
Position attributes in struct, warm:       0.002492
Attributes in one struct, cold:            0.016007
Attributes in one struct, warm:            0.003966
Attributes in one alloc'd struct, cold:    0.045979
Attributes in one alloc'd struct, warm:    0.008424
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 1000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.003259
Attributes in separate arrays, warm:       0.002521
Position attributes in struct, cold:       0.003120
Position attributes in struct, warm:       0.002442
Attributes in one struct, cold:            0.006148
Attributes in one struct, warm:            0.003110
Attributes in one alloc'd struct, cold:    0.045164
Attributes in one alloc'd struct, warm:    0.008291
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 10000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.069618
Attributes in separate arrays, warm:       0.024966
Position attributes in struct, cold:       0.067765
Position attributes in struct, warm:       0.024406
Attributes in one struct, cold:            0.159106
Attributes in one struct, warm:            0.035251
Attributes in one alloc'd struct, cold:    0.466851
Attributes in one alloc'd struct, warm:    0.102340
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 50000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.359707
Attributes in separate arrays, warm:       0.137166
Position attributes in struct, cold:       0.355077
Position attributes in struct, warm:       0.135211
Attributes in one struct, cold:            0.833434
Attributes in one struct, warm:            0.184556
Attributes in one alloc'd struct, cold:    2.738726
Attributes in one alloc'd struct, warm:    0.780988
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 100000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       0.716531
Attributes in separate arrays, warm:       0.272533
Position attributes in struct, cold:       0.705356
Position attributes in struct, warm:       0.268394
Attributes in one struct, cold:            1.678093
Attributes in one struct, warm:            0.365475
Attributes in one alloc'd struct, cold:    7.647449
Attributes in one alloc'd struct, warm:    10.089399
-- Cleaning up data
-- Benchmarks done

-- Preparing data for 200000000 entities
-- Start benchmarks
Attributes in separate arrays, cold:       1.453424
Attributes in separate arrays, warm:       0.546632
Position attributes in struct, cold:       1.424962
Position attributes in struct, warm:       0.537129
Attributes in one struct, cold:            4.488125
Attributes in one struct, warm:            6.447619
Attributes in one alloc'd struct, cold:    15.015039
Attributes in one alloc'd struct, warm:    22.875200
-- Cleaning up data
-- Benchmarks done
```
