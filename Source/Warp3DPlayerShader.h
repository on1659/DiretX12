#pragma once

//RSH '16.07.16 
/*
	CWarp3DPlayerShader 제작
*/

#include "Warp3DSkinningShader.h"


class CWarp3DModel;
class CWarpAnimation;

//----------------------------------------------------------------

class CWarp3DPlayerShader : public CInstancedObjectsShader
{

public:

	CWarp3DPlayerShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CWarp3DPlayerShader(std::unique_ptr<CMesh> pMesh);

	CWarp3DPlayerShader(int nObjects = 1);

	virtual ~CWarp3DPlayerShader();

	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void AnimateObjects(float fTimeElapsed);
	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	CWarpAnimation* GetAnimationData(AnimState anim) { return m_pAnimations[anim]; }

	void SetMeshName(const std::string& name)
	{
		if(m_pMesh)m_pMesh->name = name;
	}

protected:

	//RSH 2016.04.21
	/*
	플레이어 캐릭터 AJ의 스키닝 애니메이션 쉐이더 기능이 이 쉐이더에 추가되었습니다.
	*/

	CWarp3DModel					*m_pModelData;

	std::vector<CWarpAnimation*>	m_pAnimations;
	XMFLOAT4X4						m_d3dxmtxTexture;

	GlobalMatrixBoneTransforms		m_boneTransform;
	ID3D11Buffer					*m_pcbBoneTransform;

	ID3D11Buffer					*m_pd3dBlendingIndexBuffer;
	ID3D11Buffer					*m_pd3dBlendingWeightBuffer;
	bool							bBoneTransformSetting;

	int								m_currentKeyFrameNum;
	float							m_keyframeStack;
	AnimState						m_currnetAnimationState;
};

class CQueryChanPlayerShader : public CWarp3DPlayerShader
{
public:
	CQueryChanPlayerShader(int nObjects = 1);

	void AddPlayer(std::shared_ptr<CPlayer> pPlayer);

};

