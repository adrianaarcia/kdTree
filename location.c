#include <stddef.h>
#include <math.h>

#include "location.h"

/**
 * Returns the distance between the two locations on the Earth's surface,
 * assuming a spherical earth with radius 6371 km.  If either location
 * is invalid, the return value is NaN.
 *
 * @param l1 a pointer to a location, non-NULL
 * @param l2 a pointer to a location, non-NULL
 * @return the distance between those points, in kilometers
 */
double location_distance(const location *l1, const location *l2)
{
  return sqrt(pow(l2->x - l1->x, 2) + pow(l2->y - l1->y, 2));
}

double location_distance_to_rectangle(const location *loc, const location *lower_left, const location *upper_right)
{
  location closest; // the closest point on the rectangle
  
  if (loc->x < lower_left->x)
    {
      // to the left
      closest.x = lower_left->x;
    }
  else if (loc->x <= upper_right->x)
    {
      // between left and right
      closest.x = loc->x;
    }
  else
    {
      // to the right
      closest.x = upper_right->x;
    }

  if (loc->y < lower_left->y)
    {
      // below
      closest.y = lower_left->y;
    }
  else if (loc->y <= upper_right->y)
    {
      // between top and bottom
      closest.y = loc->y;
    }
  else
    {
      // above
      closest.y = upper_right->y;
    }

  return location_distance(loc, &closest);
}
