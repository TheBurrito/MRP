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
  virtual double get(const double& x, const double& y) const =0;

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
   * Removes points from the list that fit within the same grid cell of the map.
   *
   * @param pts The list of points to discretize into map space.
   */
  virtual void discretizePoints(PosPolList& pts) const =0;

  inline void quantize(double& x, double& y) const;

  /**
   * Reports the width of the environment stored in the map in units of meters.
   * @return The width of the current map in meters.
   */
  virtual double envWidth() const =0;

  /**
   * Reports the height of the environment stored in the map in units of meters.
   * @return The height of the current map in meters.
   */
  virtual double envHeight() const =0;

  /**
   * @return The x coordinate on the left edge of the environment.
   */
  virtual double envLeft() const =0;

  /**
   * @return The x coordinate on the right edge of the environment.
   */
  virtual double envRight() const =0;

  /**
   * @return The y coordinate on the top edge of the environment.
   */
  virtual double envTop() const =0;

  /**
   * @return The y coordinate on the bottom edge of the environment.
   */
  virtual double envBottom() const =0;

  /**
   * Reports the resolution of a grid cell.
   * @return The length of a side of a grid, in meters.
   */
  virtual double gridRes() const =0;
};

void Map::quantize(double& x, double& y) const {
  double r = gridRes();
  x = r * (floor(x / r) + 0.5);
  y = r * (floor(y / r) + 0.5);
}

#endif /* map_h_ */
