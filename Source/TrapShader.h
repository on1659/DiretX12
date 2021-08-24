#pragma once

#include "HeadBangBangShader.h"

class CTrapShader : public CHeadBangBangShader
{

public:

	CTrapShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CTrapShader(std::unique_ptr<CMesh> pMesh);

	CTrapShader(int nObjects = 1);

	virtual ~CTrapShader();

	void RenderTrap(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	void AnimateTrap(float fTimeElapsed);
};

