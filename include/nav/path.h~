#ifndef path_h_
#define path_h_

#include "common.h"

class Path {
public:
	Path(const double& epsilon);
	
	int readFromFile(const char* filename);
	
	int atEnd();
	
	Pos2 getCurrentPt();
	
	int update(const Pose& robot);
	
	void addPoint(const Pos2& p);
	
	void clear();
	
private:
	void init(const double& epsilon);
	
	Pos2List *_points;
	double _eps;
	int _curPt;
};

#endif /* path_h_ */
