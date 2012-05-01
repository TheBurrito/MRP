#include "nav/path.h"

#include <fstream>

Path::Path(const double& epsilon) {
	init(epsilon);
}

int Path::readFromFile(const char* filename) {
	std::ifstream pt_file(filename, std::ifstream::in);
	int n = 0;
	
	if (pt_file.is_open()) {
		Pos2 curReadPt;
		while (pt_file.good()) {
			pt_file >> curReadPt.x;
			
			if (!pt_file.good()) {
				break;
			}
			
			pt_file >> curReadPt.y;
			
			std::cout << "Read point (" << curReadPt.x << ", " << curReadPt.y << ")" << std::endl;
			
			addPoint(curReadPt);
			++n;
		}
	
		pt_file.close();
	}
	
	return n;
}

int Path::atEnd() {
	return _curPt == _points->size();
}

Pos2 Path::getCurrentPt() {
	return (*_points)[_curPt];
}

int Path::update(const Pose& robot) {
	if (atEnd()) {
		return 0;
	}
	
	double dx = robot.p.x - (*_points)[_curPt].x;
	double dy = robot.p.y - (*_points)[_curPt].y;
	double d = hypot(dx, dy);
	
	if (d < _eps) {
		++_curPt;
		return 1;
	}
	
	return 0;
}

void Path::addPoint(const Pos2& p) {
	_points->push_back(p);
}

void Path::clear() {
	_points->clear();
	_curPt = 0;
}

void Path::init(const double& epsilon) {
	_eps = epsilon;
	_points = new std::vector<Pos2>();
	_curPt = 0;
}

