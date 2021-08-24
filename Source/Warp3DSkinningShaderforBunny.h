#pragma once

#include "stdafx.h"
#include "Warp3DSkinningShader.h"

#define BUNNY_SKINNEDINSTANCING

class CWarp3DSkinningShaderforBunny : public CWarp3DSkinningShader
{
public:
	CWarp3DSkinningShaderforBunny(int nObjects = 1);
	virtual void AnimateObjects(float fTimeElapsed);
#ifdef BUNNY_SKINNEDINSTANCING
	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
#endif
private:
	std::vector<XMFLOAT4> texelsBuffer;
	ID3D11ShaderResourceView		**animationTexSRV;
};