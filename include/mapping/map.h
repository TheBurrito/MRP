#ifndef map_h_
#define map_h_

#include "core/types.h"
#include "core/common.h"

inline double OtoP(const double& odd) { return (odd / (odd + 1)); }
inline double PtoO(const double& prob) { return (prob / (1 - prob)); }

/**
 * Defines the interface for a map representation.
 */
class Map {
public:

  /**
   * Provides the functionality to retrieve a single point's odds of an
   * obstacle.
   * @param x The x coordinate of the position in the world to retrieve.
   * @param y The y coordinate of the position in the world to retrieve.
   * @return The odds of an obstacle at that location in the world.
   */
  virtual double get(const double& x, const double& y)=0;

  /**
   * Updates a single point's odds of an obstacle with new evidence. Note that
   * this version of the function does not prune repeated updates to a single
   * grid location. If several points are being updated from a single sensor it
   * would be better to use the functions below that take a Pos2VList as an
   * argument.
   *
   * @param x The x coordinate of the position in the world to update.
   * @param y The y coordinate of the position in the world to update.
   * @param odds The new odds of an obstacle to update with.
   */
  virtual void update(const double& x, const double& y, const double& odds)=0;

  /**
   * Performs odds updates with a list of corresponding points in the world and
   * the new odds value to update with. Note that this function does not make
   * multiple updates to the same location. This function keeps track of the
   * max odds seen for a grid cell and uses that as the update value.
   *
   * @param pts The list of points and corresponding odds values to update with.
   */
  virtual void update(const Pos2VList& pts)=0;

  /**
   * Reports the width of the environment stored in the map in units of meters.
   * @return The width of the current map in meters.
   */
  virtual double envWidth()=0;

  /**
   * Reports the height of the environment stored in the map in units of meters.
   * @return The height of the current map in meters.
   */
  virtual double envHeight()=0;

  /**
   * @return The x coordinate on the left edge of the environment.
   */
  virtual double envLeft()=0;

  /**
   * @return The x coordinate on the right edge of the environment.
   */
  virtual double envRight()=0;

  /**
   * @return The y coordinate on the top edge of the environment.
   */
  virtual double envTop()=0;

  /**
   * @return The y coordinate on the bottom edge of the environment.
   */
  virtual double envBottom()=0;

  /**
   * Reports the resolution of a grid cell.
   * @return The length of a side of a grid, in meters.
   */
  virtual double gridRes()=0;
};

#endif /* map_h_ */
