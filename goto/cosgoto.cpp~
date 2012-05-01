#include "cosgoto.h"

CosGoto::CosGoto(const double& maxVel, const double& maxTurn) {
	_maxv = maxVel;
	_maxt = maxTurn;
}

double CosGoto::calcVel(const double& error) {
}

double CosGoto::calcTurn(const double& error) {
}

double CosGoto::velError(const double& actual, const double& target, const double& turnTarget) {
	double v = cos(turnTarget);
	if (v < 0) {
		v = -sqrt(-v);
	}
	
	if (v < -_maxv) v = -_maxv;
	if (v > _maxv) v = _maxv;
	
	return v - actual;
}
