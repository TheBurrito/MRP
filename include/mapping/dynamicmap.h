/*
 * dynamicmap.h
 *
 *  Created on: Apr 28, 2012
 *      Author: derrick
 */

#ifndef DYNAMICMAP_H_
#define DYNAMICMAP_H_

#include "core/dynamicgrid.h"
#include "mapping/map.h"

class DynamicMap: public Map {
public:
  /**
   * Creates a dynamic map with the specified grid cell resolution, initial
   * size, and default value. Note that the initial size specified is the size
   * of each GridRegion allocated internally. As such, the initial size should be
   * neither so small as to cause overly frequent allocations nor too big so as
   * to be inefficient with memory usage (if an issue).
   *
   * @param res The resolution of a grid cell in meters. IE for a grid cell that
   * is 5 cm to a side, specify 0.05 for the argument.
   * @param regWidth The width of a region in meters.
   * @param regHeight The height of a region in meters.
   * @param defVal The default value to use for the odds of each cell. In most
   * cases this should just be 1.
   */
  DynamicMap(const double& res, const double& regWidth, const double& regHeight,
      const double& defVal);

  virtual double get(const double& x, const double& y) const;

  virtual void set(const double& x, const double& y, const double& odds);

  virtual void update(const double& x, const double& y, const double& odds);

  virtual void update(const Pos2VList& pts);

  virtual double envWidth() const;

  virtual double envHeight() const;

  virtual double envLeft() const;

  virtual double envRight() const;

  virtual double envTop() const;

  virtual double envBottom() const;

  virtual double gridRes() const;

  virtual void discretizePoints(PosPolList& pts) const;

  int needsAllocation() const;

  void doRegionAllocations();

  static DynamicMap* loadMap(std::string filename,
      const double& regWidth, const double& regHeight);

private:

  void _updateStep2(const double& x, const double& y, const double& odds,
      int addMissed);

  DynamicGrid<double> *_grd;

  /**
   * Stores points that are waiting for an region to be allocated.
   */
  Pos2VList _updates;

  /**
   * The default value of grid cells within the map.
   */
  double _def;
};

#endif /* DYNAMICMAP_H_ */
