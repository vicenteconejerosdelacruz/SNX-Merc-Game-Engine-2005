#ifndef MERC_DYNAMICMANAGER_H
#define MERC_DYNAMICMANAGER_H

#include "DynamicObject.h"
#include "Vector3.h"

class DynamicManager
{
private:
	DynamicObject *DOlist;
public:
	void AddObject(float *Position,float *Rotation,char *modelname);
	void DeleteObject(int id);
	DynamicObject *GetObject(int id);
	void UpdateObject(int id,Vector3 Position,Vector3 Velocity,Vector3 Rotation);
	void AdvanceFrame();
};

#endif