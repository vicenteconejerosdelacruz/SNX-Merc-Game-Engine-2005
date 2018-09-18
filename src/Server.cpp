#include "Merc.h"
#include "Server.h"
#include "ConfigFile.h"
#include "Log.h"
#include "Vector3.h"
#include "Action.h"
#include "AudioManager.h"

extern ConfigFile Config;

Server::Server()
{
	now=0;
	last=0;
	accum=0;
	Running=false;
	bShutDown=false;
	SListWorlds=NULL;
	OffsetPool=0;
	memset(PlayerName,0,sizeof(PlayerName));
}

Server::~Server()
{

}

int Server::GetInitQueue()
{
	return INIT_NETWORK|INIT_PHYSICS|INIT_TIMER;
}

void Server::Init(int Value)
{
	Services=GetInitQueue();
	UsingAcounts=Config.getBoolean("acounts");

	SDL_Init(0);	

	if((Services&INIT_NETWORK)==INIT_NETWORK)
		Net_Server.Init();

#ifdef MERC_DB
	if((Services&INIT_DB)==INIT_DB)
		InitWorldsUsingDB();
	else
#endif
	InitWorldsUsingLua();

	
	if(Value==(GAME_CLIENT|GAME_SERVER))
	{
		IHandler.Init();
		SDisplay.Init();
		ServerType=Value;//the player is the server	
		strcat(PlayerName,Config.getString("user"));

		SPManager.Add(
			PlayerName,//name
			NULL,//IPaddress
			0,//ID
			"Humans",//model
			TEST_MAP);

		AudioManager *manager = AudioManager::getInstance();
		manager->setMapName(TEST_MAP);
		manager->InitAudio();
		manager->loadPlayLists();
		manager->play("fondo.ogg",MERC_STATIC);
		manager->play("fire.ogg",MERC_STATIC);
	
		ServerPlayer *SPptr;
		World_Server *WSptr;

		SPptr=SPManager.GetPlayer(PlayerName);
		WSptr=GetWorld(SPptr->planetname);
		WSptr->World_Client::Init(SPptr->planetname);
		WSptr->PhManager.AddPlayer(SPptr->id);
		SPptr->ping=0;

		SDisplay.AddObject(WSptr);
		int NumStructures=WSptr->GetNumStructures();

		for(int i=0;i<NumStructures;i++)
			SDisplay.AddObject(WSptr->GetStructureByNum(i));
		
		SDisplay.AddObject(WSptr->GetSkyDome());
		//SDisplay.AddObject(SPptr);
		SPptr->enabled=true;

		for(int i=0;i<2;i++)
		{
			for(int j=0;j<1;j++)
			{
				//Position[0]=800;
				//Position[1]=1000;
				//Position[2]=300;
				//float LaPos[]={500.0f,170.0f+i*8.0f,350.0f+j*8.0f};
				float LaPos[]={840.0f,470.0f+i*8.0f,320.0f+j*8.0f};
				float LaRot[]={0.0f,0.0f,0.0f};
				WSptr->PhManager.AddBody("dummy",LaPos,LaRot);
			}
		}

		int numbodies=WSptr->PhManager.GetNumBodies();
		for(int i=0;i<numbodies;i++)
		{
			char modelname[32];
			int BodyID;
			WSptr->PhManager.GetBodyData(i,&BodyID,modelname);
			DManager.AddObject(BodyID,modelname);
			SDisplay.AddObject(DManager.GetObjectByNum(i));
		}

		int numstatics=WSptr->PhManager.GetNumObjects();
		for(int i=0;i<numstatics;i++)
		{
			SDisplay.AddObject(WSptr->PhManager.GetObjectByNum(i));
		}
	}
	else if(Value==GAME_SERVER)
	{
		ServerType=Value;//is a dedicated server
	}
	else//Not Should Be Here
	{
		Log::Output("invalid value passed to Server::Init\n");
		exit(EXIT_FAILURE);
	}

	CreateTimers();

}

#ifdef MERC_DB

void Server::InitWorldsUsingDB()
{
	DB.Init(Config.getString("hostname"),
			Config.getString("user"),
			Config.getString("password"));

	DB.Query("SELECT name FROM planets ORDER BY ID");

	NumWorlds=DB.GetNTuples();
    Log::Output("Num planets %d\n",NumWorlds);

	for(int i = 0 ; i < NumWorlds ; i++){
		Log::Output("Planet number %d = %s Loading...\n",
					i,DB.GetValue(i,0));

		AddWorld(DB.GetValue(i,0));
		
	}

	DB.CloseQuery();
}
#endif

void Server::InitWorldsUsingLua()
{
	ConfigFile CFMap("Configs/maps.cfg");

	NumWorlds=CFMap.getInteger("num_maps");
	Log::Output("Num planets %d\n",NumWorlds);
	for(int i = 0 ; i < NumWorlds ; i++){
		Log::Output("Planet number %d = %s Loading...\n",i,CFMap.getString("map%d.name",i));
		AddWorld(CFMap.getString("map%d.name",i));
	}
}

void Server::AddWorld(char *name)
{
	World_Server *WSPtr;
	WSPtr=SListWorlds;

	if(WSPtr==NULL)//No Worlds Loaded
	{
		SListWorlds=new World_Server;
		SListWorlds->next=NULL;
		SListWorlds->prev=NULL;
		WSPtr=SListWorlds;
	}
	else
	{
		while(WSPtr->next!=NULL)
			WSPtr=WSPtr->next;

		World_Server *NewWorld;

		NewWorld=new World_Server;

		WSPtr->next=NewWorld;
		WSPtr->next->prev=WSPtr;
		WSPtr->next->next=NULL;
		WSPtr=WSPtr->next;
	}

	Log::Output("el mundo se llama %s\n",name);
	WSPtr->Init(name,ServerType);
}

void Server::CreateTimers()
{
	if(SDL_WasInit(SDL_INIT_TIMER)==0)
		SDL_InitSubSystem(SDL_INIT_TIMER);

	SDL_AddTimer(1000,SendPing,this);
	SDL_AddTimer(50,SendPlayersStates,this);
	SDL_AddTimer(150,SendBodyStates,this);
}

void Server::AdvanceFrame()
{
	now=SDL_GetTicks();
	delta=now-last;
	last=now;
	accum+=delta;

	GetIncomingData();
	CheckZombies();
	//HandleConnections();
	
	World_Server *WSptr;

	//Physic Step Size
	while(accum>=(int)(PHYSIC_STEP_SIZE/1*1000)){
		WSptr=SListWorlds;
		for(int i=0;i<NumWorlds;i++)
		{
			WSptr->PhManager.AdvanceFrame();
			WSptr=WSptr->next;
		}
		
		accum-=(int)(PHYSIC_STEP_SIZE/1*1000);
	}

	//update client states
	int nplayers=SPManager.GetNumPlayers();
	
	for(int i=0;i<nplayers;i++)
	{
		ServerPlayer *SPptr;
		SPptr=(ServerPlayer *)SPManager.GetPlayerByNum(i);

		if(SPptr==NULL)//check
			continue;
		if(SPptr->enabled==false)//check
			continue;
		
		WSptr=GetWorld(SPptr->planetname);
		
		if(WSptr==NULL)//check
			continue;

		Vector3 Position;
		Vector3 Velocity;
		float Rotation[2];
		int FeetState;
		WSptr->PhManager.GetPlayerState(SPptr->id,Position,Velocity,Rotation,&FeetState);
		SPManager.SetPlayerState(SPptr->id,Position,Velocity,Rotation,FeetState);
	}

	if((ServerType&GAME_CLIENT)==GAME_CLIENT)
	{
		Input input;
		input=IHandler.CheckInput();

		if((input.GameState&GAME_QUIT)==GAME_QUIT)
			bShutDown=true;

		ServerPlayer *SPptr;
		SPptr=(ServerPlayer *)SPManager.GetPlayer(PlayerName);

		if(input.Rotation[0]!=0.0f||input.Rotation[1]!=0.0f)
			SPManager.PlayerAddRotation(SPptr->id,input.Rotation);

		if(SPptr->Rotation[0]	>=	1.0)
			SPptr->Rotation[0]	=	1.0;
		if(SPptr->Rotation[0]	<=	-1.0)
			SPptr->Rotation[0]	=	-1.0;

		int action=input.KeyState;
		if((action&A_STANDING))
		{
			SPptr->SecondHierarchy&=(~S2_WALK);
			SPptr->SecondHierarchy|=S2_STAND;
			SPptr->SecondHierarchy&=(~S2_RUN);
		}

		if(	((action & A_MOVING_FRONT)==A_MOVING_FRONT)||
			((action & A_MOVING_BACK )==A_MOVING_BACK )||
			((action & A_STRAFE_RIGHT)==A_STRAFE_RIGHT)||
			((action & A_STRAFE_LEFT )==A_STRAFE_LEFT )){

			SPptr->SecondHierarchy&=(~S2_WALK);
			SPptr->SecondHierarchy&=(~S2_STAND);
			SPptr->SecondHierarchy|=S2_RUN;

			if((action&A_SILENT_WALK)==A_SILENT_WALK)
			{
				SPptr->SecondHierarchy|=S2_WALK;
				SPptr->SecondHierarchy&=(~S2_RUN);
			}
		}

		WSptr->PhManager.UpdatePlayer(SPptr->id,SPptr->SecondHierarchy,SPptr->UltraHierarchy,action);
		WSptr->PhManager.UpdatePlayerRotation(SPptr->id,SPptr->Rotation);

		int num_bodies=WSptr->PhManager.GetNumBodies();
		for(int i=0;i<num_bodies;i++)
		{
			WSptr->PhManager.GetBodyByNum(i);
			int BodyID;
			Vector3 Position;
			Vector3 Velocity;
			Vector3 AngVel;
			Quaternion Quat;

			WSptr->PhManager.GetBodyState(i,
				&BodyID,
				Position,
				Velocity,
				AngVel,
				Quat);

			DManager.UpdateObject(BodyID,
				Position,
				Velocity,
				AngVel,
				Quat);

		}

		DManager.AdvanceFrame();

		WSptr->Sky.Place(SPptr->Position);
		SDisplay.PlaceCamera(SPptr);
		SDisplay.Render();
	}
}

World_Server *Server::GetWorld(char *name)
{
	World_Server *WSptr;

	WSptr=SListWorlds;
	if(WSptr->next==NULL)
	{
		if(strcmp(WSptr->GetName(),name)==0)
			return WSptr;
	}
	else
	{
		while(WSptr->next!=NULL)
		{
			if(strcmp(WSptr->GetName(),name)==0)
				return WSptr;
	
			WSptr=WSptr->next;
		}
	}

	return NULL;
}

void Server::GetIncomingData()
{
	IPaddress ip;
	char Data[1024];
	int recv;
	ServerPlayer * SPptr;

	recv=Net_Server.Recv(&ip,Data);

	if(Data[0]!=0){
		//Clon's-Stuff
		//CheckConectionList(ip);//someday :D
		switch(Data[0])
		{
		case MSG_LOGIN:
			MsgLogin *Login;
			Login=(MsgLogin *)(Data+1);
			if(strncmp(Login->version,GAME_VERSION,strlen(GAME_VERSION))!=0)
				break;

			//if(CheckUser(Login->user,Login->pass)==false)
				//break;

			AddToConnectionPool(Login->user,Login->pass,ip);
		break;
		case MSG_JOIN_GAME:
			if(!IsInConnectionPool(ip))
				return;//faking packets

			PoolElement *PlayerData;

			PlayerData=GetPendingPlayer(ip);
			
			if(PlayerData!=NULL)
			{
				AddClient(PlayerData->name,PlayerData->pass,PlayerData->ip);
				DeleteFromConnectionPool(ip);
			}
		break;
		case MSG_PING:
			SPptr=SPManager.GetPlayer(ip);
			if(SPptr->PingFree==false)
			{
				SPptr->ping=(SDL_GetTicks()-SPptr->PingTime)/2;
				SPptr->PingFree=true;
			}
			//else//fake packet
		break;
		case MSG_CLIENT_NEWPOS:
			MsgClientPositionChange *MCHPos;

			SPptr=SPManager.GetPlayer(ip);

			if(SPptr->FirstHierarchy!=S1_LIVE)
				return;

			MCHPos=(MsgClientPositionChange *)(Data+1);
			UpdateClientState(SPptr->id,MCHPos);
		break;
		case MSG_CLIENT_ROT:
			MsgClientRotation *MCRot;

			SPptr=SPManager.GetPlayer(ip);

			if(SPptr->FirstHierarchy!=S1_LIVE)
				return;

			MCRot=(MsgClientRotation *)(Data+1);
			UpdateClientRotation(SPptr->id,MCRot);

		break;
		}

	}
}

bool Server::IsInConnectionPool(IPaddress ip)
{
	for(int i=0;i<OffsetPool;i++)
		if(	ConnectionPool[i].ip.host==ip.host&&
			ConnectionPool[i].ip.port==ip.port)
			return true;

	return false;
}

PoolElement *Server::GetPendingPlayer(IPaddress ip)
{
	for(int i=0;i<OffsetPool;i++)
		if(	ConnectionPool[i].ip.host==ip.host&&
			ConnectionPool[i].ip.port==ip.port)
			return &ConnectionPool[i];

	return NULL;
}

void Server::AddToConnectionPool(char *name, char *pass, IPaddress ip)
{
	strcat(ConnectionPool[OffsetPool].name,name);
	strcat(ConnectionPool[OffsetPool].pass,pass);
	ConnectionPool[OffsetPool].ip=ip;
	ConnectionPool[OffsetPool].time=SDL_GetTicks();

	Net_Server.Send(ConnectionPool[OffsetPool].ip,MSG_LOAD_MAP,TEST_MAP);
	for(int i=0;i<SPManager.GetNumPlayers();i++)
	{
		ServerPlayer *SPptr;
		SPptr=SPManager.GetPlayerByNum(i);
		Net_Server.Send(ConnectionPool[OffsetPool].ip,
			MSG_CLIENT_DATA,
			SPptr->id,
			SPptr->name,
			SPptr->agency,
			SPptr->race,
			SPptr->modelname);
	}

	Net_Server.Send(ConnectionPool[OffsetPool].ip,
		MSG_WAITING);
	OffsetPool++;
}

void Server::DeleteFromConnectionPool(IPaddress ip)
{
	for(int i=0;i<OffsetPool;i++)
		if(	ConnectionPool[i].ip.host==ip.host&&
			ConnectionPool[i].ip.port==ip.port)
			for(int j=i;j<OffsetPool;j++)
			{
				memcpy(&ConnectionPool[j],&ConnectionPool[j+1],sizeof(PoolElement));
				OffsetPool--;
			}
}

void Server::CheckZombies()
{
	for(int i=0;i<OffsetPool;i++)
	{
		Uint32 dTime=SDL_GetTicks()-ConnectionPool[i].time;

		if(dTime>OTIME_CONNECTION)
		{
			Uint32 ipaddr;
			ipaddr=SDL_SwapBE32(ConnectionPool[i].ip.host);

			Log::Output("Deleting a Zombie Connection from %d.%d.%d.%d\n",
				ipaddr>>24,
				(ipaddr>>16)&0xff,
				(ipaddr>>8)&0xff,
				ipaddr&0xff);

			DeleteFromConnectionPool(ConnectionPool[i].ip);
		}
	}
}

void Server::AddClient(char *name, char *pass, IPaddress ip)
{
	Log::Output("name %s,pass %s\n",name,pass);

	int ID=SPManager.GetNumPlayers();
	SPManager.Add(name,&ip,ID,"Humans",TEST_MAP);
	
	ServerPlayer *NewServerPlayer;
	NewServerPlayer=SPManager.GetPlayer(name);
	World_Server *WSptr;
	WSptr=GetWorld(NewServerPlayer->planetname);

	WSptr->PhManager.AddPlayer(NewServerPlayer->id);

	for(int i=0;i<SPManager.GetNumPlayers();i++)
	{
		ServerPlayer *SPptr;
		SPptr=SPManager.GetPlayerByNum(i);
		if(SPptr->ip!=NULL)
			if(strcmp(SPptr->planetname,NewServerPlayer->planetname)==0)
			{
				Net_Server.Send(*SPptr->ip,
					MSG_CLIENT_DATA,
					NewServerPlayer->id,
					NewServerPlayer->name,
					NewServerPlayer->agency,
					NewServerPlayer->race,
					NewServerPlayer->modelname);
			}
	}

	if(ServerType==(GAME_CLIENT|GAME_SERVER))
	{
		SDisplay.AddObject(NewServerPlayer);
	}

	NewServerPlayer->enabled=true;
}

Uint32 SendPing(Uint32 interval, void* param){

	Server *Sptr=(Server*)param;

	for(int i = 0 ; i < Sptr->SPManager.GetNumPlayers(); i++)
	{
		ServerPlayer *SPptr;

		SPptr=Sptr->SPManager.GetPlayerByNum(i);
		if(SPptr->PingFree==true)
		{
			if(SPptr->ip!=NULL)
			{
				Sptr->Net_Server.Send(*SPptr->ip,MSG_PING);	
				SPptr->PingFree=false;
				SPptr->PingTime=SDL_GetTicks();
			}
		}
	}

	return interval;
}

Uint32 SendPlayersStates(Uint32 interval,void *param)
{
	Server *Sptr=(Server*)param;
	ServerPlayerManager *SPMptr;

	SPMptr=&Sptr->SPManager;

	int numplayers=SPMptr->GetNumPlayers();

	for(int i=0;i<numplayers;i++)
	{
		ServerPlayer *SPptr;
		SPptr=SPMptr->GetPlayerByNum(i);
		if(SPptr->ip==NULL)
			continue;

		for(int j=0;j<numplayers;j++)
		{
			ServerPlayer *SP2ptr;
			SP2ptr=SPMptr->GetPlayerByNum(j);

			if(strcmp(SPptr->planetname,SP2ptr->planetname)==0)
				Sptr->Net_Server.Send(
					*SPptr->ip,
					MSG_CLIENT_STATE,
					SP2ptr->id,
					SP2ptr->ping,//ping WTF??
					SP2ptr->FirstHierarchy,
					SP2ptr->SecondHierarchy,
					SP2ptr->UltraHierarchy,
					SP2ptr->Position.GetVector(),
					SP2ptr->Velocity.GetVector(),
					SP2ptr->Rotation,
					SP2ptr->FeetState);
		}
	}

	return interval;
}

Uint32 SendBodyStates(Uint32 interval,void *param)
{
	/*
	Server *Sptr;

	Sptr=(Server *)param;

	int num_worlds=Sptr->GetNumWorlds();

	World_Server *WSptr;

	for(int i=0;i<num_worlds;i++)
	{
		WSptr=Sptr->GetWorldByNum(i);
		int num_bodies=WSptr->PhManager.GetNumBodies();
		for(int j=0;j<num_bodies;j++)
		{
			Vector3 Position;
			Vector3 Velocity;
			Vector3 AngularVel;
			Quaternion Quat;
			int BodyID;
			WSptr->PhManager.GetBodyState(j,&BodyID,Position,Velocity,AngularVel,Quat);

			int num_players=WSptr->PhManager.GetNumPlayers();
				
			for(int k=0;k<num_players;k++)
			{
				PhysicPlayer *PPptr;
				PPptr=WSptr->PhManager.GetPlayerByNum(k);

				ServerPlayer *SPptr;
				SPptr=Sptr->SPManager.GetPlayer(PPptr->PlayerID);
				if(SPptr->enabled==false)
					continue;

				WSptr->AddMessage(MSG_BODY_STATE,
					BodyID,
					Position.GetVector(),
					Velocity.GetVector(),
					AngularVel.GetVector(),
					Quat.GetQuaternion()
					);
			}
		}
	}
*/
	return interval;
	
}

void Server::UpdateClientRotation(int PlayerID, MsgClientRotation *NewRot)
{
	ServerPlayer *PlayerPtr;

	PlayerPtr=SPManager.GetPlayer(PlayerID);

	for(int i=0;i<2;i++)
	{
		PlayerPtr->Rotation[i]=NewRot->Rotation[i];
	}

	World_Server *WorldPtr;
	
	WorldPtr=GetWorld(PlayerPtr->planetname);
	WorldPtr->PhManager.UpdatePlayerRotation(PlayerID,PlayerPtr->Rotation);
	//UpdateClientState(PlayerID,
}

void Server::UpdateClientState(int PlayerID, MsgClientPositionChange *Change)
{
	int action=Change->input;
	ServerPlayer *PlayerPtr;

	PlayerPtr=SPManager.GetPlayer(PlayerID);
	
	if((action&A_STANDING))
	{
		PlayerPtr->SecondHierarchy&=(~S2_WALK);
		PlayerPtr->SecondHierarchy|=S2_STAND;
		PlayerPtr->SecondHierarchy&=(~S2_RUN);
	}

	if(	((action & A_MOVING_FRONT)==A_MOVING_FRONT)||
		((action & A_MOVING_BACK )==A_MOVING_BACK )||
		((action & A_STRAFE_RIGHT)==A_STRAFE_RIGHT)||
		((action & A_STRAFE_LEFT )==A_STRAFE_LEFT )){

		PlayerPtr->SecondHierarchy&=(~S2_WALK);
		PlayerPtr->SecondHierarchy&=(~S2_STAND);
		PlayerPtr->SecondHierarchy|=S2_RUN;

		if((action&A_SILENT_WALK)==A_SILENT_WALK)
		{
			PlayerPtr->SecondHierarchy|=S2_WALK;
			PlayerPtr->SecondHierarchy&=(~S2_RUN);
		}
	}

	World_Server *WorldPtr;
	
	WorldPtr=GetWorld(PlayerPtr->planetname);
	WorldPtr->PhManager.UpdatePlayer(	PlayerID,
										PlayerPtr->SecondHierarchy,
										PlayerPtr->UltraHierarchy,
										action);
	
}

World_Server *Server::GetWorldByNum(int num)
{
	World_Server *WSptr=SListWorlds;

	for(int i=0;i<num;i++)
		WSptr=(World_Server *)WSptr->next;

	if(WSptr!=NULL)
		return WSptr;
	
	return NULL;
}
