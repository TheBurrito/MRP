#ifndef Sensor_sonar_h_
#define Sensor_sonar_h_

#include "sensor/sensor-ranging.h"

extern Pose Pioneer8Sonar[8];

/**
 * Represents a sonar sensor using a player/stage proxy.
 */
class SensorSonar: public SensorRanger {
public:

  SensorSonar(SonarProxy *sp, Pose *geom, size_t numSonar, double min,
      double max);

  /**
   * Returns a list of detected points in robot-local space, used primarily for
   * algorithms such as APF and obstacle avoidance.
   * @param sAng The starting angle to retrieve points from. In radians.
   * @param eAng The ending angle to retrieve points to. In radians.
   * @return A list of points detected that lie between the specified angles in
   * robot-local space.
   */
  virtual Pos2List getLocalScan(double sAng, double eAng);

  /**
   * Used for determining the number of data points associated with a scan.
   * Note that for this implementation, 8 is always returned irregardless of the
   * data returned.
   *
   * @return The number of points that were returned by the sensor. In most
   * cases this should agree with the length of the list returned from
   * getLocalScan.
   */
  virtual size_t getRangeCount();

  virtual size_t getNumSensors();

  virtual const Pose& getGeom(size_t idx);

  /**
   * Allows access to a single sonar's range from the sonar bank.
   * @param i The index of the sonar.
   * @return The range reading from the sonar in meters.
   */
  virtual double operator[](size_t i);

  virtual double getMin();
  virtual double getMax();

private:

  SonarProxy *_sp;
  size_t _num;
  double _min, _max;

  Pose* _geom;
};

#endif /* Sensor_sonar_h_ */
