#pragma once

#include "Player.h"

class COtherPlayer : public CPlayer
{
public:
	COtherPlayer(int nObjects = 1);

	~COtherPlayer();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

};

