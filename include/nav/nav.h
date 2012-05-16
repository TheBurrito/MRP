/*
 * nav.h
 *
 *  Created on: May 14, 2012
 *      Author: derrick
 */

#ifndef NAV_H_
#define NAV_H_

#include "core/types.h"
#include "nav/path.h"

/**
 * The Nav interface signifies a class that can generate a Path from a start
 * point, and a goal point.
 */
class Nav {
public:
  /**
   * Given a start and goal point, generate a path of navigation points that
   * will guide the robot through the environment to the goal.
   * @param start The point to navigate from.
   * @param end The point to navigate to.
   * @return A path of waypoints to follow to get to the end point. Does not
   * include the starting point and includes the end point.
   */
  virtual Path* getPath(const Pos2& start, const Pos2& end) const =0;
};

#endif /* NAV_H_ */
