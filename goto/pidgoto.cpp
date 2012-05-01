#include "goto/pidgoto.h"

PIDGoto::PIDGoto(PID *vel, PID *turn) {
	_vel = vel;
	_turn = turn;
}

virtual double PIDGoto::calcVel(const double& error) {
	return _vel.calc(error);
}

virtual double PIDGoto::calcTurn(const double& error) {
	return _turn.calc(error);
}
