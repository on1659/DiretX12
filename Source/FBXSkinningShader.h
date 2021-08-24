#pragma once

//RSH 16.03.04
/*
Skinned Animation FBXModel
*/

#include "InstancedObjectsShader.h"

class FBXAnimationModelLoader;

class CFBXSkinnedShader : public CInstancedObjectsShader
{
public:
	CFBXSkinnedShader(std::unique_ptr<CMesh> pMesh);
	CFBXSkinnedShader(int nObjects = 1);
	CFBXSkinnedShader::CFBXSkinnedShader(FBXAnimationModelLoader fbxLoader, int nObjects = 1);
	~CFBXSkinnedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	virtual void AnimateObjects(float fTimeElapsed);

	void KeyframeReset() { m_currentKeyFrameNum = 0; }

protected:

	FBXAnimationModelLoader			*m_pSkinnedModel;
	XMFLOAT4X4						m_d3dxmtxTexture;

	GlobalMatrixBoneTransforms		m_boneTransform;
	ID3D11Buffer					*m_pcbBoneTransform;

	ID3D11Buffer					*m_pd3dBlendingIndexBuffer;
	ID3D11Buffer					*m_pd3dBlendingWeightBuffer;

	bool							bBoneTransformSetting;

	float							m_currentKeyFrameNum;
};

class CFBXSkinnedShaderforZombunny : public CFBXSkinnedShader
{
public:
	CFBXSkinnedShaderforZombunny(int nObjects = 1);
};

class CFBXSkinnedShaderforRunningZombunny : public CFBXSkinnedShader
{
public:
	CFBXSkinnedShaderforRunningZombunny(int nObjects = 1);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	virtual void AnimateObjects(float fTimeElapsed);
};

class CFBXSkinnedShaderforPlayerCharacter : public CFBXSkinnedShader
{
public:
	CFBXSkinnedShaderforPlayerCharacter(int nObjects = 1);
};