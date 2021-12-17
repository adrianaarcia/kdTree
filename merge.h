#ifndef __MERGE_H__
#define __MERGE_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "location.h"

bool x_sorted(const location p1, const location p2);

bool y_sorted(const location p1, const location p2);

void merge(int n1, const location a1[], int n2, const location a2[], location out[], bool (*f)(const location, const location));

void mergeSort(int n, const location a[], location out[], bool (*f)(const location, const location));

#endif