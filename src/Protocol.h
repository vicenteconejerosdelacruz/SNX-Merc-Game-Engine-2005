#ifndef MERC_PROTOCOL_H
#define MERC_PROTOCOL_H

#include <SDL/SDL_net.h>

#define MERC_NET_PORT		6996

#define MSG_LOGIN				0x32
#define MSG_GOTO_SERVER			0x33
#define MSG_LOAD_MAP			0x34
#define MSG_ENTITY				0x35
#define MSG_CLIENT_DATA			0x36
#define MSG_CLIENT_DISCONNECT	0x37
#define MSG_JOIN_GAME			0x38
#define MSG_WAITING				0x39
#define MSG_CLIENT_NEWPOS		0x5A
#define MSG_CLIENT_STATE		0x5B
#define MSG_CLIENT_ROT			0x5C
#define MSG_BODY_DATA			0x97
#define MSG_BODY_STATE			0x98
#define MSG_PING				0x6E

struct MsgLogin
{
	char user[32];
	char pass[32];
	char version[5];
};

struct MsgMap
{
	char mapname[32];
};

struct MsgChatPublic
{
	char user[20];
	char message[255];
};

struct MsgClientData
{
    char name[32];
    int id;
    char agency[32];
    char race[32];
    char model[32];
};

struct MsgClientState
{
	int id;
	int FirstHierarchy;
	int SecondHierarchy;
	int UltraHierarchy;
	float Position[3];
	float Velocity[3];
	float Rotation[2];
	int FeetState;
	int ping;//(ms)
};

struct MsgClientPositionChange
{
	int input;
};

struct MsgClientRotation
{
	float Rotation[2];
};

struct MsgWeaponInfo
{
	int ID;
	int cartridges;
	int bullets;
};

struct MsgCurrentWeapon
{
	int ID;
};

struct MsgBodyData
{
	int BodyID;
	char modelname[32];
};

struct MsgBodyState
{
	int BodyID;
	float Position[3];
	float Velocity[3];
	float Q[4];
	float AngVel[3];
};
#endif
