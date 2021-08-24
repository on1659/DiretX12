#pragma once

#include "Mesh.h"
#include "GameObject.h"

#define ASPECT_RATIO				(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

#define FIRST_PERSON_CAMERA			0x01
#define SPACESHIP_CAMERA			0x02
#define THIRD_PERSON_CAMERA			0x03
#define FREE_CAMERA					0x04

class CPlayer;

//KYT '16.02.02  plus
/*Instanced는 VIew, Projection이 따로 따로 있어야되기 때문에*/
struct VS_CB_CAMERA
{
	XMFLOAT4X4					m_d3dmtxViewProjection;
	//XMFLOAT4X4				m_d3dxmtxView;
	//XMFLOAT4X4				m_d3dxmtxProjection;
};
struct VS_CB_SHADOW_CAMERA
{
	//XMFLOAT4X4					m_d3dmtxViewProjection;
	XMFLOAT4X4						m_d3dxmtxView;
	XMFLOAT4X4						m_d3dxmtxProjection;
};

//--------------------------------------직교 투영 JJY
struct VS_CB_CAMERA_ORTHO
{
	XMFLOAT4X4							m_d3dxmtxOrthoViewProjection;
	//XMFLOAT4X4						m_d3dxmtxOrthoView;
	//XMFLOAT4X4						m_d3dxmtxOrtho;

};

//-----------------------------------------지오매트리 KJH
struct TS_CB_CAMERA
{
	XMFLOAT4X4						m_d3dxmtxViewProjection;
	XMFLOAT3						m_d3dxCameraDistance;
	//D3DXMATRIX m_d3dxmtxViewProjection;
};

//
struct PS_CB_CAMERA
{
	XMFLOAT4 m_xmCameraPosition;
};

class CCamera : public Object
{
protected:
	XMFLOAT3						m_d3dxvPosition;
	XMFLOAT3						m_d3dxvRight;
	XMFLOAT3						m_d3dxvUp;
	XMFLOAT3						m_d3dxvLook;


	float           				m_fPitch;
	float           				m_fRoll;
	float           				m_fYaw;

	DWORD							m_nMode;

	XMFLOAT3						m_d3dxvOffset;
	float           				m_fTimeLag;
public:
	XMFLOAT4X4						m_d3dxmtxView;
	XMFLOAT4X4						m_d3dxmtxProjection;

protected:
	D3D11_VIEWPORT					m_d3dViewport;

	static ID3D11Buffer				*m_pd3dcbCamera;

	//지오매투리
	static ID3D11Buffer				*m_pd3dGScbCamera;

	//ps매투리
	static ID3D11Buffer				*m_pd3dPScbCamera;

	//Shadow Buffer
	static ID3D11Buffer				*m_pd3dcShaodwbCamera;
	XMFLOAT4X4						 m_d3dxmtxShadowView;
	XMFLOAT4X4						m_d3dxmtxShadowProjection;


	CPlayer							*m_pPlayer;

	XMFLOAT4						m_pd3dxFrustumPlanes[6]; //World Coordinates          

															 //Triangle
	XMVECTOR						xm3FrustumVertex[3];

	XMFLOAT3						xf3_RegenerateViewMatrix;

public:
	CCamera(std::shared_ptr<CCamera> pCamera = nullptr);
	virtual ~CCamera();

	void SetMode(DWORD nMode) { m_nMode = nMode; }
	DWORD GetMode() { return(m_nMode); }

	void GenerateViewMatrix();
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	static void CreateConstBuffers(ID3D11Device *pd3dDevice);
	static void ReleaseConstBuffers();

	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX& pd3dxmtxView, XMMATRIX& pd3dxmtxProjection);
	void UpdateConstBuffers(ID3D11DeviceContext *pd3dDeviceContext);


	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	CPlayer *GetPlayer() { return(m_pPlayer); }

	void SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetViewport(ID3D11DeviceContext *pd3dDeviceContext);
	D3D11_VIEWPORT GetViewport() { return(m_d3dViewport); }



	XMMATRIX& GetViewMatrix() { return(XMLoadFloat4x4(&m_d3dxmtxView)); }
	XMMATRIX& GetProjectionMatrix() { return(XMLoadFloat4x4(&m_d3dxmtxProjection)); }

	XMFLOAT4X4& GetViewMatrixXMFLOAT4X4() { return((m_d3dxmtxView)); }
	XMFLOAT4X4& GetProjectionMatrixXMFLOAT4X4() { return((m_d3dxmtxProjection)); }

	XMFLOAT4X4& GetShadowViewMatrixXMFLOAT4X4() { return((m_d3dxmtxShadowView)); }
//	XMFLOAT4X4& GetShadowProjectionMatrixXMFLOAT4X4() { return((m_d3dxmtxShadowProjection)); }

	ID3D11Buffer *GetCameraConstantBuffer() { return(m_pd3dcbCamera); }


	void SetPosition(const XMFLOAT3& d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	XMVECTOR& GetPosition() { m_xmVector = (XMLoadFloat3(&m_d3dxvPosition)); return m_xmVector; }
	XMFLOAT3& GetPositionXMFLOAT3() { return(m_d3dxvPosition); }

	XMVECTOR& GetRightVector() { m_xmVector = (XMLoadFloat3(&m_d3dxvRight)); return m_xmVector; }
	XMVECTOR& GetUpVector() { m_xmVector = (XMLoadFloat3(&m_d3dxvUp)); return m_xmVector; }
	XMVECTOR& GetLookVector() { m_xmVector = (XMLoadFloat3(&m_d3dxvLook)); return m_xmVector; }

	XMFLOAT3& GetRightVectorXMFLOAT3() { return((m_d3dxvRight)); }
	XMFLOAT3& GetUpVectorXMFLOAT3() { return((m_d3dxvUp)); }
	XMFLOAT3& GetLookVectorXMFLOAT3() { return((m_d3dxvLook)); }

	float& GetPitch() { return(m_fPitch); }
	float& GetRoll() { return(m_fRoll); }
	float& GetYaw() { return(m_fYaw); }

	void SetOffset(XMVECTOR d3dxvOffset)
	{
		XMStoreFloat3(&m_d3dxvOffset, d3dxvOffset);
		XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
		v += d3dxvOffset;
		XMStoreFloat3(&m_d3dxvPosition, v);
	}
	XMVECTOR& GetOffset() { return(XMLoadFloat3(&m_d3dxvOffset)); }
	XMFLOAT3& GetOffsetXMFLOAT3() { return((m_d3dxvOffset)); }



	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return(m_fTimeLag); }

	virtual void Move(const XMVECTOR& d3dxvShift)
	{
		XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
		v += d3dxvShift;
		XMStoreFloat3(&m_d3dxvPosition, v);
	}
	virtual void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
	{
		if (dwDirection)
		{
			XMVECTOR d3dxvShift = XMVectorZero();
			if (dwDirection & DIR_FORWARD)	d3dxvShift += XMLoadFloat3(&m_d3dxvLook) * fDistance;
			if (dwDirection & DIR_BACKWARD) d3dxvShift -= XMLoadFloat3(&m_d3dxvLook) * fDistance;
			if (dwDirection & DIR_RIGHT)	d3dxvShift += XMLoadFloat3(&m_d3dxvRight) * fDistance;
			if (dwDirection & DIR_LEFT)		d3dxvShift -= XMLoadFloat3(&m_d3dxvRight) * fDistance;
			if (dwDirection & DIR_UP)		d3dxvShift += XMLoadFloat3(&m_d3dxvUp) * fDistance;
			if (dwDirection & DIR_DOWN)		d3dxvShift -= XMLoadFloat3(&m_d3dxvUp) * fDistance;


			XMVECTOR d3dxvPosition = XMLoadFloat3(&m_d3dxvPosition);
			d3dxvPosition = d3dxvPosition + d3dxvShift;
			XMStoreFloat3(&m_d3dxvPosition, XMVectorAdd(XMLoadFloat3(&m_d3dxvPosition), d3dxvShift));

		}
	}

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void SetRotation(float x, float y, float z) {}
	virtual void Update(XMVECTOR& d3dxvLookAt, float fTimeElapsed) { }
	virtual void Update(XMVECTOR& d3dxvLookAt, CPlayer* player, float fTimeElapsed) { }
	virtual void Update(CPlayer* player, float fTimeElapsed) { }
	virtual void Update(float fTimeElapsed) { }
	virtual void SetLookAt(XMVECTOR& vLookAt) { }
	virtual void SetLookAt() { }
	virtual void SetLookAt(XMVECTOR& d3dxvPosition, XMVECTOR& d3dxvLookAt, XMVECTOR& d3dxvUp);

	void CalculateFrustumPlanes();

	bool IsInFrustum(AABB *pAABB);
	bool IsInFrustum(XMFLOAT3& d3dxvMinimum, XMFLOAT3& d3dxvMaximum);
	bool IsInFrustumQuad(AABB *pAABB);
	bool IsInFrustumQuad(XMFLOAT3& d3dxvMinimum, XMFLOAT3& d3dxvMaximum);
	bool IsInFrustum2D(XMFLOAT3 fCenter);


	//KYT '16.02.02
	/*Ortho 최초 1회만 하면 된다.*/
protected:
	static ID3D11Buffer				*m_pd3dcbCameraOrtho;

	XMFLOAT4X4						m_d3dxmtxOrthoView;
	XMFLOAT4X4						m_d3dxmtxOrtho;

	//--------------------------------------직교 투영 JJY

public:
	static void OrthoCreateConstBuffer(ID3D11Device *pd3dDevice);

	void OrthoCameraLoad(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext = nullptr);

private:
	void OrthoGenerateProjectionMatrix();

	void UpdateShaderVariableOrtho(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX& pd3dxmtxView, XMMATRIX& pd3dxmtxProjection);

	void UpdateShaderVariablesOrtho(ID3D11DeviceContext *pd3dDeviceContext);

	XMMATRIX& GetOrthoViewMatrix() { return XMLoadFloat4x4(&m_d3dxmtxOrthoView); }

	XMMATRIX& GetOrthoMatrix() { return XMLoadFloat4x4(&m_d3dxmtxOrtho); }

	XMFLOAT4X4& GetOrthoViewMatrixXMFLOAT4X4() { return (m_d3dxmtxOrthoView); }

	XMFLOAT4X4& GetOrthoMatrixXMFLOAT4X4() { return (m_d3dxmtxOrtho); }


private:
	float m_fFovAngle;
	float m_fFarPlaneDistance;

	D3DXVECTOR3 v1, v2, v3;

	//'16.03.26
private:
	int m_nDepth;
	XMFLOAT3 m_xmSpaceSize;

	void SetSpacePartition(XMFLOAT3 xmSize, int nDepth)
	{
		m_nDepth = nDepth;
		m_xmSpaceSize = xmSize;
	}

	//그림자
public:

	XMMATRIX *m_xmf4Shadows;

	void UpdateShadowConstBufferUpdate(ID3D11DeviceContext *pd3dDeviceContext);

	void CreateShadowBuffer(const XMFLOAT3& position, const XMFLOAT3& up = XMFLOAT3(0.0f, 1.0f, 0.0f));
};
