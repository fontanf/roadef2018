Note: the algorithm has been re-implemented in [a new package](https://github.com/fontanf/packingsolver) which solves a large variety of additional packing problems. Please refer to it for up-to-date results.

# ROADEF/EURO 2018 challenge

A branch-and-bound algorithm for the ROADEF/EURO 2018 challenge 2D-packing problem. Winner of the final phase.

Authors: [Luc Libralesso](https://github.com/librallu), Florian Fontan

![Example](example.png?raw=true "Title")

Compile:
```
bazel build -- //lib:main
```

Execute:
```
./bazel-bin/lib/main -v -t 3600 -p instances/A15 -o solutions_3600/A15_solution.csv
```

Benchmarks:
```
./bench
```

Unit tests:
```
bazel test --compilation_mode=dbg -- //lib:test
```

