#ifndef __KDTREE_H__
#define __KDTREE_H__

#include <stdbool.h>

#include "location.h"

/**
 * A set of geographic locations given by latitude and longitude
 * implemented using a k-d tree, where k = 2.  For all functions,
 * points are considered different based on the values of the
 * coordinates that are passed in, even if the coordinates represent
 * the same physical point, so, for example, longitude -180 is
 * considered different than longitude 180.
 */
typedef struct _kdtree kdtree;

/**
 * Creates a balanced k-d tree containing copies of the points in the
 * given array of locations.  If the array is NULL and n is 0 then the
 * returned tree is empty.  If the tree could not be created then the
 * returned value is NULL.
 *
 * @param pts an array of unique valid locations, non-NULL if n != 0
 * @param n the number of points in that array, or 0 if pts is NULL
 * @return a pointer to the newly created collection of points, or NULL
 * if it could not be created
 */
kdtree *kdtree_create(const location *pts, int n);

/**
 * Adds a copy of the given point to the given k-d tree.  There is no
 * effect if the point is already in the tree.  The tree need not be
 * balanced after the add.  The return value is true if the point was
 * added successfully and false otherwise (if the point was already in the
 * tree or if there was a memory allocation error when trying to add).
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param p a pointer to a valid location, non-NULL
 * @return true if and only if the point was successfully added
 */
bool kdtree_add(kdtree *t, const location *p);

/**
 * Determines if the given tree contains a point with the same coordinates
 * as the given point.
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param p a pointer to a valid location, non-NULL
 * @return true if and only of the tree contains the location
 */
bool kdtree_contains(const kdtree *t, const location *p);

/**
 * Copies the nearest neighbor to the given point among points in the
 * given tree to the given location.  Ties are broken arbitrarily.
 * There is no change to neighbor and distance is set to infinity if
 * the tree is empty.  If p is equal to a point in the tree then p is
 * copied into neighbor and distance is set to zero.
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param p a pointer to a valid location, non-NULL
 * @param d a pointer to a double, non-NULL
 * @param neighbor a pointer to a valid location, non-NULL
 */
void kdtree_nearest_neighbor(kdtree *t, const location *p, location *neighbor, double *d);

/**
 * Passes the points in the given tree to the given function
 * in an arbitrary order.  The last argument to this function is also passed
 * to the given function along with each point.
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param f a pointer to a function that takes a location and an extra
 * argument, and does not modify t, non-NULL
 * @param arg a pointer
 */
void kdtree_for_each(const kdtree* r, void (*f)(const location *, void *), void *arg);

/**
 * Destroys the given k-d tree.  The tree is invalid after being destroyed.
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 */
void kdtree_destroy(kdtree *t);

#endif
