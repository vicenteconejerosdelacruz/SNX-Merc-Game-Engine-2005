#include "World_Server.h"
#include "Constants.h"
#include "Log.h"
#include "ConfigFile.h"

World_Server::World_Server()
{
	sizeX=0;
	sizeY=0;
	memset(name,0,sizeof(name));
	
}
void World_Server::Init(char *WorldName)
{
	strcat(name,WorldName);

	char MapFile[64];
	memset(MapFile,0,sizeof(MapFile));
	sprintf(MapFile,"%s%s%s",MAP_DIR,name,MAP_EXTENSION);

	FILE *fp;

	fp=fopen(MapFile,"rb");
	if(fp==NULL){
		Log::Output("Could not found map:%s\n",MapFile);
		return;
	}

	char MapID[4];
	int MapVersion;	

	memset(MapID,0,sizeof(MapID));
	fread(MapID,sizeof(MapID),1,fp);
	if(strncmp(MapID,MAP_ID,strlen(MAP_ID))!=0){
		Log::Output("Map incompatible %s\n",MapFile);
		return;
	}
	
	fread(&MapVersion,sizeof(int),1,fp);
	if(MapVersion!=MAP_VERSION){
		Log::Output("Map incompatible %s\n",MapFile);
		return;
	}

	MapHeader MHeader;

	fread(&MHeader,sizeof(MHeader),1,fp);
	
	sizeX=MHeader.sizeX;
	sizeY=MHeader.sizeY;

	HeightMap=new unsigned char[sizeX*sizeY];
	fread(HeightMap,sizeX*sizeY,1,fp);
	fclose(fp);

	ConfigFile CF("maps.cfg");
	int num_maps;
	num_maps=CF.getInteger("num_maps");

	PhManager.Init(HeightMap,sizeX,sizeY);

	for(int i=0;i<num_maps;i++)
		if(strcmp(WorldName,CF.getString("map%d.name",i))==0){				

			int NumObjects=CF.getInteger("map%d.num_structures",i);

			for(int e=0;e<NumObjects;e++){
				float Position[3];
				sscanf(	CF.getString("map%d.structure_%d_pos",i,e),
						"%f | %f | %f",
						&Position[0],
						&Position[1],
						&Position[2]);

				float Rotation[3];
				sscanf(	CF.getString("map%d.structure_%d_rot",i,e),
						"%f | %f | %f",
						&Rotation[0],
						&Rotation[1],
						&Rotation[2]);

				PhManager.AddObject(PHYSIC_OBJECT,CF.getString("map%d.structure_%d_model",i,e),										Position,Rotation);
			}
						
			return;
		}
	
}