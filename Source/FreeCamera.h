#pragma once

//KYT '16.02.09
/*Free Camera Class*/
#include "Camera.h"
#include "CameraMove.h"
class CFreeCamera : public CCamera
{
private:


public:
	CFreeCamera(std::shared_ptr<CCamera> pCamera);

	virtual ~CFreeCamera() { }

	void Update(XMVECTOR& d3dxvLookAt, float fTimeElapsed) override;

	void SetRotation(float x, float y, float z) override;

	void SetLookAt(XMVECTOR& d3dxvLookAt) override;

	//void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) override;
};
