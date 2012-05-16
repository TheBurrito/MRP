#ifndef roadmap_h_
#define roadmap_h_

#include "nav/nav.h"
#include "core/common.h"

#include <map>

typedef std::map<Pos2, Pos2VList> EdgeMap;

/**
 * A roadmap is a set of points connected by edges that denote navigable paths.
 * This Roadmap class is not knowledgeable about creating roadmaps, it simply
 * stores roadmaps and makes them available for generating paths. The Roadmap
 * class should be subclassed in order to generate roadmaps from a map in some
 * manner.
 */
class Roadmap : public Nav {
public:
  /**
   *
   */
  Roadmap();

  /**
   * Initializes the roadmap instance and loads the map in from the specified
   * file.
   *
   * @param filename The filename of the map to load.
   */
  Roadmap(std::string filename);

  /**
   * Loads a roadmap from the specified file.
   *
   * @param filename The file containing the roadmap to load.
   * @return The status of the map loading.
   */
  int loadFromFile(std::string filename);

  /**
   * Saves the current roadmap to a file.
   *
   * @param filename The filename to save the roadmap to.
   * @return The status of saving the map to a file.
   */
  int saveToFile(std::string filename) const;

  /**
   * Inserts a connection between two points in the roadmap. If the points are
   * not referenced in the roadmap yet they are added before adding the edge.
   *
   * It is important to make sure that the points specified have the exact
   * values as points used previously.
   *
   * TODO: Add point coalescing for points with a distance below a specified
   * threshold.
   *
   * @param a The first point on the map to connect.
   * @param b The second point on the map to connect.
   * @return true if the addition linked existing points, false if at least one
   * of the points was previously unknown.
   */
  bool addEdge(const Pos2& a, const Pos2& b);

  virtual Path* getPath(const Pos2& start, const Pos2& end);

private:
  Pos2List points;
  EdgeMap edges;
};

#endif /*roadmap_h_*/
