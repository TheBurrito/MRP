#ifndef pioneer_laser_h_
#define pioneer_laser_h_

#include "sensor/sensor-ranging.h"

#define LASER_MIN	0.0
#define LASER_MAX	5.0

class SensorLaser: public SensorRanger {
public:
	SensorLaser(RangerProxy *rp, double fov, int nPts, double min, double max);
		
	virtual std::vector<Pos2> getLocalScan(double sAng, double eAng);
	
	virtual unsigned int getRangeCount();
	
private:
	
	RangerProxy *_rp;
	double _fov;
	int _nPts;
	double _d0;
};

#endif /* pioneer_laser_h_ */
