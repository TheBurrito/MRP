#include "goto/simplegoto.h"

#include "core/common.h"

double Vel_Diff::velError(const double& actual, const double& target, const double& turnTarget) {
	return target - actual;
}

double Vel_CosTurn::velError(const double& actual, const double& target, const double& turnTarget) {
	return cos(target - actual);
}

double Turn_Diff::turnError(const double& actual, const double& target) {
	double d0 = target - actual;
	normalizeTheta(d0);
	return d0;
}
