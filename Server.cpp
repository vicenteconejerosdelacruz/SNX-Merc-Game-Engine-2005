#include "Merc.h"
#include "Server.h"
#include "ConfigFile.h"
#include "Log.h"
#include "Vector3.h"

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
}

Server::~Server()
{

}

int Server::GetInitQueue()
{
	return INIT_NETWORK|INIT_PHYSICS|INIT_TIMER;
}

void Server::Init()
{
	Services=GetInitQueue();

	UsingAcounts=Config.getBoolean("acounts");

	SDL_Init(0);	

	if((Services&INIT_NETWORK)==INIT_NETWORK)
		Net_Server.Init();

	if((Services&INIT_DB)==INIT_DB)
		InitWorldsUsingDB();
	else
		InitWorldsUsingLua();

	CreateTimers();

}

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

void Server::InitWorldsUsingLua()
{
	ConfigFile CFMap("maps.cfg");

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

	WSPtr->Init(name);
}

void Server::CreateTimers()
{
	SDL_InitSubSystem(SDL_INIT_TIMER);
	SDL_AddTimer(1000,SendPing,&SPManager);
	SDL_AddTimer(50,SendPlayersStates,&SPManager);
}

void Server::AdvanceFrame()
{
	now=SDL_GetTicks();
	delta=now-last;
	last=now;
	accum+=delta;

	GetIncomingData();
	HandleConnections();
	DispatchMessages();
	
	World_Server *WSptr;

	//Physic Step Size
	while(accum>=(int)(PHYSIC_STEP_SIZE/2*1000)){
		WSptr=SListWorlds;
		for(int i=0;i<NumWorlds;i++)
		{
			WSptr->PhManager.AdvanceFrame();
			WSptr=WSptr->next;
		}
		
		accum-=(int)(PHYSIC_STEP_SIZE/2*1000);
	}

	//update client states
	for(int i=0;i<SPManager.GetNumPlayers();i++)
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
		WSptr->PhManager.GetPlayerState(SPptr->id,Position,Velocity);
		SPManager.SetPlayerState(SPptr->id,Position,Velocity);
	}

	DispatchMessages();
}

void Server::HandleConnections()
{
	char Data[1024];

	for(int i=0;i<OffsetPool;i++)
		if(Net_Server.isSocketActive(SocketPool[i]))
		{
			MsgHeader Header;
			if(Net_Server.Recv(SocketPool[i],&Header,Data))
			{
				switch(Header.type)
				{
				case MSG_LOGIN:
					MsgLogin *MLogin;
					MLogin=(MsgLogin *)Data;
					if(UsingAcounts)
					{
						AddClientUsingAccount(MLogin,SocketPool[i]);
					}
					else
					{
						AddClient(MLogin,SocketPool[i]);
					}

				break;
				default:

				break;
				}
			}
		}
}

void Server::AddClientUsingAccount(MsgLogin *MLogin,TCPsocket socket)
{

}

void Server::AddClient(MsgLogin *MLogin,TCPsocket socket)
{
	bool kicked;
	kicked=false;

	Log::Output("--%s Trying to Connect--\n",MLogin->user);

	if(SPManager.GetPlayer(MLogin->user)!=NULL)
	{
		IPaddress *remoteip;
		Uint32 ipaddr;
		remoteip=SDLNet_TCP_GetPeerAddress(socket);
		ipaddr=SDL_SwapBE32(remoteip->host);

		Log::Output("%s Has Been Kicked from %d.%d.%d.%d:%hu\n",MLogin->user,
					ipaddr>>24,
					(ipaddr>>16)&0xff,
					(ipaddr>>8)&0xff,
					ipaddr&0xff,
					remoteip->port);

		return;	
	}
					
	//it supposed that this thing are for the typical FPS
	//not experience,nothing of that RPG's Things :P,etc
	int ID=SPManager.GetNumPlayers();
	SPManager.Add(MLogin->user,socket,ID,"default","Korhal");
	int ClientID;
	ClientID=SPManager.GetPlayer(MLogin->user)->id;

	//Not Using Accounts mean's that we have to spawn the client in the Main World
	//CHANGE: "Korhal"
	World_Server *WSptr;
	WSptr=GetWorld("Korhal");

	if(WSptr==NULL)
	{
		Net_Server.DeleteSocket(socket);
		DeleteFromPool(socket);
		SPManager.Delete(ClientID);
		return;
	}

	SPManager.AddMessage(ClientID,MSG_LOAD_MAP,"Korhal");
	WSptr->PhManager.AddPlayer(ClientID);
	ServerPlayer *SPptr;

	for(int i=0;i<SPManager.GetNumPlayers();i++)
	{
		SPptr=SPManager.GetPlayerByNum(i);
		if(strcmp(SPptr->planetname,"Korhal")==0)
		{
			if(strcmp(SPptr->name,MLogin->user)!=0)
			{
				SPManager.AddMessage(SPptr->id,MSG_CLIENT_DATA,SPManager.GetPlayer(ClientID));
				SPManager.AddMessage(ClientID,MSG_CLIENT_DATA,SPptr);
			}
		}
	}

	SPManager.AddMessage(ClientID,MSG_CLIENT_DATA,SPManager.GetPlayer(ClientID));
	Vector3 Position;
	Vector3 Velocity;

	WSptr->PhManager.GetPlayerState(ClientID,Position,Velocity);
	SPManager.SetPlayerState(ClientID,Position,Velocity);
	SPManager.AddMessage(ClientID,MSG_CLIENT_STATE);
	SPManager.GetPlayer(ClientID)->enabled=true;
	DeleteFromPool(socket);
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
	TCPsocket NewConnection=Net_Server.CheckNewConnection();

	if(NewConnection!=NULL)
	{
		Net_Server.AddSocket(NewConnection);
		SocketPool[OffsetPool]=NewConnection;
		OffsetPool++;
	}
}

void Server::DispatchMessages()
{
	char Data[1024];

	for(int i = 0 ; i < SPManager.GetNumPlayers() ; i++){
		
		memset(Data,0,sizeof(Data));
		int size;
		size=SPManager.GetPlayerByNum(i)->GetMessages(Data);

		if(size>0)
			Net_Server.Send(SPManager.GetPlayerByNum(i)->socket,Data,size);
	}
}

bool Server::ShutDownPending()
{

	return false;
}

Uint32 SendPing(Uint32 interval, void* param){
	
	ServerPlayerManager *SPMptr=(ServerPlayerManager*)param;
	int nPlayers=SPMptr->GetNumPlayers();

	for(int i = 0 ; i < nPlayers ; i++)
		if(SPMptr->GetPlayer(i)->PingFree==true){
			int id;
			id=SPMptr->GetPlayer(i)->id;
			SPMptr->AddMessage(id,110);
			SPMptr->GetPlayer(i)->PingFree=false;
		}

	return interval;
}

Uint32 SendPlayersStates(Uint32 interval,void *param)
{
	ServerPlayerManager *SPMptr=(ServerPlayerManager*)param;

	int numplayers=SPMptr->GetNumPlayers();
	for(int i=0;i<numplayers;i++)
	{
		if(SPMptr->GetPlayerByNum(i)->enabled==true)
			SPMptr->AddMessage(SPMptr->GetPlayerByNum(i)->id,MSG_CLIENT_STATE);
	}
	return interval;
}

void Server::DeleteFromPool(TCPsocket socket)
{
	for(int i=0;i<OffsetPool;i++)
		if(SocketPool[i]==socket)
		{
			for(int e=i;e<OffsetPool;e++)
				SocketPool[e]=SocketPool[e+1];

			OffsetPool--;
			break;
		}
}
