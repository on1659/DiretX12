#pragma once

#include "InstancedObjectsShader.h"

class CWarp3DModel;
class CWarpAnimation;

class CWarp3DSkinningShader : public CInstancedObjectsShader
{
public:
	CWarp3DSkinningShader(std::unique_ptr<CMesh> pMesh);
	CWarp3DSkinningShader(int nObjects = 1);
	~CWarp3DSkinningShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	virtual void AnimateObjects(float fTimeElapsed);

protected:
	CWarpAnimation					*m_pAnimData;
	CWarp3DModel					*m_pModelData;

	XMFLOAT4X4						m_d3dxmtxTexture;
	GlobalMatrixBoneTransforms		m_boneTransform;
	ID3D11Buffer					*m_pcbBoneTransform;

	ID3D11Buffer					*m_pd3dBlendingIndexBuffer;
	ID3D11Buffer					*m_pd3dBlendingWeightBuffer;

	bool							bBoneTransformSetting;
	int								m_currentKeyFrameNum;
	float							m_keyframeStack;
};

class CWarp3DSkinningShaderforQueryChan : public CWarp3DSkinningShader
{
public:
	CWarp3DSkinningShaderforQueryChan(int nObjects = 1);
	virtual void AnimateObjects(float fTimeElapsed);
};

