#ifndef MERC_WEAPONLIST_H
#define MERC_WEAPONLIST_H

#include "WeaponInfo.h"

class WeaponList
{
private:
	static WeaponInfo *Wlist;
public:
	static WeaponInfo *GetInfo(int id);
	static WeaponInfo *GetInfo(char *name);
	static void Init();
};

#endif