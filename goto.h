#ifndef goto_h_
#define goto_h_

#include <libplayerc++/playerc++.h>
#include "types.h"

using namespace PlayerCc;

class IVelCalc {
public:
	virtual double calcVel(const double& error)=0;
};

class ITurnCalc {
public:
	virtual double calcTurn(const double& error)=0;
};

class IVelError {
public:
	virtual double velError(const double& actual, const double& target, const double& turnTarget)=0;
};

class ITurnError {
public:
	virtual double turnError(const double& actual, const double& target)=0;
};

class Goto {
public:
	Goto(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc);
	
	Goto(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc, IVelError *ve);
	
	Goto(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc, ITurnError *te);
	
	Goto(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc, IVelError *ve, ITurnError *te);
	
	//Move towards a goal point relative to robot's current location
	// - irrespective of current yaw.
	void goLocal(const Pose& robot, const Pos2& local);
	
	void goWorld(const Pose& robot, const Pos2& world);
	
private:

	void init(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc, IVelError *ve, ITurnError *te);
	
	Position2dProxy *_p2d;
	
	IVelCalc *_vc;
	ITurnCalc *_tc;
	
	IVelError *_ve;
	ITurnError *_te;
};

#endif /* goto_h_ */
