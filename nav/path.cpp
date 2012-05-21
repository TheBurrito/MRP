#include "nav/path.h"

#include <fstream>

Path::Path() {
  _points = new std::vector<Pos2>();
  _curPt = 0;
}

int Path::readFromFile(std::string filename) {
	std::ifstream pt_file(filename.c_str());
	int n = 0;
	
	if (pt_file.is_open()) {
		Pos2 curReadPt;
		while (pt_file.good()) {
			pt_file >> curReadPt.x;
			
			if (!pt_file.good()) {
				break;
			}
			
			pt_file >> curReadPt.y;
			
			//std::cout << "Read point (" << curReadPt.x << ", " << curReadPt.y << ")" << std::endl;
			
			addPoint(curReadPt);
			++n;
		}
	
		pt_file.close();
	}
	
	return n;
}

Pos2List * Path::getPoints() {
  return _points;
}

int Path::saveToFile(std::string filename) {
  std::ofstream out(filename.c_str());

  int n = 0;
  Pos2List::iterator iter;

  if (out.is_open()) {
    for (iter = _points->begin(); iter != _points->end(); ++iter) {
      out << (*iter).x << " " << (*iter).y << std::endl;
      ++n;
    }
  }

  out.close();
  return n;
}

bool Path::atEnd() {
	return _curPt == _points->size();
}

size_t Path::numPts() {
  return _points->size();
}

size_t Path::curIndex() {
  return _curPt;
}

bool Path::getCurrentPt(Pos2& pt) {
	if (_curPt == _points->size()) {
	  return true;
	}

	pt = (*_points)[_curPt];
	return false;
}

bool Path::update(const Pose& robot, const double& epsilon) {
	if (atEnd()) {
		return false;
	}
	
	double dx = robot.p.x - (*_points)[_curPt].x;
	double dy = robot.p.y - (*_points)[_curPt].y;
	double d = hypot(dx, dy);
	
	if (d < epsilon) {
		++_curPt;
		return true;
	}
	
	return false;
}

void Path::addPoint(const Pos2& p) {
	_points->push_back(p);
}

void Path::clear() {
	_points->clear();
	_curPt = 0;
}

