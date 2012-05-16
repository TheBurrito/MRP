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

double DynamicMap::get(const double& x, const double& y) const {
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
    _grd->allocateRegion(x, y);
    return;
  }
  //std::cout << *v << std::endl;
  updateGrid(v, odds);
}

void DynamicMap::discretizePoints(PosPolList& pts) const {
  /*std::map<double *, PosPol *> vals;
  std::map<double *, PosPol *>::iterator iter;

  bool closer, medial;

  PosPolList::iterator ptIter;

  std::map<PosPol *, bool> rem;

  double *p;
  size_t n;

  n = pts.size();

  for (size_t i = 0; i < n; ++i) {
    p = _grd->get(pts[i].pos.x, pts[i].pos.y);

    //std::cout << "(" << pts[i].pos.x << ", " << pts[i].pos.y << ") -> " << p << std::endl;

    if (p) {
      iter = vals.find(p);
      if (iter != vals.end() ) {
        //These conditions are applied to the existing values
        //IE Is the existing value closer / more medial?

        closer = iter->second->pol.d < pts[i].pol.d
        if (iter->second->pol.d < pts[i].pol.d) {
          std::cout << "Found and reset older" << std::endl;
          rem[iter->second] = true;
          vals[p] = &pts[i];
          rem[&pts[i]] = false;
        } else {
          std::cout << "Found and keep older" << std::endl;
          rem[&pts[i]] = true;
        }

        std::cout << "\t" << iter->second->pol.d << " | " << pts[i].pol.d;
        std::cout << ", " << iter->second->pol.theta << " | " << pts[i].pol.theta << std::endl;
      } else {
        std::cout << "Not found" << std::endl;
        std::cout << "\t" << pts[i].pol.d << " | " << pts[i].pol.theta << std::endl;
        vals[p] = &pts[i];
        rem[&pts[i]] = false;
      }
    } else {
      //std::cout << "\tMarking grid for allocation." << std::endl;
      _grd->allocateRegion(pts[i].pos.x, pts[i].pos.y);
    }
  }

  //std::cout << std::endl;

  for (ptIter = pts.begin(); ptIter != pts.end(); ++ptIter) {
    if (rem[&(*ptIter)]) {
      //std::cout << "Removing (" << ptIter->pos.x << ", " << ptIter->pos.y << ")" << std::endl;
      ptIter = pts.erase(ptIter) - 1;
    }
  }*/
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

double DynamicMap::envWidth() const {
  return _grd->width();
}

double DynamicMap::envHeight() const {
  return _grd->height();
}
double DynamicMap::envLeft() const {
  return _grd->left();
}

double DynamicMap::envRight() const {
  return _grd->right();
}

double DynamicMap::envTop() const {
  return _grd->top();
}

double DynamicMap::envBottom() const {
  return _grd->bottom();
}

double DynamicMap::gridRes() const {
  return _grd->gridRes();
}

int DynamicMap::needsAllocation() const {
  return _grd->needsAllocation();
}

void DynamicMap::doRegionAllocations() {
  _grd->doRegionAllocations();
}
