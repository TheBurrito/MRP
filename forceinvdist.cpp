#include "apf.h"

ForceInvDist::ForceInvDist(int inv_power) {
	_pow = inv_power;
}

double ForceInvDist::calcForce(const double& dist) {
	double ret = dist;
	for (int i = 1; i < _pow; ++i) {
		ret *= ret;
	}
	
	return 1.0/ret;
}
