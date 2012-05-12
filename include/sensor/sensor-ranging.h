#ifndef pioneer_ranger_h_
#define pioneer_ranger_h_

#include "core/common.h"

/**
 * Abstract class used to wrap various ranging sensor sources in.
 */
class SensorRanger {
public:

  /**
   * Convenience method for returnining points from the front hemi-sphere of the
   * robot. See getLocalScan(double, double).
   *
   * @return A list of points detected from the front hemi-sphere of the robot.
   */
	inline Pos2List getLocalScan() {
		return getLocalScan(dtor(90), dtor(-90));
	}
	
	/**
   * Returns a list of detected points in robot-local space, used primarily for
   * algorithms such as APF and obstacle avoidance.
   * @param sAng The starting angle to retrieve points from. In radians.
   * @param eAng The ending angle to retrieve points to. In radians.
   * @return A list of points detected that lie between the specified angles in
   * robot-local space.
   */
	Pos2List getLocalScan(double sAng, double eAng);
	
	/**
	 * Used for determining the number of data points associated with a scan.
	 * @return The number of points that were returned by the sensor. In most
	 * cases this should agree with the length of the list returned from
	 * getLocalScan.
	 */
	virtual size_t getRangeCount()=0;
	
	/**
   * Returns the pose of a specific sensor relative to the robot.
   * @param i The index of the sensor.
   * @return The pose (position and angle) of the sensor.
   */
	virtual Pose getGeom(size_t i)=0;

	virtual size_t getNumSensors()=0;

	virtual double operator[](size_t i)=0;

	virtual double getMin()=0;

	virtual double getMax()=0;
};

#endif /* pioneer_ranger_h_ */
