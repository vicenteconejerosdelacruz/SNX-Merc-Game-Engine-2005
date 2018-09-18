#ifndef MERC_SWEAPON_H
#define MERC_SWEAPON_H

class SWeapon
{
public:
	SWeapon *next;
	SWeapon *prev;
	int id;
	int bullets;
	int cartridges;
};

#endif