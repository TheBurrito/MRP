/*
 * dynamicmap.cpp
 *
 *  Created on: Apr 28, 2012
 *      Author: derrick
 */

#include "mapping/dynamicmap.h"

//#include <iostream>

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
  //std::cout << "update: " << x << " " << y << " " << odds << std::endl;
  double *v = _grd->get(x, y);

  if (!v) {
    //std::cout << "Grid location not allocated yet. Aborting update." << std::endl;
    return;
  }
  //std::cout << *v << std::endl;
  updateGrid(v, odds);
}

void DynamicMap::discretizePoints(PosPolList& pts) const {
  std::map<double *, PosPol *> vals;
  std::map<double *, PosPol *>::iterator iter;

  std::map<PosPol *, bool> rem;

  PosPolList::iterator ptIter;

  double *p;
  size_t n;

  n = pts.size();

  for (size_t i = 0; i < n; ++i) {
    p = _grd->get(pts[i].pos.x, pts[i].pos.y);
    if (p) {
      iter = vals.find(p);
      if (iter != vals.end() && iter->second->pol.d < pts[i].pol.d) {
        rem[iter->second] = true;
      }

      rem[&pts[i]] = false;

    } else {
      _grd->allocateRegion(pts[i].pos.x, pts[i].pos.y);
    }
  }

  for (size_t i = 0; i < pts.size(); ++i) {
    if (rem[&pts[i]]) {
      pts.erase(pts.begin() + i);
      --i;
    }
  }
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
