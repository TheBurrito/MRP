#include "nav/roadmap.h"

#include <fstream>
#include <set>
#include <map>
#include <cstdio>
#include <algorithm>
#include <queue>

struct SearchNode {
  Pos2 p;
  SearchNode* parent;
  double g;
  double h;
  inline double f() const {
    return g + h;
  }
};

class SNComp {
public:
  bool operator()(const SearchNode *a, const SearchNode *b) {
    return a->f() > b->f();
  }
};

typedef std::priority_queue<SearchNode *, std::vector<SearchNode *>, SNComp> FrontierList;

Roadmap::Roadmap() {

}

Roadmap::Roadmap(std::string filename) {
  loadFromFile(filename);
}

int Roadmap::loadFromFile(std::string filename) {
  std::ifstream in(filename.c_str());

  if (!in.is_open()) {
    return 1;
  }

  std::string aName, bName;
  std::map<std::string, Pos2> pNames;

  size_t np, ne;
  Pos2 a, b;

  points.clear();
  edges.clear();

  in >> np >> ne;

  //std::cout << np << " " << ne << std::endl;

  for (size_t i = 0; i < np; ++i) {
    in >> std::ws >> aName >> a.x >> a.y;
    //std::cout << aName << " " << a.x << " " << a.y << std::endl;
    points.push_back(a);
    pNames[aName] = a;
  }

  for (size_t i = 0; i < ne; ++i) {
    in >> aName >> bName;
    //std::cout << aName << " " << bName << std::endl;

    a = pNames[aName];
    b = pNames[bName];

    //std::cout << "\t(" << a.x << "," << a.y << ")<->(" << b.x << "," << b.y << ")" << std::endl;
    if (!addEdge(a, b)) {
      in.close();
      return 1;
    }
  }

  in.close();
  return 0;
}

int Roadmap::saveToFile(std::string filename) const {
  std::ofstream out(filename.c_str());
  size_t id = 0;
  char name[10];
  std::string bName;

  std::map<Pos2, std::string> pNames;
  EdgeMap::const_iterator eIter; //edge Iterator
  Pos2VList::iterator nIter; //neighbor iterator

  out << points.size() << " " << edges.size() << std::endl;
  for (size_t i = 0; i < points.size(); ++i) {
    sprintf(name, "%d", id++);
    pNames[points[i]] = std::string(name);
    out << name << points[i].x << " " << points[i].y << std::endl;
  }

  for (eIter = edges.begin(); eIter != edges.end(); ++eIter) {
    Pos2VList neighbors = eIter->second;
    strcpy(name, pNames[eIter->first].c_str());

    for (nIter = neighbors.begin(); nIter != neighbors.end(); ++nIter) {
      bName = pNames[(*nIter).p];
      out << name << " " << bName.c_str() << std::endl;
    }
  }

  out.close();
  return 0;
}

bool Roadmap::addEdge(const Pos2& a, const Pos2& b) {
  Pos2V ab, ba;
  ab.p = b;
  ba.p = a;

  ab.v = dist(a.x - b.x, a.y - b.y);
  ba.v = ab.v;

  //TODO: Check if points/edges already exist

  edges[a].push_back(ab);
  edges[b].push_back(ba);

  return true;
}

Path * Roadmap::getPath(const Pos2& start, const Pos2& end) {
  //The path that's being built
  Path *path = new Path();

  //The roadmap points that are closest to the start and end points.
  Pos2 sClosest, eClosest;

  //The list of points connected to the closest start and closest end points.
  Pos2VList sNeighbors, eNeighbors;

  //The point currently being generated along a roadmap edge.
  Pos2V curPt;

  //The intermediate points chosen along segments as the entrance and exit to
  //the roadmap.
  Pos2V startPt, endPt;
  startPt.v = -1;
  endPt.v = -1;

  std::cout << "Generating path from (" << start.x << ", " << start.y
      << ") to (" << end.x << ", " << end.y << ")" << std::endl;

  sClosest = *(closestPoint(points, start));
  eClosest = *(closestPoint(points, end));

  std::cout << "\tClosest to start on Roadmap: (" << sClosest.x << ", "
      << sClosest.y << ")" << std::endl;

  std::cout << "\tClosest to end on Roadmap: (" << eClosest.x << ", "
      << eClosest.y << ")" << std::endl;

  //We can now path find from sClosest to eClosest and just add the extra points

  SearchNode *curNode = new SearchNode(), *parent;

  Pos2List visited;
  FrontierList frontier;

  Pos2VList neighbors;

  std::deque<Pos2> tmpPath;

  curNode->parent = 0;
  curNode->g = 0;
  curNode->h = pointDist(sClosest, end);
  curNode->p = sClosest;

  frontier.push(curNode);

  std::vector<SearchNode *> allNodes;

  allNodes.push_back(curNode);

  while (!frontier.empty()) {
    curNode = frontier.top();
    frontier.pop();

    std::cout << "\n\tPopped (" << curNode->p.x << ", " << curNode->p.y
        << ") off the frontier." << std::endl;

    //Skip nodes that have already been visited.
    if (std::find(visited.begin(), visited.end(), curNode->p)
        != visited.end()) {
      continue;
    }

    //Check if goal
    if (curNode->p == eClosest) {
      std::cout << "\tPoint is goal." << std::endl;
      break;
    }

    //Get current neighbors and store parent node for backtrack referencing.
    neighbors = edges[curNode->p];
    parent = curNode;

    //Create the search nodes for each neighbor and add to frontier
    for (Pos2VList::iterator i = neighbors.begin(); i != neighbors.end(); ++i) {
      std::cout << "\tGenerating new search node" << std::endl;
      curNode = new SearchNode();
      curNode->parent = parent;

      std::cout << "\t\tPoint (" << i->p.x << ", " << i->p.y
          << ")" << std::endl;

      std::cout << "\t\tSetting parent to (" << parent->p.x << ", "
          << parent->p.y << ")" << std::endl;

      curNode->p = i->p;
      curNode->g = parent->g + i->v;

      std::cout << "\t\tG(n) = " << curNode->g << std::endl;

      curNode->h = pointDist(curNode->p, end);

      std::cout << "\t\tH(n) = " << curNode->h << std::endl;

      frontier.push(curNode);
    }
  }

  std::cout << "Finished search." << std::endl;

  if (!curNode) {
    std::cout << "Exausted search space" << std::endl;
    return 0;
  }

  //Trace backwards from curNode
  while (curNode) {
    tmpPath.push_back(curNode->p);
    curNode = curNode->parent;
  }

  //No longer need the heap searchnodes, delete them all.
  for (size_t i = 0; i < allNodes.size(); ++i) {
    delete allNodes[i];
  }

  std::cout << "Finished tracing path from goal." << std::endl;

  //tmpPath.push_back(end);

  //Now dump the path into the Path instance.
  for (std::deque<Pos2>::iterator i = tmpPath.begin(); i != tmpPath.end();
      ++i) {
    path->addPoint(*i);
  }

  path->addPoint(end);

  return path;
}
