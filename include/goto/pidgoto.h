#ifndef pidgoto_h_
#define pidgoto_h_

#include "core/pid.h"
#include "goto/goto.h"

class PIDGoto : public IVelCalc, public ITurnCalc {
public:
	PIDGoto(PID *vel, PID *turn);
	
	virtual double calcVel(const double& error);
	
	virtual double calcTurn(const double& error);

private:
	PID *_vel;
	PID *_turn;
}

#endif /* pidgoto_h_ */
