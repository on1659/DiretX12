//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once

#define VERTEX_POSITION_ELEMENT			0x0001
#define VERTEX_COLOR_ELEMENT			0x0002
#define VERTEX_NORMAL_ELEMENT			0x0004
#define VERTEX_TEXTURE_ELEMENT_0		0x0008
#define VERTEX_TEXTURE_ELEMENT_1		0x0010

#define TEXTURE_ELEMENT_0_TRANSFORM		0x0100
#define TEXTURE_ELEMENT_1_TRANSFORM		0x0200

#define VERTEX_INSTANCING_ELEMENT		0x0020

#define VERTEX_BONE_ID_ELEMENT			0x0400
#define VERTEX_BONE_WEIGHT_ELEMENT		0x0800

#define VERTEX_BLENDING_SHADER			0x1000
#define INSTANCING_SHADER				0x2000

struct MESHINTERSECTINFO {
	DWORD	m_dwFaceIndex;
	float	m_fU;
	float	m_fV;
	float	m_fDistance;
};

//------------------------------------------------------------------------------------------------
#define EPSILON				1.0e-10f

inline bool IsZero(float fValue) { return ((fabsf(fValue) < EPSILON)); }
inline bool IsEqual(float fA, float fB) { return (::IsZero(fA - fB)); }
inline float InverseSqrt(float fValue) { return 1.0f / sqrtf(fValue); }
inline void Swap(float *pfS, float *pfT) { float fTemp = *pfS; *pfS = *pfT; *pfT = fTemp; }

extern bool RayIntersectTriangle(XMVECTOR *pd3dxvOrigin, XMVECTOR *pd3dxvDirection, XMVECTOR *pd3dxvP0, XMVECTOR *pd3dxvP1, XMVECTOR *pd3dxvP2, float *pfU, float *pfV, float *pfRayToTriangle);

////------------------------------------------------------------------------------------------------
#ifdef _AABB_
class AABB
{
public:
	XMFLOAT3						m_d3dxvMinimum;
	XMFLOAT3						m_d3dxvMaximum;

public:
	AABB()
	{
		m_d3dxvMinimum = XMFLOAT3{ +FLT_MAX, +FLT_MAX, +FLT_MAX };
		m_d3dxvMaximum = XMFLOAT3{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	};
	AABB(XMVECTOR d3dxvMinimum, XMVECTOR d3dxvMaximum)
	{
		XMStoreFloat3(&m_d3dxvMinimum, d3dxvMinimum);
		XMStoreFloat3(&m_d3dxvMaximum, d3dxvMaximum);
	}

	void Merge(XMFLOAT3 d3dxvMinimum, XMFLOAT3 d3dxvMaximum);
	void Merge(AABB *pAABB);
	void Update(XMMATRIX& pd3dxmtxTransform);
	BoundingBox GetBoudningBox();
};
#endif

//------------------------------------------------------------------------------------------------
class CMesh : public Object
{
public:
	CMesh(ID3D11Device* pd3dDevice);
	virtual ~CMesh();

	void Release();

	AABB							m_bcBoundingCube;
	BoundingBox						m_bcBoundingbox;

protected:
	D3D11_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology;

	UINT							m_nType;

	ID3D11Buffer					*m_pd3dPositionBuffer;

	/*?????????? ???? ???? ?????? ?????? ???? ????????
	???????? ??????(?????? ?????? ????)?? ??????.
	???????? ?????????? ???? ???? ??????
	?????? ?????? ???????? ????????.
	?????? ???????? ???????? ????????
	???????????? ???????? ???? ????????.*/
	ID3D11Buffer					**m_ppd3dVertexBuffers;

	//?????? ???????? ???? ?????? ?????? ????????. 
	int								m_nBuffers;

	//?????? ????????. 
	int								m_nVertices;
	UINT							m_nStartVertex;

	//???????? ???????????? ???????? ???? ???? ???? ????????. 
	UINT							m_nSlot;

	//?????? ???????? ???????? ???????? ?????? ?????? ???? ???????? ????????. 
	UINT							*m_pnVertexStrides;

	//?????? ???????? ???????? ???????? ???? ????(?????? ??)?? ???????? ????????. 
	UINT							*m_pnVertexOffsets;

	//?????? ????(???????? ????)?? ???? ?????????? ??????????. 
	ID3D11Buffer					*m_pd3dIndexBuffer;

	//?????? ?????? ???????? ???????? ????????. 
	UINT							m_nIndices;

	//?????? ???????? ?????? ???????? ???? ???????? ???? ??????????. 
	UINT							m_nStartIndex;

	//?? ???????? ?????? ??????????. 
	int								m_nBaseVertex;

	//?? ???????? ????(DXGI_FORMAT_R32_UINT ???? DXGI_FORMAT_R16_UINT)????. 
	DXGI_FORMAT						m_dxgiIndexFormat;
	UINT							m_nIndexOffset;

	XMFLOAT3						*m_pd3dxvPositions;
	UINT							*m_pnIndices;

	//?????? ????
	XMFLOAT3						 m_fSize;

	//------------------------------------------------------
	UINT		  m_nStride;
	UINT		  m_nOffset;
	ID3D11Buffer *m_pd3dVertexBuffer;
	//------------------------------------------------------

public:
	UINT GetType() { return (m_nType); }

	AABB GetBoundingCube() { return (m_bcBoundingCube); }

	BoundingBox GetBoundingBox() { return (m_bcBoundingbox); }

	XMFLOAT3 GetSize() const { return m_fSize; }

	//???? ???????? ????
	void CalculateBoundingCube();

	ID3D11Buffer *CreateBuffer(ID3D11Device* pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags);
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = nullptr, UINT *pnBufferStrides = nullptr, UINT *pnBufferOffsets = nullptr);

	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);
	virtual void RenderInstancedWithBlending(ID3D11DeviceContext *pd3dDeviceContext, int nInstances, int nStartInstance);

	int CheckRayIntersection(XMVECTOR* pd3dxvRayPosition, XMVECTOR* pd3dxvRayDirection, MESHINTERSECTINFO *pd3dxIntersectInfo);

	XMFLOAT3 GetMeshSize() { return m_fSize; }

	virtual void SizeUP(ID3D11DeviceContext* pd3dDeviceContext, float width) {};

	virtual void OffSetSize(ID3D11DeviceContext* pd3dDeviceContext, float x, float y, float z) {}

	virtual void SetSize(ID3D11DeviceContext* pd3dDeviceContext, float x, float y, float z) {}

};

//------------------------------------------------------------------------------------------------
class CMeshDiffused : public CMesh
{
public:
	CMeshDiffused(ID3D11Device* pd3dDevice);
	virtual ~CMeshDiffused();

protected:
	ID3D11Buffer					*m_pd3dColorBuffer;
};

class CNormalMesh : public CMesh
{
public:
	CNormalMesh(ID3D11Device* pd3dDevice);

	~CNormalMesh();

	void Release() override;

	XMVECTOR CalculateTriAngleTangent(XMFLOAT2* pd3dxvTexCoords, UINT nIndex0, UINT nIndex1, UINT nIndex2);

	XMVECTOR CalculateTriAngleTangent(UINT nIndex0, UINT nIndex1, UINT nIndex2);

	void SetAverageVertexTangent(XMVECTOR *pd3dxvTangents, int nPrimitives, int nOffset, bool bStrip);

	void SetTriAngleListVertexTangent(XMVECTOR *pd3dxvTangents);

	void CalculateVertexTangent(XMVECTOR *pd3dxvTangents);


protected:

	XMFLOAT2				*m_pTexCoords = nullptr;
	XMFLOAT3				*m_pNormals = nullptr;
	XMFLOAT3				*m_pTangents = nullptr;
	ID3D11Buffer			*m_pd3dTangentBuffer = nullptr;

};

//------------------------------------------------------------------------------------------------
class CMeshIlluminated : public CNormalMesh
{
public:
	CMeshIlluminated(ID3D11Device* pd3dDevice);
	virtual ~CMeshIlluminated();

protected:
	ID3D11Buffer					*m_pd3dNormalBuffer;

public:
	XMVECTOR& CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2);
	void SetTriAngleListVertexNormal(XMVECTOR *pd3dxvNormals);
	void SetAverageVertexNormal(XMVECTOR *pd3dxvNormals, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexNormal(XMVECTOR *pd3dxvNormals);
};

//------------------------------------------------------------------------------------------------
class CMeshTextured : public CMesh
{
public:
	CMeshTextured(ID3D11Device* pd3dDevice);
	virtual ~CMeshTextured();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshDetailTextured : public CMeshTextured
{
public:
	CMeshDetailTextured(ID3D11Device* pd3dDevice);
	virtual ~CMeshDetailTextured();

protected:
	ID3D11Buffer					*m_pd3dDetailTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshTexturedIlluminated(ID3D11Device* pd3dDevice);
	virtual ~CMeshTexturedIlluminated();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshDetailTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshDetailTexturedIlluminated(ID3D11Device* pd3dDevice);
	virtual ~CMeshDetailTexturedIlluminated();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
	ID3D11Buffer					*m_pd3dDetailTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CAirplaneMeshDiffused : public CMeshDiffused
{
public:
	CAirplaneMeshDiffused(ID3D11Device* pd3dDevice, float fWidth = 20.0f, float fHeight = 4.0f, float fDepth = 20.0f, XMVECTOR d3dxColor = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMeshDiffused();
};

//------------------------------------------------------------------------------------------------
class CCubeMeshDiffused : public CMeshDiffused
{
public:
	CCubeMeshDiffused(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f, XMVECTOR d3dxColor = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CCubeMeshDiffused();
};

//------------------------------------------------------------------------------------------------
class CSphereMeshDiffused : public CMeshDiffused
{
public:
	CSphereMeshDiffused(ID3D11Device* pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20, XMVECTOR d3dxColor = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CSphereMeshDiffused();
};

//------------------------------------------------------------------------------------------------
class CCubeMeshIlluminated : public CMeshIlluminated
{
public:
	CCubeMeshIlluminated(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshIlluminated();
};

class CSphereMeshIlluminated : public CMeshIlluminated
{
public:
	CSphereMeshIlluminated(ID3D11Device* pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshIlluminated();
};

class CCubeMeshTextured : public CMeshTextured
{
public:
	CCubeMeshTextured(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshTextured();
};

class CSphereMeshTextured : public CMeshTextured
{
public:
	CSphereMeshTextured(ID3D11Device* pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshTextured();
};

class CCubeMeshTexturedIlluminated : public CMeshTexturedIlluminated
{
public:
	CCubeMeshTexturedIlluminated(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshTexturedIlluminated();
};

class CSphereMeshTexturedIlluminated : public CMeshTexturedIlluminated
{
public:
	CSphereMeshTexturedIlluminated(ID3D11Device* pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshTexturedIlluminated();
};

//------------------------------------------------------------------------------------------------
class CHeightMapGridMesh : public CMeshDetailTexturedIlluminated
{
protected:
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_d3dxvScale;

public:
	CHeightMapGridMesh(ID3D11Device* pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), void *pContext = nullptr, D3D11_USAGE d3dUsage = D3D11_USAGE_DEFAULT);
	virtual ~CHeightMapGridMesh();

	XMVECTOR& GetScale() { return (XMLoadFloat3(&m_d3dxvScale)); }
	int GetWidth() { return (m_nWidth); }
	int GetLength() { return (m_nLength); }

	virtual float OnGetHeight(int x, int z, void *pContext);
};

//-------------------------------------------------------------------------------------------------
class CSkyBoxMesh : public CMeshTextured
{
public:
	CSkyBoxMesh(ID3D11Device* pd3dDevice, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f);
	virtual ~CSkyBoxMesh();
};

class CWaterGridMesh : public CHeightMapGridMesh
{
private:
	int							m_xStart;
	int							m_zStart;

	float						m_fK1;
	float						m_fK2;
	float						m_fK3;

	float						m_fTimeDelta;
	float						m_fSpatialDelta;
	float						m_fAccumulatedTime;

	XMFLOAT3					*m_pd3dxvPreviousPositions;
	XMFLOAT3					*m_pd3dxvCurrentPositions;
	XMFLOAT3					*m_pd3dxvTempPositions;
	XMFLOAT3					*m_pd3dxvNormalVectors;

public:
	CWaterGridMesh(ID3D11Device* pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), void *pContext = nullptr, D3D11_USAGE d3dUsage = D3D11_USAGE_DYNAMIC);
	virtual ~CWaterGridMesh();

	virtual float OnGetHeight(int x, int z, void *pContext);

	void SetDisturbingForce(int i, int j, float fMagnitude);
	void OnPrepareAnimation(float fTimeDelta, float fSpatialDelta, float fSpeed, float fDamping);

	virtual void Animate(ID3D11DeviceContext *pd3dDeviceContext, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

//--------------------------------------------------------------------------------------------------
class CTextureToScreenRectMesh : public CMeshTextured
{
public:
	CTextureToScreenRectMesh(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f);
	virtual ~CTextureToScreenRectMesh();
};

//------------------------------------------------------------------------------------------------
class CTexturedRectMesh : public CMeshTextured
{
public:
	CTexturedRectMesh(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fLength = 2.0f);
	virtual ~CTexturedRectMesh();
};


//------------------------------------------------------------------------------------------------
//KYT '16.01.26 plus 
/*FrustumMesh Mesh*/
//------------------------------------------------------------------------------------------------

class C3DFrustumMeshDiffused : public CMeshTextured
{
private:
	BoundingBox			m_boundingbox;


public:
	C3DFrustumMeshDiffused(ID3D11Device* pd3dDevice, float fFar, float fFov);

	virtual ~C3DFrustumMeshDiffused();

	BoundingBox GetFrustumBoundingBox()
	{
		return m_boundingbox;
	}


};

//------------------------------------------------------------------------------------------------
