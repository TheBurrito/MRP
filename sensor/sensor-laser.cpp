#include "sensor/sensor-laser.h"

SensorLaser::SensorLaser(RangerProxy *rp, double fov, int nPts, double min, double max) {
		_fov = fov; //convert to radians
		_nPts = nPts;
		_d0 = (double)_fov / nPts;
		_rp = rp;
		_min = min;
		_max = max;
}

std::vector<Pos2> SensorLaser::getLocalScan(double sAng, double eAng /*, Graphics2dProxy& g2d*/ ) {
	//Make sure sAng is to the left of eAng
	if (sAng < eAng) {
		int t = sAng;
		sAng = eAng;
		eAng = t;
	}
	
	sAng = _fov / 2.0 - sAng;
	eAng = _fov / 2.0 - eAng;
	
	int sI = ceil(sAng * _nPts / _fov);
	int eI = eAng * _nPts / _fov;
	
	double theta = _fov / 2.0 - sI * _d0;
	
	std::vector<Pos2> local;
	Pos2 cur;
	
	if (!getRangeCount()) {
		return local;
	}
	
	int i2;
	
	for (int i = sI; i <= eI && i < _nPts; ++i) {
		i2 = _nPts - i - 1;
		
		if ((*_rp)[i2] < _max && (*_rp)[i2] > _min) {
			cur.x = (*_rp)[i2] * -cos(theta);
			cur.y = (*_rp)[i2] * sin(theta);
		
			local.push_back(cur);
		}
		
		theta += _d0;
	}
	
	return local;
}

unsigned int SensorLaser::getRangeCount() {
	return _rp->GetRangeCount();
}
