#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#include "kdtree.h"
#include "location.h"

void test_add(const location *, int n);
void test_build(const location *, int n);
void test_contains(int n);
void test_for_each(int n);
void test_nearest(const location *points, int n, const location *query);
void test_nearest_after_build(const location *points, int n, const location *query);
void test_nearest_random(int n);

void print_point_formatted(const location *l, void *a);
void print_point(const location *, void *);
void brute_force_nearest_neighbor(location *pts, int n, const location *p, location *neighbor, double *d);

location nearest_points[] = {{0, 3}, {100, 0}, {1, 1}, {99, -5}, {101, -4}, {-1, -5000}, {-5000, -5}, {98, -3}, {102, -2}, {-1, -5}};
int nearest_count = sizeof(nearest_points) / sizeof(location);

#define LARGE_TEST_SIZE (1000000)

location test_points[] =
  {
   {24.904359601287595, -164.679680919231197},
   {22.1889723646466788, -157.4889113459804237},
   {22.9975176700462036, -156.5980384054300085},
   {19.352792614743985, -162.739619141829996},
   {19.71197032632596, -161.0682580028004942},
   {24.796794566495814, -157.0503041973512346},
   {22.858083728266023, -158.3873640532406428},
   {15.356712575011505, -157.8071684212029604},
   {23.4204901549550186, -162.370437601685534},
   {23.8584532617037777, -157.5370774004888363},
   {22.588669770893219, -157.028378147416816},
   {19.0969835095968481, -160.17803538487762616},
   {22.2147131873924506, -158.355708807669218},
   {16.697782049411713, -164.92397630464659},
   {20.3756508986791074, -162.999269739764995},
   {17.079064596253004, -162.023667312991769},
   {18.8630989313068347, -163.993546207602046},
   {18.0848192792889675, -162.654559728282388},
   {21.8331905048543264, -161.9598511838378752},
   {15.070625181945118, -163.9570156703153074}
  };
int test_count = sizeof(test_points) / sizeof(location);

location example_1[] =
  {
   {12.0, 8.0},
   {18.0, 11.0},
   {15.0, 15.0},
   {8.0, 12.0},
   {3.0, 18.0},
   {10.0, 1.0},
   {17.0, 5.0}
  };

int example_1_count = sizeof(example_1) / sizeof(location);

location example_2[] =
  {
   {6.0, 3.0},
   {2.0, 17.0},
   {15.0, 20.0},
   {2.0, 3.0},
   {17.0, 13.0},
   {3.0, 9.0},
   {10.0, 11.0},
   {7.0, 7.0},
   {9.0, 12.0},
   {16.0, 11.0},
   {15.0, 2.0},
   {19.0, 4.0}
  };
int example_2_count = sizeof(example_2) / sizeof(location);

location example_query = {4, 5};

int main(int argc, char **argv)
{
  int test = 0;
  location nearest_points_swapped[nearest_count];
  for (int i = 0; i < nearest_count; i++)
    {
      nearest_points_swapped[i].x = nearest_points[i].y;
      nearest_points_swapped[i].y = nearest_points[i].x;
    }					      
  
  if (argc > 1)
    {
      test = atoi(argv[1]);
    }

  switch (test)
    {
    case 1:
      test_add(test_points, 0);
      break;

    case 2:
      test_add(test_points, 1);
      break;

    case 3:
      test_contains(1);
      break;
      
    case 4:
      test_add(test_points, test_count / 2);
      break;

    case 5:
      test_contains(test_count / 2);
      break;
      
    case 6:
      test_for_each(0);
      break;

    case 7:
      test_for_each(test_count);
      break;

    case 8:
      test_nearest(nearest_points, 0, &nearest_points[0]);
      break;
      
    case 9:
      test_nearest(test_points, test_count, &test_points[test_count / 2]);
      break;

    case 10:
      test_nearest(nearest_points, nearest_count - 1, &nearest_points[nearest_count - 1]);
      break;

    case 11:
      test_nearest(nearest_points_swapped, nearest_count - 1, &nearest_points_swapped[nearest_count - 1]);
      break;

    case 12:
      test_build(test_points, test_count);
      break;

    case 13:
      test_nearest_after_build(test_points, test_count, &test_points[test_count / 2]);
      break;

    case 14:
      {
	int n = LARGE_TEST_SIZE;
	if (argc > 2)
	  {
	    n = atoi(argv[2]);
	    if (argc > 3)
	      {
		srand(atoi(argv[3]));
	      }
	  }
	test_nearest_random(n);
      }
      break;

    case 15:
      test_add(example_1, example_1_count);
      break;

    case 16:
      test_add(example_2, example_2_count);
      break;

    case 17:
      test_build(example_1, example_1_count);
      break;

    case 18:
      test_build(example_2, example_2_count);
      break;

    case 19:
      test_nearest_after_build(example_1, example_1_count, &example_query);
      break;

    case 20:
      test_nearest_after_build(example_2, example_2_count, &example_query);
      break;

    default:
      fprintf(stderr, "USAGE: %s test-number\n", argv[0]);
      return 1;
    }

  return 0;
}

void print_point(const location *loc, void *a)
{
  printf("%f %f\n", loc->x, loc->y);
}

void test_add(const location *test_points, int n)
{
  kdtree *t = kdtree_create(NULL, 0);

  if (t == NULL)
    {
      printf("FAILED -- could not create tree\n");
      return;
    }
  
  for (int i = 0; i < n; i++)
    {
      if (!kdtree_add(t, &test_points[i]))
	{
	  printf("FAILED -- could not add point %f %f\n", test_points[i].x, test_points[i].y);
	  kdtree_destroy(t);
	  return;
	}
      
      if (kdtree_add(t, &test_points[i]))
	{
	  printf("FAILED -- added duplicate point %f %f\n", test_points[i].x, test_points[i].y);
	  kdtree_destroy(t);
	  return;
	}
    }
  
  kdtree_destroy(t);
  printf("PASSED\n");
}

void test_build(const location *points, int n)
{
  kdtree *t = kdtree_create(points, n);

  if (t == NULL)
    {
      printf("FAILED -- could not create tree\n");
      return;
    }
  
  for (int i = 0; i < n; i++)
    {
      if (!kdtree_contains(t, &points[i]))
	{
	  printf("FAILED -- missing point %f %f\n", points[i].x, points[i].y);
	  kdtree_destroy(t);
	  return;
	}
    }
  
  kdtree_destroy(t);
  printf("PASSED\n");
}

void test_contains(int n)
{
  kdtree *t = kdtree_create(NULL, 0);

  if (t == NULL)
    {
      printf("FAILED -- could not create tree\n");
      return;
    }

  for (int i = 0; i < n; i++)
    {
      if (!kdtree_add(t, &test_points[i]))
	{
	  printf("FAILED -- could not add point %f %f\n", test_points[i].x, test_points[i].y);
	  kdtree_destroy(t);
	  return;
	}
    }
  
  for (int i = 0; i < n; i++)
    {
      if (!kdtree_contains(t, &test_points[i]))
	{
	  printf("FAILED -- missing point %f %f\n", test_points[i].x, test_points[i].y);
	  kdtree_destroy(t);
	  return;
	}
    }
  
  for (int i = n; i < test_count; i++)
    {
      if (kdtree_contains(t, &test_points[i]))
	{
	  printf("FAILED -- contains point %f %f\n", test_points[i].x, test_points[i].y);
	  kdtree_destroy(t);
	  return;
	}
    }
  
  kdtree_destroy(t);
  printf("PASSED\n");
}

void test_nearest(const location *points, int n, const location *query)
{
  // build the tree by adding points one-by-one
  kdtree *t = kdtree_create(NULL, 0);
  for (int i = 0; i < n; i++)
    {
      kdtree_add(t, &points[i]);
    }

  // get the nearest neighbor to our query point
  location nearest;
  double distance;
  kdtree_nearest_neighbor(t, query, &nearest, &distance);

  // output the result
  printf("Distance: %f\n", distance);
  if (distance < INFINITY)
    {
      print_point_formatted(&nearest, "Point: %f %f\n");
    }
  
  kdtree_destroy(t);
}

void test_nearest_after_build(const location *points, int n, const location *query)
{
  // build the tree all at once (should be balanced)
  kdtree *t = kdtree_create(points, n);

  // get the nearest neighbor to our query point
  location nearest;
  double distance;
  kdtree_nearest_neighbor(t, query, &nearest, &distance);

  // output the result
  printf("Distance: %f\n", distance);
  if (distance < INFINITY)
    {
      print_point_formatted(&nearest, "Point: %f %f\n");
    }
  
  kdtree_destroy(t);
}

void test_nearest_random(int n)
{
  // make an array of random points
  location *random_points = malloc(sizeof(location) * (n + 1));
  if (random_points == NULL)
    {
      printf("FAILED -- could not allocate test points\n");
      return;
    }

  for (int i = 0; i < n + 1; i++)
    {
      random_points[i].x = (double)rand() / INT_MAX * 1000.0;
      random_points[i].y = (double)rand() / INT_MAX * 1000.0;
    }
  location query = random_points[n];
  
  // build a tree using those points
  kdtree *random = kdtree_create(random_points, n);

  // get the nearest neighbor using the kdtree algorithm
  location neighbor;
  double distance;
  kdtree_nearest_neighbor(random, &query, &neighbor, &distance);

  // get the nearest neighbor by brute force
  location bf_neighbor;
  double bf_distance;
  brute_force_nearest_neighbor(random_points, n, &query, &bf_neighbor, &bf_distance);

  // free points and tree
  free(random_points);
  kdtree_destroy(random);

  // check the results
  if (distance != bf_distance || (distance < INFINITY && (neighbor.x != bf_neighbor.x || neighbor.y != bf_neighbor.y)))
    {
      printf("FAILED -- for %f %f got %f to %f %f instead of %f to %f %f\n", query.x, query.y, distance, neighbor.x, neighbor.y, bf_distance, bf_neighbor.x, bf_neighbor.y);
    }
  else
    {
      printf("PASSED\n");
    }
}

void test_for_each(int n)
{
  kdtree *t = kdtree_create(NULL, 0);

  if (t == NULL)
    {
      printf("FAILED -- could not create tree\n");
      return;
    }
  else
    {
      for (int i = 0; i < n; i++)
	{
	  if (!kdtree_add(t, &test_points[i]))
	    {
	      printf("FAILED -- could not add point %f %f\n", test_points[i].x, test_points[i].y);
	      kdtree_destroy(t);
	      return;
	    }
	}
    }
  
  kdtree_for_each(t, print_point_formatted, "%.3f %.3f\n");
  
  kdtree_destroy(t);
}

void print_point_formatted(const location *l, void *a)
{
  const char *format = a;

  printf(format, l->x, l->y);
}

void brute_force_nearest_neighbor(location *pts, int n, const location *p, location *neighbor, double *d)
{
  if (n == 0)
    {
      *d = INFINITY;
    }
  else
    {
      *d = location_distance(&pts[0], p);
      *neighbor = pts[0];
      for (int i = 1; i < n; i++)
	{
	  double curr_d = location_distance(&pts[i], p);
	  if (curr_d < *d)
	    {
	      *d = curr_d;
	      *neighbor = pts[i];
	    }
	}
    }
}
