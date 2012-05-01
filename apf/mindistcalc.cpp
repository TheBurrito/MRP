#include "apf/mindistcalc.h"

#include <cmath>

MinDistCalc::MinDistCalc(const double& dist, const double& fact) {
	_dist = dist;
	_fact = fact;
}

double MinDistCalc::calcForce(const double& dist) {
	return pow(_fact, (_dist - dist) * _fact);
}
