#ifndef mindistcalc_h_
#define mindistcalc_h_

#include "apf.h"

class MinDistCalc : public IForceCalc {
public:
	MinDistCalc(const double& dist, const double& fact);
	
	virtual double calcForce(const double& dist);
	
private:
	double _dist;
	double _fact;
};

#endif /* mindistcalc_h_ */
