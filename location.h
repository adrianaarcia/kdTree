#ifndef __LOCATION_H__
#define __LOCATION_H__

typedef struct _point
{
  double x;
  double y;
} location;

/**
 * Returns the Euclidean distance between the points.
 *
 * @param l1 a pointer to a location, non-NULL
 * @param l2 a pointer to alocation, non-NULL
 * @return the distance between those points
 */
double location_distance(const location *l1, const location *l2);

/**
 * Returns the minimum Euclidean distance between the given point and the
 * rectangle defined by the given lower left and upper right corners.
 *
 * @param loc a pointner to a location, non-NULL
 * @param lower_left a pointer to a location, non-NULL
 * @param upper_right a pointer to a location whose coordinates are greater
 * than or equal to the corresponding coordinates in the location
 * pointed to by upper_left
 * @return the minimum distance from l1 to the rectangle defined by
 * lower_left and upper_right
 */
double location_distance_to_rectangle(const location *loc, const location *lower_left, const location *upper_right);

#endif
