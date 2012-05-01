#ifndef DynamicGrid_h_
#define dynamicgrid_h_

#include <map>
#include <set>
#include <cmath>

#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif

namespace std
{
 using namespace __gnu_cxx;
}

#include "core/types.h"

#define xInd(x) (floor(x / _rWidth))
#define yInd(y) (floor(y / _rHeight))

template<class T> class DynamicGrid;
template<class T> class GridRegion;

typedef std::pair<int, int> IntPair;
typedef std::set<IntPair> IntPairSet;

template<class T>
class GridRegion {
public:
  GridRegion(DynamicGrid<T> *dmap, const double& left, const double& top) {
    _map = dmap;
    _left = left;
    _top = top;

    _vals = (T*) malloc(_map->_rLen * sizeof(T));
    for (size_t i = 0; i < _map->_rLen; ++i) {
      _vals[i] = _map->_def;
    }
  }

  inline size_t indexOf(const double& x, const double& y) {
    /**
     * Cells are indexed in X major order, with 0 being the top-left.
     */
    size_t xi = (x - _left) / _map->_res;
    size_t yi = (_top - y) / _map->_res;

    return (xi * _map->_rRows + yi);
  }

  inline double& get(const double& x, const double& y) {
    return _vals[indexOf(x, y)];
  }

  inline double* getp(const double& x, const double& y) {
    return &_vals[indexOf(x, y)];
  }

  inline double& operator[](size_t i) {
    return _vals[i];
  }

private:
  /**
   * The DynamicGrid that this region belongs to.
   */
  DynamicGrid<T> *_map;

  /**
   * The location of the upper-left corner of the region in the world. Only the
   * top-left is needed as the size can be retrieved from the DynamicGrid
   * instance.
   */
  double _left, _top;

  /**
   * The actual values representing the region.
   */
  T *_vals;
};

/**
 * A grid that is capable of dynamically updating its size as needed.
 */
template<class T>
class DynamicGrid {
public:
  /**
   * Make GridRegion a friend of DynamicGrid so the regions can make use of the
   * private instance variables in this class for layout and indexing.
   */
  friend class GridRegion<T>;

  /**
   * Creates a dynamic map with the specified grid cell resolution, initial
   * size, and default value. Note that the initial size specified is the size
   * of each GridRegion allocated internally. As such, the initial size should be
   * neither so small as to cause overly frequent allocations nor too big so as
   * to be inefficient with memory usage (if an issue).
   *
   * Initially, there will be 4 regions created, once in each quadrant and each
   * having a corner at the "world" origin.
   *
   * @param res The resolution of a grid cell in meters. IE for a grid cell that
   * is 5 cm to a side, specify 0.05 for the argument.
   * @param regWidth The width of a region in meters.
   * @param regHeight The height of a region in meters.
   * @param defVal The default value to use for the odds of each cell. In most
   * cases this should just be 1.
   */
  DynamicGrid(const double& res, const double& regWidth,
      const double& regHeight, const T& defVal) {
    _res = res;

    //The resolution will remain as specified, but we'll fudge the requested
    //width and height to make sure everything is evenly divisible.
    _rCols = ceil(regWidth / res);
    _rRows = ceil(regHeight / res);
    _rWidth = _rCols * res;
    _rHeight = _rRows * res;

    _rLen = _rCols * _rRows;

    _def = defVal;

    _left = -_rWidth;
    _right = _rWidth;
    _top = _rHeight;
    _bottom = -_rHeight;

    double w2 = _rWidth / 2;
    double h2 = _rHeight / 2;

    //Determine which region to allocate by specifying a point within the region.
    allocateRegion(w2, h2); //Quadrant 1
    allocateRegion(-w2, h2); //Quadrant 2
    allocateRegion(-w2, -h2); //Quadrant 3
    allocateRegion(w2, -h2); //Quadrant 4

    doRegionAllocations();
  }

  /**
   * Gives access to the underlying grid value specified by a location.
   * @param x The x coordinate contained by the grid to return.
   * @param y The y coordinate contained by the grid to return.
   * @return A pointer to the double value if it exists, null if the region
   * containing the specified point has not been allocated yet.
   */
  inline T* get(const double& x, const double& y) {
    GridRegion<T> *mr = _reg(x, y);
    return mr ? &(mr->get(x, y)) : 0;
  }

  inline double width() {
    return _right - _left;
  }

  inline double height() {
    return _top - _bottom;
  }

  inline double left() {
    return _left;
  }

  inline double right() {
    return _right;
  }

  inline double top() {
    return _top;
  }

  inline double bottom() {
    return _bottom;
  }

  inline double gridRes() {
    return _res;
  }

  /**
   * Requests to have the map allocate the map region that contains the
   * specified point. Note that this is not done immediately, but is marked for
   * later allocation once doAllocations() is called. Also, this function does
   * not ensure that the newly requested region is connected to existed regions.
   *
   * @param x The x coordinate of the point within the region to allocate.
   * @param y The y coordinate of the point within the region to allocate.
   */
  void allocateRegion(const double& x, const double& y) {
    if (isRegionAllocated(x, y)) {
      return;
    }

    IntPair loc(xInd(x), yInd(y));

    //std::cout << "Marking (" << loc.first << ", " << loc.second
    //    << ") for allocation." << std::endl;

    _marked.insert(loc);
  }

  /**
   * Checks if a region in the environment is allocated already or not. This
   * function still returns false (0) for a region if it has been flagged for
   * allocation, but has not actually been allocated yet. To see if a region has
   * been marked, see isRegionMarked().
   *
   * @param x The x coordinate of the point within a region to check.
   * @param y The y coordinate of the point within a region to check.
   * @return 0 if the region containing the specified point has not been
   * allocated yet, 1 if it has.
   */
  inline int isRegionAllocated(const double& x, const double& y) {
    return (_reg(x, y) != 0);
  }

  /**
   * Checks if a region is currently waiting to be allocated. A region will be
   * waiting for allocation when either:
   *  A) a value update has been requested for a point within the region, or
   *  B) a call to allocateRegion has been made for a point within the region.
   *
   * @param x The x coordinate of a point within the region to check.
   * @param y The y coordinate of a point within the region to check.
   * @return 0 if the region is either already allocated or not marked for
   * allocation, 1 if the region is marked for allocation.
   */
  inline int isRegionMarked(const double& x, const double& y) {
    IntPair loc(xInd(x), yInd(y));
    return _marked.find(loc) != _marked.end();
  }

  /**
   * Reports whether or not there are regions waiting to be allocated.
   *
   * @return 1 if at least 1 reagion is waiting to be allocated, 0 otherwise.
   */
  inline int needsAllocation() {
    return _marked.size() > 0;
  }

  /**
   * Forces the map to allocate all of the marked regions and subsequently
   * perform any updates within those regions, if any are stored. Depending on
   * how long since the last allocation call this function can take a "long"
   * time to run. If lots of updates from sensors are made at once it may be a
   * good idea to realize when updates are being made close to an unallocated
   * region so that the region can be allocated before updates to it are made.
   */
  inline void doRegionAllocations() {

    //Iterate through all the MapRegion indices to be created.
    for (IntPairSet::iterator iter = _marked.begin(); iter != _marked.end();
        ++iter) {
      double newLeft = iter->first * _rWidth;
      double newTop = (iter->second + 1) * _rHeight;

      if (newLeft < _left) {
        _left = newLeft;
      }

      if (newTop > _top) {
        _top = newTop;
      }

      if (newLeft + _rWidth > _right) {
        _right = newLeft + _rWidth;
      }

      if (newTop - _rHeight < _bottom) {
        _bottom = newTop - _rHeight;
      }

      _regions[iter->first][iter->second] = new GridRegion<T>(this, newLeft, newTop);
    }

    //Created all the regions, so clear them now.
    _marked.clear();
  }

  inline GridRegion<T> * regionAt(const double& x, const double& y) {
    return _reg(x, y);
  }

private:

  /**
   * Retrieves the GridRegion containing the specified point.
   *
   * @param x The x coordinate of the point within the region to return.
   * @param y The y coordinate of the point within the region to return.
   *
   * @return A pointer to the GridRegion containing the point if it is allocated,
   * or null (0) if it is not allocated.
   */
  GridRegion<T> * _reg(const double& x, const double& y) {
    int xi = xInd(x);

    //std::cout << "Looking for region in col " << xi << std::endl;

    typename std::hash_map<int, std::hash_map<int, GridRegion<T> *> >::iterator col = _regions.find(xi);
    if (col == _regions.end()) {
      //std::cout << "Failed to find region at col " << xi << std::endl;
      return 0;
    }

    int yi = yInd(y);

    //std::cout << "Looking for region in row " << yi << std::endl;

    typename std::hash_map<int, GridRegion<T> *>::iterator reg = col->second.find(yi);

    if (reg == col->second.end()) {
      //std::cout << "Failed to find region at row " << yi << std::endl;
      return 0;
    }

    //std::cout << "Found region at (" << xi << ", " << yi << ")" << std::endl;

    return reg->second;
  }

  /**
   * STL Maps are used to represent a potentially sparse 2-D "array" of
   * GridRegions. The key of the outer map is the x index of the space while the
   * inner map's key is the y index.
   */
  std::hash_map<int, std::hash_map<int, GridRegion<T> *> > _regions;

  /**
   * Stores regions that are marked for allocation.
   */
  IntPairSet _marked;

  /**
   * The default value of grid cells within the map.
   */
  double _def;

  /**
   * The resolution of grid cells, in meters.
   */
  double _res;

  /**
   * The size, in cells, of a single GridRegion.
   */
  size_t _rCols, _rRows;

  /**
   * GridRegions are implemented as flat arrays, this is the precomputed length.
   */
  size_t _rLen;

  /**
   * The size, in meters, of a single GridRegion.
   */
  double _rWidth, _rHeight;

  /**
   * Stores the current bounds of the environment. Note that this is determined
   * from the furthest left, top, right, and bottom regions allocated and does
   * not take into account if there are any empty regions.
   */
  double _left, _top, _right, _bottom;
};

#endif /* dynamicgrid_h_ */
