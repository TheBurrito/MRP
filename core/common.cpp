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

void normalizeTheta(double& theta) {
	while (theta > PI) theta -= PI2;
	while (theta < -PI) theta += PI2;
}

bool getNextPoint(std::istream& is, double& x, double& y) {
	if (!is.good()) {
		return false;
	};
	is >> x;
	is >> y;
	return true;
}

Pos2 rotate(const Pos2& pos, const double& theta) {
  Pos2 rot;
  double c = cos(theta);
  double s = sin(theta);

  rot.x = pos.x * c - pos.y * s;
  rot.y = pos.x * s + pos.y * c;

  return rot;
}

double getSecs() {
	double s;
	
	timespec now;
	clock_gettime(CLOCK_REALTIME, &now);

	s = now.tv_sec + now.tv_nsec / 1000000000.0;
	return s;
}
