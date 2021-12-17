#include "merge.h"

bool x_sorted(const location p1, const location p2)
{
    if(p1.x < p2.x)
    {
        return true;
    }
    else if(p1.x > p2.x)
    {
        return false;
    }
    else if (p1.y < p2.y)
    {
        return true;
    }
    else return false;
}

bool y_sorted(const location p1, const location p2)
{
    if(p1.y < p2.y)
    {   
        return true;
    }
    else if (p1.y > p2.y)
    {
        return false;
    }
    else if (p1.x < p2.x)
    {
        return true;
    }
    else return false;
}

/* merge sorted arrays a1 and a2, putting result in out */
void merge(int n1, const location a1[], int n2, const location a2[], location out[], bool (*f)(const location, const location))
{
    int i1;
    int i2;
    int iout;

    i1 = i2 = iout = 0;

    while(i1 < n1 || i2 < n2) 
    {
        if(i2 >= n2 || ((i1 < n1) && f(a1[i1], a2[i2]))) 
        {
            /* a1[i1] exists and is smaller */
            out[iout++] = a1[i1++];
        } 
        else 
        {
            /* a1[i1] doesn't exist, or is bigger than a2[i2] */
            out[iout++] = a2[i2++];
        }
    }
}

void mergeSort(int n, const location a[], location out[], bool (*f)(const location, const location))
{
    location* a1;
    location* a2;

    if(n < 2)
    {
        /* 0 or 1 elements is already sorted */
        memcpy(out, a, sizeof(location)*n);
    }
    else
    {
        /* sort into temp arrays */
        a1 = malloc(sizeof(location) * (n/2));
        a2 = malloc(sizeof(location) * (n - n/2));

        mergeSort(n/2, a, a1, f);
        mergeSort(n - n/2, a + n/2, a2, f);

        merge(n/2, a1, n - n/2, a2, out, f);

        /* free the temp arrays */
        free(a1);
        free(a2);
    }
}