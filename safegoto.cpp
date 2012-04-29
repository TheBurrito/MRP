#include "common.h"

#include "path.h"

#include "goto.h"

#include "pioneer-sonar.h"
#include "pioneer-laser.h"

#include "apf.h"
#include "mindistcalc.h"

#include <string>
#include <cstring>

#define EPS_D		0.15

#define EPS_0		0.2

#define N_SONAR		8

#define N_LASER		512

#define Kg		1.0
#define Kl		0.001
#define Ks		0.4

#define APF_MIN_D 	0.7
#define APF_FACT	4

#define MAX_V	0.4

#define REL_TO_START 0
#define WAIT_TO_START 1

#define f_apf	.1  /* 10Hz rate for apf - want to give it time to run all the input points */
#define f_goto	.01 /* 100Hz rate for driving PID */
#define f_path	.2  /* 5Hz rate for Path control */

using namespace std;

static bool bRunning = true;

enum SensorType {
	Sonar,
	Laser
};

void usage(char* argv0) {
	cerr << "Usage: " << argv0 << " sonar|laser points-file [host port]" << endl;
}

int main(int argc, char** argv) {
	string host = "localhost";
	int port = 6665;
	SensorType st;
	
	if (argc == 5) {
		host = argv[3];
		port = atoi(argv[4]);
	} else if (argc != 3) {
		cerr << "Invalid number of arguments." << endl;
		usage(argv[0]);
		return 1;
	}
	
	cout << "Sensor type: " << argv[1] << endl;
	if (!strcmp(argv[1], "sonar")) {
		st = Sonar;
	} else if (!strcmp(argv[1], "laser")) {
		st = Laser;
	} else {
		cerr << "Unknown sensor type specified." << endl;
		usage(argv[0]);
		return 1;
	}
	
	Path path(EPS_D);
	cout << "Read " << path.readFromFile(argv[2]) << " points from file." << endl;
	
	cout << "Creating robot client..." << endl;
	PlayerClient robot(host, port);
	Position2dProxy ps_pos(&robot, 0);
	
	//We have now checked all the params and can now start! Yay!
	Pose r_pos;
	
	double Kgoal = Kg;
	double Kobs;
	
	//This will be our universal interface that handles sonars and lasers the same.
	PioneerRanger *pr;
	
	cout << "Creating ranger device...";
	switch (st) {
		case Laser:
			pr = new PioneerLaser(new RangerProxy(&robot, 0), dtor(240), 681, 0.0, 2.0);
			Kobs = Kl;
			cout << " Laser device created." << endl;
			break;
			
		case Sonar:
			pr = new PioneerSonar(new SonarProxy(&robot, 0), 0.0, 2.0);
			Kobs = Ks;
			cout << " Sonar device created." << endl;
			break;
	}
	
	cout << "Initializing APF" << endl;
	MinDistCalc apf_calc(APF_MIN_D, APF_FACT);
	APF apf(Kgoal, Kobs, &apf_calc);
	
	robot.Read();
	#if WAIT_TO_START==1
		cout << "\nPress enter to start..." << endl;
		cin.get();
	#endif
	
	cout << "Enabling motors" << endl;
	ps_pos.SetMotorEnable(true);
	
	cout << "Beginning main loop..." << endl;
	
	while (bRunning) {
		
		//usleep(50000);
		//cout << "Reading robot" << endl;
		robot.Read();
		
		//cout << "Reading current pose" << endl;
		r_pos.p.x = ps_pos.GetXPos();
		r_pos.p.y = ps_pos.GetYPos();
		r_pos.yaw = ps_pos.GetYaw();
		
		normalizeTheta(r_pos.yaw);
		
		//cout << "Pose: (" << r_pos.p.x << ", " << r_pos.p.y << ", " << r_pos.yaw << ")" << endl;
		//cout << "Goal: (" << path.points[path.curPt].x << ", " << path.points[path.curPt].y << ")" << endl;
		
		if (pr->getRangeCount()) {
		
			//cout << "Running apf" << endl;
			Pos2 r = apf.calc(r_pos, path.getCurrentPt(), pr->getLocalScan(dtor(90), dtor(-90)));
			
			double v = hypot(r.x, r.y);
			double t = atan2(r.y, r.x);
			
			//cout << "P_APF: (" << v << ", " << t << ")" << endl;
			
			normalizeTheta(t);
			double ct = cos(t);
			v *= ct;
			
			if (v < 0) v = -sqrt(-v);
			if (v > MAX_V) v = MAX_V;
			//if (v < -MAX_V / 2) v = -MAX_V / 2;
			
			ps_pos.SetSpeed(v, t / PI_2);
			
			if (path.update(r_pos)) {
				cout << "Reached path point." << endl;
			}
			
			if (path.atEnd()) {
				cout << "Last point reached" << endl;
				bRunning = false;
			}
		}
	}
}
