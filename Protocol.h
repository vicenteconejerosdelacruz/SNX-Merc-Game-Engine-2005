#ifndef MERC_PROTOCOL_H
#define MERC_PROTOCOL_H

#include <SDL/SDL_net.h>

#define MERC_NET_PORT		6996

#define MSG_LOGIN				0x32
#define MSG_GOTO_SERVER			0x33
#define MSG_LOAD_MAP			0x34
#define MSG_ENTITY				0x35
#define MSG_CLIENT_DATA			0x36
#define MSG_CLIENT_STATE		0x5B


struct MsgHeader
{
	unsigned int size;
	unsigned char type;
};

struct MsgLogin
{
	char user[32];
	char pass[32];
};

struct MsgChatPublic
{
	char user[20];
	char message[255];
};

struct MsgClientData
{
    char name[20];
    int id;
    char agency[20];
    char race[20];
    char model[20];
};

struct MsgClientState
{
	int uid;
	int FirstHierarchy;
	int SecondHierarchy;
	int UltraHierarchy;
	float xpos,ypos,zpos;
	float xvel,yvel,zvel;
	float Rotation[2];
	int ping;//(ms)
	int time;
};

struct MsgClientPositionChange
{
	int input;
	int time;
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

/*This are Server-Server Communication Parts, dont use in the client*/
struct MsgNewClient
{
	TCPsocket socket;
};

#endif
