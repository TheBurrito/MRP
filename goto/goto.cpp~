#include "goto/goto.h"

//Include the "default" calculation classes
#include "goto/simplegoto.h"

Goto::Goto(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc) {
	init(p2d, vc, tc, 0, 0);
}

Goto::Goto(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc, IVelError *ve) {
	init(p2d, vc, tc, ve, 0);
}

Goto::Goto(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc, ITurnError *te) {
	init(p2d, vc, tc, 0, te);
}

Goto::Goto(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc, IVelError *ve, ITurnError *te) {
	init(p2d, vc, tc, ve, te);
}

//Move towards a goal point relative to robot's current location
// - irrespective of current yaw.
void Goto::goLocal(const Pose& robot, const Pos2& local) {
}

void Goto::goWorld(const Pose& robot, const Pos2& world) {
	Pos2 local;
	double d = hypot(world.x - robot.p.x, world.y - robot.p.y);
	
	goLocal(robot, local);
}

void Goto::init(Position2dProxy *p2d, IVelCalc *vc, ITurnCalc *tc, IVelError *ve, ITurnError *te) {
	_p2d = p2d;
	
	if (ve == 0) {
		_ve = new Vel_Diff();
	} else {
		_ve = ve;
	}
	
	if (te == 0) {
		te = new Turn_Diff();
	} else {
		_te = te;
	}
	
	_vc = vc;
	_tc = tc;
}
