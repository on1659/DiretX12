#pragma once

#include "Camera.h"

class CSpaceShipCamera : public CCamera
{
public:
	CSpaceShipCamera(std::shared_ptr<CCamera> pCamera);
	
	virtual ~CSpaceShipCamera() { }

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};
