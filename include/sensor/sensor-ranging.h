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
	virtual std::vector<Pos2> getLocalScan() {
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
	virtual Pos2List getLocalScan(double sAng, double eAng)=0;
	
	/**
	 * Used for determining the number of data points associated with a scan.
	 * @return The number of points that were returned by the sensor. In most
	 * cases this should agree with the length of the list returned from
	 * getLocalScan.
	 */
	virtual unsigned int getRangeCount()=0;
	
protected:
	double _min;
	double _max;
};

#endif /* pioneer_ranger_h_ */
