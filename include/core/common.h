#ifndef common_h_
#define common_h_

#include <iostream>
#include <cmath>

#include <libplayerc++/playerc++.h>

#include "core/types.h"

#define PI_2 1.570796325
#define PI   3.14159265
#define PI2  6.2831853

#define dist(dx, dy) hypot(dx, dy)

using namespace PlayerCc;

Pos2 polar2Pos(const Polar2& pt);

Polar2 pos2Polar(const Pos2& pt);

void normalizeTheta(double& theta);

bool getNextPoint(std::istream& is, double& x, double& y);

Pos2 rotate(const Pos2& pos, const double& theta);

double getSecs();

inline Pos2 rotate(const Pos2& p, const double& theta) {
  double c = cos(theta);
  double s = sin(theta);
  Pos2 n;

  n.x = p.x * c - p.y * s;
  n.y = p.x * s + p.y * c;
  
  return n;
}

#endif /* common_h_ */
