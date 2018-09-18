#ifndef MERC_PREDICTION_H
#define MERC_PREDICTION_H

#include "Vector3.h"
#include "InputHandler.h"

typedef struct State{
	Vector3 Position;
	Vector3 Velocity;
}State;

typedef struct ClientMovement{
	Input input;
	State state;
	Uint32 time;
	int FeetState;
}ClientMovement;

#endif