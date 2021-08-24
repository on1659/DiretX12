#pragma once

#include "Object.h"
#include "Camera.h"

class CInstancedObjectsShader;
class CShadowMapShader;
class PlayerState;
class CBulletShader;
class CHeadBangBangShader;
class PlayerAnimation;
class CCubeParticleShader;

class CPlayer : public CGameObject
{

protected:
	XMFLOAT3							m_d3dxvPosition;
	XMFLOAT3							m_d3dxvRight;
	XMFLOAT3							m_d3dxvUp;
	XMFLOAT3							m_d3dxvLook;

	float           					m_fPitch;
	float           					m_fYaw;
	float           					m_fRoll;

	XMFLOAT3							m_d3dxvVelocity;
	XMFLOAT3     						m_d3dxvGravity;
	float           					m_fMaxVelocityXZ;
	float           					m_fMaxVelocityY;
	float           					m_fFriction;

	LPVOID								m_pPlayerUpdatedContext;
	LPVOID								m_pCameraUpdatedContext;

	bool								m_bHovering;

	//RSH '16.05.11
	/*
		등짝때리기(Smash)의 타격 범위를 나타내는 바운딩 박스 추가
	*/
	//BoundingBox						m_bcSmashBoundingbox;
	//BoundingBox						m_bcBackBoundingbox;

	//KYT '16.05.08
	/*
		머리위에 큐브
	*/
	std::unique_ptr<CInstancedObjectsShader>	m_pCubeShader;

	//KYT '16.05.10
	/*
		그림자 텍스쳐
	*/
	std::unique_ptr<CShadowMapShader>			m_pShadowShader;

	//KYT '16.07.21
	/*
		헤드뱅뱅
	*/
	bool										m_bHeadMove;
	std::unique_ptr<CHeadBangBangShader>		m_pHeadBangBangShader;


public:
	CPlayer(int nMeshes = 1);
	virtual ~CPlayer();

	void Release();

	XMVECTOR& GetPosition()
	{
		m_xmVector = XMLoadFloat3(&m_d3dxvPosition);
		return(m_xmVector);
	}
	XMVECTOR& GetLookVector() { m_xmVector = (XMLoadFloat3(&m_d3dxvLook)); return m_xmVector; }
	XMVECTOR& GetUpVector() { m_xmVector = (XMLoadFloat3(&m_d3dxvUp)); return m_xmVector; }
	XMVECTOR& GetRightVector() { m_xmVector = (XMLoadFloat3(&m_d3dxvRight)); return m_xmVector; }

	XMFLOAT3& GetPositionXMFLOAT3() { return(m_d3dxvPosition); }
	XMFLOAT3& GetLookVectorXMFLOAT3() { return((m_d3dxvLook)); }
	XMFLOAT3& GetUpVectorXMFLOAT3() { return((m_d3dxvUp)); }
	XMFLOAT3& GetRightVectorXMFLOAT3() { return((m_d3dxvRight)); }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const XMVECTOR& d3dxvGravity) { XMStoreFloat3(&m_d3dxvGravity, d3dxvGravity); }
	void SetGravity(const XMFLOAT3& d3dxvGravity) { m_d3dxvGravity = d3dxvGravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const XMVECTOR& d3dxvVelocity) { XMStoreFloat3(&m_d3dxvVelocity, d3dxvVelocity); }
	void SetVelocity(const XMFLOAT3& d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; }

	const XMVECTOR& GetVelocity() { m_xmVector = (XMLoadFloat3(&m_d3dxvVelocity)); return m_xmVector; }
	const XMFLOAT3& GetVelocityXMFLOAT3() const { return(m_d3dxvVelocity); }

	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }

	//--------------------------------------------------- ---------------

	void OnPrepareRender();

	virtual void SetPosition(float x, float z);
	virtual void SetPosition(const XMVECTOR& d3dxvPosition);
	virtual void SetPosition(const XMFLOAT3& d3dxvPosition);
	virtual void OffsetPosition(const XMFLOAT3& xmf3Offset);
	virtual void OffsetPosition(const XMVECTOR& xmf3Offset);


	virtual void Move(DWORD nDirection, float fDistance, bool bVelocity = false);
	virtual void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	virtual void Move(const XMVECTOR& d3dxvShift, bool bVelocity = false);
	virtual void UnMove(DWORD nDirection, float fDistance, bool bVelocity = false);
	void SetRotation(float x, float y, float z);


	virtual void OnPlayerUpdated(float fTimeElapsed);
	virtual void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	virtual void OnCameraUpdated(float fTimeElapsed);
	virtual void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	virtual void CreateConstBuffers(ID3D11Device *pd3dDevice);


	virtual void UpdateConstBuffers(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void CreateHeadUpCube(ID3D11Device* pd3dDebice);

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	virtual void Animate(float fTimeElapsed, XMMATRIX *pd3dxmtxParent);
	virtual void Animate(float fTimeElapsed);
	virtual void Rotate(float x, float y, float z);

	//KYT '16.07.21
	/*
		외부 접근
	*/
	virtual void StartHeadBangBang();
	virtual void StopHeadBangBang();

	//KYT '16.04.02 plus
	/*
		Terrain Bounding Check

	*/
	BoundingBox GetBoundingBox() override
	{
		m_bcMeshBoundingbox.Center =
			XMFLOAT3(m_d3dxvPosition.x + (m_bcMeshBoundingbox.Extents.x * 0.5f),
				m_d3dxvPosition.y + (m_bcMeshBoundingbox.Extents.y * 0.5f),
				m_d3dxvPosition.z + (m_bcMeshBoundingbox.Extents.z * 0.5f));
		return m_bcMeshBoundingbox;

	}

	//KYT '16.05.11
	/*
		등짝때리기(Smash) 충돌박스 관련 함수 추가
	*/
	#pragma region[추가할 필요 없음]
		/*
			void SetSmashBoundingbox(CMesh* pMesh)
			{
				if (pMesh)m_bcSmashBoundingbox = pMesh->GetBoundingBox();
			}
			BoundingBox GetSmashBoundingbox() { return m_bcSmashBoundingbox; }
		*/
	#pragma endregion

	//KYT '16.06.06
	PlayerState* GetPlayerState() const { return m_cPlayerState; }
	void Respawn();

};


