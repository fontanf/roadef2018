# ROADEF/EURO 2018 challenge (J28 Final submission)

A branch-and-bound algorithm for the ROADEF/EURO 2018 challenge 2D-packing problem.

## Compile

```
bazel build --cxxopt='-std=c++14' --compilation_mode=opt -- //lib:main
```

## Execute

```
./bazel-bin/lib/main -t 3600 -p instances/A15 -o solutions_3600/A15_solution.csv
```

## Benchmarks

```
chmod +x ./bench
./bench 3600
```

## Unit tests

```
bazel test --cxxopt='-std=c++14' --compilation_mode=dbg -- //lib:test
```

