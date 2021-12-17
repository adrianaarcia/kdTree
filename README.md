# kdTree
Implementation of a variant of binary search trees. Last Updated: 11/21/2019

## Overview
A kd-tree is a data structure that allows for efficient nearest neighbor queries. A kd-tree is a binary search tree that rotates which coordinate it compares at each level of the tree. So for 2-dimensional data, the root might compare on x-coordinate and then the children of the root would compare on y-coordinate, then grandchildren of the root on x-coordinate again, and so on.

In order to keep the math simple, the points are  2-D Cartesian coordinates rather than geographic coordinates with latitudes and longitudes.

## Example
$ for N in `seq 1 14`; do echo === TEST $N ===; ./Unit $N | sort -n; done
=== TEST 1 ===
PASSED
=== TEST 2 ===
PASSED
=== TEST 3 ===
PASSED
=== TEST 4 ===
PASSED
=== TEST 5 ===
PASSED
=== TEST 6 ===
=== TEST 7 ===
15.071 -163.957
15.357 -157.807
16.698 -164.924
17.079 -162.024
18.085 -162.655
18.863 -163.994
19.097 -160.178
19.353 -162.740
19.712 -161.068
20.376 -162.999
21.833 -161.960
22.189 -157.489
22.215 -158.356
22.589 -157.028
22.858 -158.387
22.998 -156.598
23.420 -162.370
23.858 -157.537
24.797 -157.050
24.904 -164.680
=== TEST 8 ===
Distance: inf
=== TEST 9 ===
Distance: 0.000000
Point: 22.588670 -157.028378
=== TEST 10 ===
Distance: 6.324555
Point: 1.000000 1.000000
=== TEST 11 ===
Distance: 6.324555
Point: 1.000000 1.000000
=== TEST 12 ===
PASSED
=== TEST 13 ===
Distance: 0.000000
Point: 22.588670 -157.028378
=== TEST 14 ===
PASSED
