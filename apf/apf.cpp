#include "apf/apf.h"

#include <cmath>

APF::APF(const double& Kgoal, const double& Kobs) {
	init(Kgoal, Kobs, 0);
}

APF::APF(const double& Kgoal, const double& Kobs, IForceCalc* forceCalc) {
	init(Kgoal, Kobs, forceCalc);
}

Pos2 APF::calc(const Pose& robot, const Pos2& goal, const Pos2List& points) {
	//The vector sum of obstacles, relative offset to goal, and return vector
	Pos2 sum, dg, res;
	
	//Current distance, current obstacle vector factor
	double dist, fact, theta;
	
	//Initialize the vector sum of the obstacles and perform the summation.
	sum.x = 0;
	sum.y = 0;
	for (int i = 0; i < points.size(); ++i) {
		dist = hypot(points[i].x, points[i].y);
		
		//Use the assigned calculation for the vector factor
		fact = _forceCalc->calcForce(dist);
		
		//Add the normalized vector multiplied by the factor returned above.
		sum.x += fact * (points[i].x / dist);
		sum.y += fact * (points[i].y / dist);
	}
	
	//Apply obstacle gain
	sum.x *= _ko;
	sum.y *= _ko;
	
	//Find the offset to the goal relative to the robot
	dg.x = goal.x - robot.p.x;
	dg.y = goal.y - robot.p.y;
	
	//Distance to the goal
	theta = atan2(dg.y, dg.x) - robot.yaw;
	
	//Perform the result vector calculation
	res.x = _kg * cos(theta) - sum.x;
	res.y = _kg * sin(theta) - sum.y;
	
	return res;
}

void APF::init(const double& Kgoal, const double& Kobs, IForceCalc *forceCalc) {
	if (forceCalc == 0) {
		//Didn't specify a force calculation class, use default of inv square.
		_forceCalc = new ForceInvDist(2);
	} else {
		_forceCalc = forceCalc;
	}
	
	_kg = Kgoal;
	_ko = Kobs;
}
