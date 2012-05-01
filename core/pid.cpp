#include "core/pid.h"

PID::PID(const double& p, const double& i, const double& d) {
	init(p, i, d, 0);
}

PID::PID(const double& p, const double& i, const double& d, const double& smax) {
	init(p, i, d, smax);
}

double PID::calc(const double& error) {
	if (_lt == 0) {
		_lt = getSecs();
		_e = error;
		return 0;
	}
	
	double now = getSecs();
	double ret = calc(error, now - _lt);
	
	_lt = now;
	
	return ret;
}

double PID::calc(const double& error, const double& dt) {
	_sum += error * dt;
	if (_sum > _smax) _sum = _smax;
	if (_sum < -_smax) _sum = -_smax;
	
	double p, i, d;
	
	p = _p * error;
	i = _i * _sum;
	d = _d * (error - _e) / dt;
	
	_e = error;
	
	return p + i + d;
}

void PID::init(const double& p, const double& i, const double& d, const double& smax) {
	_p = p;
	_i = i;
	_d = d;
	_e = 0;
	_sum = 0;
	_smax = smax;
}
