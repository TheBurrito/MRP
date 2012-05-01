#ifndef pid_h_
#define pid_h_

#include "core/common.h"

class PID {
public:
	PID(const double& p, const double& i, const double& d);
	
	PID(const double& p, const double& i, const double& d, const double& smax);
	
	double calc(const double& error);
	
	double calc(const double& error, const double& dt);
	
private:
	void init(const double& p, const double& i, const double& d, const double& smax);
	
	double _p; 		/* Proportional term */
	double _i;		/* Integral term */
	double _d;		/* Derivative term */
	double _e;		/* Previous error for d calculation */
	double _sum;	/* Running integral sum */
	double _smax;	/* Cap on integral sum */
	double _lt;		/* Last time calc() was called */
};

#endif /* pid_h_ */
