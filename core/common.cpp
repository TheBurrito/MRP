#include "core/common.h"

#include <ctime>

Pos2 polar2Pos(const Polar2& pt) {
	Pos2 p;
	
	p.x = pt.d * cos(pt.theta);
	p.y = pt.d * sin(pt.theta);
	
	return p;
}

Polar2 pos2Polar(const Pos2& pt) {
	Polar2 p;
	
	p.d = hypot(pt.x, pt.y);
	p.theta = atan2(pt.y, pt.x);
	
	return p;
}

bool getNextPoint(std::istream& is, double& x, double& y) {
	if (!is.good()) {
		return false;
	};
	is >> x;
	is >> y;
	return true;
}

/*double getSecs() {
	double s;
	
	timespec now;
	clock_gettime(CLOCK_REALTIME, &now);

	s = now.tv_sec + now.tv_nsec / 1000000000.0;
	return s;
}*/
