#include "sensor-sonar.h"

static Pose sonar_geom[8] = {
  {{0.075, 0.130}, dtor(90)},
  {{0.115, 0.115}, dtor(50)},
  {{0.150, 0.080}, dtor(30)},
  {{0.170, 0.025}, dtor(10)},
  {{0.170, -0.025}, dtor(-10)},
  {{0.150, -0.080}, dtor(-30)},
  {{0.115, -0.115}, dtor(-50)},
  {{0.075, -0.130}, dtor(-90)}
};

/*static Pose sonar_geom[5] = {
  {{0.11, 0.05}, dtor(40)},
  {{0.13, 0.02}, dtor(20)},
  {{0.15, 0.00}, dtor(0) },
  {{0.13, -0.02}, dtor(-20)},
  {{0.11, -0.05}, dtor(-40)}
};*/

SensorSonar::SensorSonar(SonarProxy *sp, double min, double max) {
	_sp = sp;
	_min = min;
	_max = max;
}

Pose SensorSonar::geom(size_t i) {
  return sonar_geom[i];
}

double SensorSonar::operator[](size_t i) {
  return (*_sp)[i];
}

Pos2List SensorSonar::getLocalScan(double sAng, double eAng) {
	//Make sure sAng is to the left of eAng
	if (sAng < eAng) {
		int t = sAng;
		sAng = eAng;
		eAng = t;
	}
	
	std::vector<Pos2> pts;
	Pos2 cur;
	
	for (int i = 0; i < 8; ++i) {
		
		if (sonar_geom[i].yaw <= sAng && sonar_geom[i].yaw >= eAng) {
			if ((*_sp)[i] < _max && (*_sp)[i] > _min) {
				cur.x = (*_sp)[i] * cos(sonar_geom[i].yaw) + sonar_geom[i].p.x;
				cur.y = (*_sp)[i] * sin(sonar_geom[i].yaw) + sonar_geom[i].p.y;
				
				pts.push_back(cur);
			}
		}
	}
	
	return pts;
}

unsigned int SensorSonar::getRangeCount() {
	return 8;
}
