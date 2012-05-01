/*
 * dynamicmap.cpp
 *
 *  Created on: Apr 28, 2012
 *      Author: derrick
 */

#include "dynamicmap.h"

DynamicMap::DynamicMap(const double& res, const double& regWidth,
    const double& regHeight, const double& defVal) {
  _grd = new DynamicGrid<double>(res, regWidth, regHeight, defVal);
  _def = defVal;

  //std::cout << "Finished initializing map instance." << std::endl;
}

double DynamicMap::get(const double& x, const double& y) {
  //std::cout << "Getting map value at (" << x << ", " << y << ")" << std::endl;

  double *v = _grd->get(x, y);
  return (v ? *v : _def);
}

inline void updateGrid(double *p, const double& v) {
  *p *= v;

  if (*p < .001) {
    *p = .001;
  } else if (*p > 999) {
    *p = 999;
  }
}

void DynamicMap::update(const double& x, const double& y, const double& odds) {
  double *v = _grd->get(x, y);

  updateGrid(v, odds);
}

void DynamicMap::update(const Pos2VList& pts) {
  //The set to add updates to to make sure we only keep the most relevant
  //update per grid location
  std::map<double *, double> vals;
  double *p;

  //Enforce one update per grid, keeping the largest value per grid found.
  for (size_t i = 0; i < pts.size(); ++i) {
    p = _grd->get(pts[i].p.x, pts[i].p.y);

    if (p) {
      if (vals[p] < pts[i].v) {
        vals[p] = pts[i].v;
      }
    } else {
      _grd->allocateRegion(pts[i].p.x, pts[i].p.y);
    }
  }

  //Now apply the updates.
  std::map<double *, double>::iterator iter;
  for (iter = vals.begin(); iter != vals.end(); ++iter) {
    updateGrid(iter->first, iter->second);
  }
}

double DynamicMap::envWidth() {
  return _grd->width();
}

double DynamicMap::envHeight() {
  return _grd->height();
}
double DynamicMap::envLeft() {
  return _grd->left();
}

double DynamicMap::envRight() {
  return _grd->right();
}

double DynamicMap::envTop() {
  return _grd->top();
}

double DynamicMap::envBottom() {
  return _grd->bottom();
}

double DynamicMap::gridRes() {
  return _grd->gridRes();
}

int DynamicMap::needsAllocation() {
  return _grd->needsAllocation();
}

void DynamicMap::doRegionAllocations() {
  _grd->doRegionAllocations();
}
