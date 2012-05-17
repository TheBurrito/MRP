/*
 * dynamicmap.cpp
 *
 *  Created on: Apr 28, 2012
 *      Author: derrick
 */

#include "mapping/dynamicmap.h"

#include <fstream>

//#include <iostream>

DynamicMap::DynamicMap(const double& res, const double& regWidth,
    const double& regHeight, const double& defVal) {
  _grd = new DynamicGrid<double>(res, regWidth, regHeight, defVal);
  _def = defVal;

  //std::cout << "Finished initializing map instance." << std::endl;
}

DynamicMap * DynamicMap::loadMap(std::string filename, const double& regWidth,
    const double& regHeight) {
  std::string pngfile;
  std::string mapfile = "maps/" + filename;
  double res, left, top;

  std::ifstream in(mapfile.c_str(), std::ifstream::in);

  in >> pngfile;
  in >> res;
  in >> left;
  in >> top;

  in.close();

  DynamicMap *map = new DynamicMap(res, regWidth, regHeight, 1.0);
  map->loadMapPng("maps/" + pngfile, left, top);
  return map;
}

double DynamicMap::get(const double& x, const double& y) const {
  //std::cout << "Getting map value at (" << x << ", " << y << ")" << std::endl;

  double *v = _grd->get(x, y);
  return (v ? *v : _def);
}

void DynamicMap::set(const double& x, const double& y, const double& odds) {
  double *v = _grd->get(x, y);

  //If the region doesn't exist yet (v is null) then allocate the region so that
  //the set is not missed.
  if (!v) {
    _grd->allocateRegion(x, y);
    _grd->doRegionAllocations();
    v = _grd->get(x, y);
  }

  *v = odds;
}

inline void updateGrid(double *p, const double& v) {
  //Skip the update if the cell is capped.
  if (*p == 0.0 || *p == MAX_ODDS) {
    return;
  }

  *p *= v;

  if (*p < .001 && *p > 0.0) {
    *p = .001;
  } else if (*p > 999 && *p < MAX_ODDS) {
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

/*int DynamicMap::loadMap(std::string filename) {
 return 1;
 }

 int DynamicMap::saveMap(std::string filename) {
 return 1;
 }*/
