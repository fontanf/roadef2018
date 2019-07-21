# ROADEF/EURO 2018 challenge

A branch-and-bound algorithm for the ROADEF/EURO 2018 challenge 2D-packing problem. Winner of the final phase.

Authors: [Luc Libralesso](https://github.com/librallu), Florian Fontan

![Example](example.png?raw=true "Title")

## Results

Please do not hesitate to contact us:
* if you find a better solution for any of the instances.
* if your algorithm beats ours on all datasets or on datasets B and X. The score for an instance is 10% of the value on a 180s run and 90% of the value on a 3600s run. The total score of an algorithm is the number of instances for which the score is lesser than the score of the other algorithm. The computer that should be used to evaluate programs is a Linux OS machine (Ubuntu 18.04) with 4 CPU (8 Threads) with 3.6GHz Intel Xeon W-2123 Processor, 48GB of RAM and a graphic card of 5GB. It is allowed to use both CPUs and GPUs on the machine.  

| Instance  |        Notes       | Solution 180s | Solution 3600s | Best known solution |          Found by          |
| :-------: | :----------------: | ------------: | -------------: | ------------------: | :------------------------: |
| A1        | Trivial            |      *425 486 |       *425 486 |             425 486 |                            |
| A2        | No prec            |     9 506 669 |     *4 383 509 |           4 383 509 |                            |
| A3        |                    |    *2 651 880 |     *2 651 880 |           2 651 880 |                            |
| A4        |                    |    *2 924 730 |     *2 924 730 |           2 924 730 |                            |
| A5        |                    |     3 566 133 |      3 283 653 |           3 017 223 |                            |
| A6        |                    |     3 389 640 |      3 225 930 |           3 188 646 |                            |
| A7        |                    |     4 703 760 |      4 334 610 |           3 920 520 |                            |
| A8        |                    |     9 691 844 |     *8 378 954 |           8 378 954 |                            |
| A9        |                    |    *2 664 276 |     *2 664 276 |           2 664 276 |                            |
| A10       |                    |    *4 084 381 |     *4 084 381 |           4 084 381 |                            |
| A11       |                    |     4 660 669 |      4 622 149 |           4 358 929 |                            |
| A12       |                    |     2 056 504 |     *1 879 954 |           1 879 954 |                            |
| A13       |                    |    10 226 883 |      9 440 433 |           9 331 293 |                            |
| A14       |                    |    11 686 638 |    *10 383 378 |          10 383 378 |                            |
| A15       |                    |    12 918 611 |     11 108 171 |          10 828 901 |                            |
| A16       | Trivial            |    *3 380 333 |     *3 380 333 |           3 380 333 |                            |
| A17       | Trivial, 2 stacks  |    *3 617 251 |     *3 617 251 |           3 617 251 |                            |
| A18       |                    |     5 596 728 |     *4 983 618 |           4 983 618 |                            |
| A19       |                    |     3 654 374 |     *3 323 744 |           3 323 744 |                            |
| A20       | Trivial, no prec   |    *1 467 925 |     *1 467 925 |           1 467 925 |                            |
|           |                    |               |                |                     |                            |
| B1        | No prec            |     3 765 558 |      3 136 398 |           2 661 318 |                            |
| B2        |                    |    14 312 915 |     13 398 065 |          11 931 095 |                            |
| B3        |                    |    19 786 463 |     17 093 273 |          15 786 803 | Vasquez and Buljubasic     |
| B4        |                    |     8 323 615 |      7 973 725 |           7 315 675 |                            |
| B5        | 2 stacks           |    72 155 615 |    *72 155 615 |          72 155 615 |                            |
| B6        |                    |    12 488 887 |     11 089 327 |          10 800 427 | Vasquez and Buljubasic     |
| B7        | No prec            |     9 177 579 |      7 678 509 |           6 628 839 |                            |
| B8        |                    |    17 152 939 |     15 840 049 |          14 398 759 | Vasquez and Buljubasic     |
| B9        |                    |    19 969 117 |     17 474 947 |          16 495 897 | Vasquez and Buljubasic     |
| B10       |                    |    26 904 563 |     23 065 403 |          21 951 533 |                            |
| B11       |                    |    27 312 710 |     23 820 230 |          20 626 280 | Vasquez and Buljubasic     |
| B12       |                    |    13 734 007 |     13 120 897 |          12 514 207 |                            |
| B13       |                    |    27 360 375 |     23 078 235 |          22 657 725 |                            |
| B14       |                    |     9 442 780 |      8 377 060 |           8 023 960 |                            |
| B15       |                    |    24 568 391 |     23 088 581 |          22 619 921 |                            |
|           |                    |               |                |                     |                            |
| X1        |                    |    15 302 657 |     14 127 797 |          13 720 127 |                            |
| X2        | No prec            |     6 083 087 |      4 879 337 |           4 795 877 |                            |
| X3        |                    |     7 649 626 |      7 180 966 |           6 837 496 |                            |
| X4        |                    |    15 488 372 |     13 366 562 |                     |                            |
| X5        |                    |     4 988 207 |      4 715 357 |           4 522 757 |                            |
| X6        |                    |    11 031 293 |      9 496 913 |           9 365 303 |                            |
| X7        |                    |    22 876 710 |     21 191 460 |          20 568 720 |                            |
| X8        | 2 stacks           |   *22 265 601 |    *22 265 601 |          22 265 601 |                            |
| X9        |                    |    22 312 215 |     20 479 305 |          20 039 535 |                            |
| X10       |                    |    18 778 322 |     17 186 162 |          16 865 162 |                            |
| X11       |                    |    12 802 752 |     11 676 042 |          11 011 572 |                            |
| X12       |                    |    12 358 675 |     10 503 295 |          10 246 495 |                            |
| X13       |                    |    14 345 172 |     13 125 372 |          12 130 272 |                            |
| X14       |                    |     8 591 012 |      7 644 062 |           7 422 572 |                            |
| X15       |                    |    13 710 924 |     11 682 204 |          10 882 914 |                            |

## Compile

```
bazel build -- //lib:main
```

## Execute

```
./bazel-bin/lib/main -v -t 3600 -p instances/A15 -o solutions_3600/A15_solution.csv
```

## Benchmarks

```
./bench
```

## Unit tests

```
bazel test --compilation_mode=dbg -- //lib:test
```

