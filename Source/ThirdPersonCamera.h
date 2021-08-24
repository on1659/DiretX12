#pragma once

#include "Camera.h"

class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(std::shared_ptr<CCamera> pCamera = nullptr);
	virtual ~CThirdPersonCamera() { }

	virtual void Update(XMVECTOR& d3dxvLookAt, float fTimeElapsed);
	virtual void Update(XMVECTOR& d3dxvLookAt, CPlayer* player, float fTimeElapsed);
	virtual void Update(CPlayer* player, float fTimeElapsed);
	virtual void Update(float fTimeElapsed);
	virtual void SetLookAt(XMVECTOR& vLookAt);
	virtual void SetLookAt();
};