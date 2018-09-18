#ifndef MERC_WEAPONINFO_H
#define MERC_WEAPONINFO_H

class WeaponInfo
{
public:
	char model[32];
	int bullets_per_cartridge;
	WeaponInfo *next;
	WeaponInfo *prev;
};

#endif