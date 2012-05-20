#ifndef path_h_
#define path_h_

#include "core/common.h"

class Path {
public:
	Path();
	
	/**
	 * Reads a list of points in from a file.
	 * @param filename The file to load the points from.
	 * @return The number of points read from the file.
	 */
	int readFromFile(std::string filename);

	/**
	 * Saves the whole path to a file.
	 * @param filename The file to save the path to.
	 * @return The number of points that have been saved.
	 */
	int saveToFile(std::string filename);
	
	/**
	 * Determines whether or not the path has been completed.
	 * @return true if the last point has been reached, false otherwise.
	 */
	bool atEnd();
	
	/**
	 *
	 * @return The current way point along the path.
	 */
	Pos2 getCurrentPt();
	
	/**
	 * Updates the progress of the path based on the current pose of the robot.
	 *
	 * @param robot The current location of the robot
	 * @return true if the path was update, false if the current goal has not been
	 * reached.
	 */
	bool update(const Pose& robot, const double& epsilon);
	
	void addPoint(const Pos2& p);
	
	void clear();
	
	Pos2List * getPoints();

private:
	
	Pos2List *_points;
	int _curPt;
};

#endif /* path_h_ */
