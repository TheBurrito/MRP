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

inline double frand(const double& fMin, const double& range)
{
    double f = rand() / (RAND_MAX + 1.0);
    return fMin + f * range;
}

/**
 * Given two points that make up a line and a third point, p, determine the
 * closest point on that line to p, optionally clamping the point within the
 * segment created by a -> b.
 *
 * @param a The first point making up the line.
 * @param b The second point making up the line.
 * @param p The point to find the closest point on the line to.
 * @param clamp true to limit the point to the segment created by ab.
 * @return The point on the line closest to p, or one of the endpoints if clamp
 * is true and the closest point lies outside the line segment ab.
 */
inline Pos2 closestOnLine(const Pos2& a, const Pos2& b, const Pos2& p, bool clamp) {
  Pos2 pa = p - a;
  Pos2 ab = b - a;
  double ab2 = ab.x*ab.x + ab.y*ab.y;
  double ap_ab = pa.x*ab.x + pa.y*ab.y;
  double t = ap_ab / ab2;

  if (clamp) {
    if (t > 1.0) return b;
    else if (t < 0.0) return a;
  }

  Pos2 c;
  c.x = a.x + t * ab.x;
  c.y = a.y + t * ab.y;
  return c;
}

inline Pos2 closestOnLine(const LineSeg& l, const Pos2& p, bool clamp) {
  return closestOnLine(l.a, l.b, p, clamp);
}

inline double pointDist(const Pos2& a, const Pos2& b) {
  return hypot(a.x - b.x, a.y - b.y);
}

inline const Pos2* closestPoint(const Pos2List& pts, const Pos2& pt) {
  double minD = -1, d;
  const Pos2* closest;

  for (Pos2List::const_iterator i = pts.begin(); i != pts.end(); ++i) {
    d = pointDist((*i), pt);
    if (d < minD || minD < 0) {
      minD = d;
      closest = &(*i);
    }
  }

  return closest;
}

#endif /* common_h_ */
