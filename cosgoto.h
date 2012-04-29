#ifndef cosgoto_h_
#define cosgoto_h_

#include "goto.h"

class CosGoto : public IVelCalc, public ITurnCalc, public IVelError {
public:
	CosGoto(const double& maxVel, const double& maxTurn);
	
	virtual double calcVel(const double& error);
	
	virtual double calcTurn(const double& error);
	
	virtual double velError(const double& actual, const double& target, const double& turnTarget);

private:
	double _maxv, _maxt;
};

#endif /* cosgoto_h_ */
