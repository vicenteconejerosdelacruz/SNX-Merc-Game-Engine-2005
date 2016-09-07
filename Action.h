#ifndef MERC_ACTION_H
#define MERC_ACTION_H

/*Game States*/
#define GAME_QUIT			0x01
#define SHOW_CONSOLE		0x02

/*FSM States*/
//First Hierarchy States
#define S1_DEATH			0x00
#define S1_LIVE				0x01

//Second Hierarchy States
#define S2_STAND			0x01
#define S2_WALK				0x02
#define S2_RUN				0x04
#define S2_CROUCH			0x08
#define S2_FALL				0x10

//Ultra Hierarchy States
#define SU_SHOOT_1			0x01
#define SU_JUMP				0x02
#define SU_GET_UP			0x04

/*FSM Actions*/
#define	A_STANDING			0x01
#define A_CROUCH			0x02
#define A_JUMP				0x04
#define A_STANCE			0x08
#define A_RUNNING			0x10
#define	A_MOVING_FRONT		0x20
#define A_MOVING_BACK		0x40
#define A_STRAFE_RIGHT		0x80
#define A_STRAFE_LEFT		0x100
#define A_SHOOT_1			0x200
#define	A_SHOOT_2			0x400
#define A_SILENT_WALK		0x800
#define A_NEXT_WEAPON		0x1000
#define A_PREV_WEAPON		0x2000
#define A_SALUTE			0x4000

#endif