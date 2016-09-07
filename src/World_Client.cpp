#include "World_Client.h"
#include "Constants.h"
#include "Log.h"
#include "ConfigFile.h"

World_Client::World_Client()
{
	sizeX=0;
	sizeY=0;
	memset(name,0,sizeof(name));
}

void World_Client::Init(char *WorldName)
{
	//Use The CACHE please xD
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

	Patches=new Patch[(sizeX/PATCH_SIZE)*(sizeY/PATCH_SIZE)];

	for(int Y=0;Y<(sizeY/PATCH_SIZE);Y++)
		for(int X=0;X<(sizeY/PATCH_SIZE);X++)
		{
			Patch *patch;

			patch=&Patches[Y*(sizeX/PATCH_SIZE)+X];
			patch->Init(X*PATCH_SIZE,Y*PATCH_SIZE,X*PATCH_SIZE,Y*PATCH_SIZE,sizeX,sizeY,HeightMap);
			patch->ComputeVariance();
		}
}

void World_Client::Render()
{

}