#ifndef sensormodel_h_
#define sensormodel_h_

#include "mapping/map.h"

class SensorProb;
class SensorRegion;

/**
 * Handles the math for working working with ranger data in a map. Has functions
 * for helping build maps as well as perform localization.
 */
class SensorModel {
public:

  /**
   * Initializes the sensor model
   * @param sensor
   * @param rIter
   * @param prof
   */
  SensorModel(SensorRanger *sensor, SensorRegion *region,
      SensorOddsProfile *prof, const double& obsThresh);

  /**
   * Updates an evidence grid map with the assigned sensor given the map
   * instance and a robot pose.
   *
   * @param map The map instance to update with the current sensor's readings.
   * @param robot The current pose of the robot to update the map from.
   */
  void updateMap(Map& map, const Pose& robot);

  /**
   * Generates a the probability of the robot existing at a specified pose in
   * the specified map. Uses the map to generate scans as if the robot was
   * actually at that pose using the model and then compares the generated
   * scan against the actual scan currently being reported by the underlying
   * SensorRanger instance.
   *
   * @param map The map data to use for generating the scan.
   * @param robot The pose to generate the scan from.
   * @return A double between 0.0 and 1.0 representing the probability that the
   * robot is at the specified pose.
   */
  double localizationProb(Map& map, const Pose& robot);

protected:

  SensorRanger *_sensor;
  SensorRegion *_region;
  SensorOddsProfile *_prof;

  double _obs;
};

/**
 * Generates the odds of obstacles at specified locations.
 */
class SensorOddsProfile {
public:
  /**
   * Returns the odds of an obstacle being present at the specified point.
   *
   * @param pt The location (both in the map and in polar coordinates relative
   * to the sensor) to generate the current odds for based on the current
   * distance returned from the sensor.
   * @param d The distance that the sensor is reporting.
   * @return The odds of an obstacle existing at the indicated location.
   */
  virtual double getOdds(const PosPol& pt, const double& d)=0;
};

/**
 * Generates the region of map points affected by the current robot pose.
 */
class SensorRegion {
public:

  /**
   * Tells the sensor region to generate its list of points from the current
   * sensor reading. Uses the map instance to perform the discretization of
   * points.
   *
   * @param map The map instance that will be updates by these points.
   * @param robot The pose of the robot to generate the region from.
   * @param idx The index of the physical sensor to generate a region for.
   *
   * @return The list of points contained in the current sensor region.
   */
  const PosPolList& generateRegion(SensorRanger *sensor, const Map& map,
      const Pose& robot, size_t idx);

protected:

  /**
   * Does the actual generation of the points covered by the current sensor
   * reading. This function is called from generateRegion() which handles the
   * point discretization, so the specific buildRegion() implementation does not
   * have to worry about doing this itself. All points generated should be
   * placed in the _pts list. This list will be cleared before buildRegion is
   * called.
   *
   * @param sensor The sensor the region is being build for.
   * @param robot The location of the robot to generate the region relative to.
   * @param idx The index of the physical sensor to generate a region for.
   */
  void buildRegion(SensorRegion *sensor, const Pose& robot, size_t idx)=0;

  PosPolList _pts;
};

#endif /* sensormodel_h_ */
