#ifndef MERC_CLIENTPLAYER_H
#define MERC_CLIENTPLAYER_H

#include "Player.h"
#include "Model.h"
#include "RenderAble.h"

class ClientPlayer:public Player,public RenderAble
{
public:
	Model model;
	void Render();

};

#endif