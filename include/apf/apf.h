#ifndef apf_h_
#define apf_h_

#include "core/types.h"

class IForceCalc {
public:
	virtual double calcForce(const double& dist)=0;
};

class ForceInvDist : public IForceCalc {
public:
	ForceInvDist(int inv_power);
	
	virtual double calcForce(const double& dist);

private:
	double _pow;
};


class APF {
public:
	APF(const double& Kgoal, const double& Kobs);
	
	APF(const double& Kgoal, const double& Kobs, IForceCalc* forceCalc);
	
	Pos2 calc(const Pose& robot, const Pos2& goal, const Pos2List& points);
	
private:
	void init(const double& Kgoal, const double& Kobs, IForceCalc *forceCalc);
	
	IForceCalc *_forceCalc;
	double _kg, _ko;
};

#endif /* apf_h_ */
