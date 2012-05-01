#include "goto.h"

//Some default error calcs that will be pretty simple
class Vel_Diff : public IVelError {
public:
	virtual double velError(const double& actual, const double& target, const double& turnTarget);
};

class Vel_CosTurn : public IVelError {
public:
	virtual double velError(const double& actual, const double& target, const double& turnTarget);
};

class Turn_Diff : public ITurnError {
public:
	virtual double turnError(const double& actual, const double& target);
};
