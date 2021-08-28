//-----------------------------------------------------------------------------
// File: Shader.h
//-----------------------------------------------------------------------------

#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Player.h"

#define _WITH_INSTANCING_CULLING
#ifdef _WITH_INSTANCING_CULLING
#define _WITH_INSTANCING_FROobjectS_DYNAMICALLY
#endif

struct VS_CB_WORLD_MATRIX
{
	XMFLOAT4X4						m_d3dxmtxWorld;
};

struct VS_VB_INSTANCE
{
	XMFLOAT4X4						m_d3dxTransform;
};

//KYT '16.01.24 plus_03
/* Particle�� ����ϱ� ���� Info */
struct SO_Geometry_INFO
{
	D3D11_SO_DECLARATION_ENTRY * pSODecls;
	UINT NumEntries;
	UINT *pBufferStrides;
	UINT NumStrides;
	UINT RasterizedStream;
};


class CShader : public Object
{
public:
	CShader();

	//�ȵǳ�
	template<class _Ty> inline
	CShader(_Ty&& other) noexcept (is_nothrow_move_constructible<_Ty>::value&& is_nothrow_move_assignable<_Ty>::value)
	{
		this = move(other);
	}


	virtual ~CShader();

protected:
	UINT							m_nType;

	int								m_nInputElements;

	D3D11_INPUT_ELEMENT_DESC		*m_pd3dInputElementDescs;

	ID3D11InputLayout				*m_pd3dVertexLayout;		//Vertex -LayOut
	ID3D11VertexShader				*m_pd3dVertexShader;		//Vertex -Shader
	ID3D11GeometryShader			*m_pd3dGeometryShader;		//Geo   -Shader
	ID3D11VertexShader				*m_pd3dSOVertexShader;		//Sov	- Shader
	ID3D11PixelShader				*m_pd3dPixelShader;			//Pixel -Shader
	ID3D11GeometryShader			*m_pd3dSOGeometryShader;	//SOGeo - Shader
	ID3D11DomainShader				*m_pd3dDomainShader;		//Domain- Shader
	ID3D11HullShader				*m_pd3dHullShader;			//Hull   - SHader
	ID3D11ComputeShader				*m_pd3dComputeShader;		//Compute   - SHader

protected:
	void CreateVertexShaderFromFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputElements, UINT nElements, ID3D11InputLayout **ppd3dInputLayout);
	void CreatePixelShaderFromFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);
	void CreateGeometryShaderFromFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader);
	void CreateSOGeometryShaderFromFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader);
	void CreateSOGeometryShaderFromFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader, SO_Geometry_INFO sgi);
	void CreateHullShaderFromFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11HullShader **ppd3dHullShader);
	void CreateDomainShaderFromFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11DomainShader **ppd3dDomainShader);
	void CreateComputeShaderFromFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11ComputeShader **ppd3dComputeShader);

	//KYT 16.08.06 
	/*
		������ �� ���̴��� ����ϱ� ���� �߰�.
										feat.����
	*/
	void CreateVertexShaderFromCompiledFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11VertexShader **ppd3dVertexShader);
	void CreateSOVertexShaderFromCompiledFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11VertexShader **ppd3dVertexShader, ID3D11InputLayout *ppInputLayout);
	void CreateGeoMoetryShaderFromCompiledFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, ID3D11GeometryShader **ppd3dGeomeotryShader);
	void CreatePixelShaderFromCompiledFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, ID3D11PixelShader **ppd33dPixelShader);
	void CreateComputeShaderFromCompiledFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, ID3D11ComputeShader **ppd33dComputeShader);
	void CreateDomainShaderFromCompiledFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, ID3D11DomainShader **ppd33dPixelShader);
	void CreateHullShaderFromCompiledFile(ID3D11Device* pd3dDevice, WCHAR *pszFileName, ID3D11HullShader **ppd33dHullShader);




public:
	virtual void GetInputElementDesc(UINT nVertexElementType);
	virtual void GetShaderName(UINT nVertexElementType, LPCSTR *ppszVSShaderName, LPCSTR *ppszVSShaderModel, LPCSTR *ppszPSShaderName, LPCSTR *ppszPSShaderModel);

	virtual void CreateShader(ID3D11Device* pd3dDevice) { }
	virtual void CreateShader(ID3D11Device* pd3dDevice, UINT nType);
	virtual void CreateShader(ID3D11Device* pd3dDevice, D3D11_INPUT_ELEMENT_DESC *pd3dInputElementDesc, int nInputElements, WCHAR *pszFileName, LPCSTR pszVSShaderName, LPCSTR pszVSShaderModel, LPCSTR pszPSShaderName, LPCSTR pszPSShaderModel);

	virtual void CreateConstBuffers(ID3D11Device* pd3dDevice);
	virtual void ReleaseShaderVariables();

	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

public:
	static ID3D11Buffer *CreateBuffer(ID3D11Device* pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags);


protected:
	// �������� ������
	// �� �����Ӹ��� �� ���Ϳ� �������� ������Ʈ�� ������Ʈ ���ش�. 
	std::vector<std::shared_ptr<CGameObject>>		m_vRenderObject;

public:

	//KYT '15.01.29 
	std::vector<std::shared_ptr<CGameObject>> GetvRenderObject() { return m_vRenderObject; }

	void PushRenderObject(std::shared_ptr<CGameObject> object = nullptr);

	void ClearRenderObject(){ m_vRenderObject.clear(); }

	virtual void SpacePartitioningRender(ID3D11DeviceContext *pd3dDeviceContext,std::shared_ptr<CCamera> pCamera);
	
	CMesh* GetMesh(){ return m_pMesh.get(); }

	AABB GetAABB()
	{
		if (m_pMesh)
			return m_pMesh->GetBoundingCube();
		else
		{
			return AABB();
		}
	}

protected:
	std::unique_ptr<CMesh>			m_pMesh;

};

class CObjectsShader : public CShader
{
public:
	CObjectsShader(int nObjects = 1);
	virtual ~CObjectsShader();

protected:
	//std::vector<std::
	//	shared_ptr<CGameObject>>	m_vsObjects;
	////CGameObject						**m_ppObjects;
	int								m_nObjects;

	//KYT '16.01.20 plus -> 16'01.25 minus
	/* Default Object�� ����ϴ��� ���ϴ��� �Ǵ�.
	   true = ��� / false = �̻��
	   -> Shader Object Vector ������ ���� �������� ����
	*/
	//bool							m_bIsDefaultUseObject;

	std::shared_ptr<CMaterial>		m_sMaterial;

	void							*m_pContext;

private:
	int								m_nIndexToAdd;

public:
	virtual void BuildObjects(ID3D11Device* pd3dDevice, void *pContext = nullptr);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	virtual void SetPosition(float x, float y, float z){}


	//KYT '16.01.25 minus
	/* 
	 Shader���� Object Vector ������ �ִ°� �����Ѵ�.
	*/
	//void SetObject(std::shared_ptr<CGameObject> object) { m_vsObjects[0] = object; }
	//void SetObject(int nIndex, std::shared_ptr<CGameObject> pGameObject);
	
	void AddObject(std::shared_ptr<CGameObject> pGameObject);

	//void SetEnviornmentObject(int nIndex, std::shared_ptr<CGameObject> pGameObject);
	//void AddEnviornmentObject(std::shared_ptr<CGameObject> pGameObject);

	void SetMaterial(std::shared_ptr<CMaterial> pMaterial);
	CGameObject *PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);

	//KYT '16.01.21 plus
	/*
	  Material�� ���� �� ���� �߰��� �� �ְ� �����Ͽ���.
	  Į��� �⺻������ �ؽ��� Į�� �״�� ������ ����.
	*/
	void SetMaterial(ID3D11Device* pd3dDevice, std::string path, int nStartSampler = PS_SLOT_SAMPLER, int nCountSampler = 1, YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT);

	void SetMaterial(ID3D11Device* pd3dDevice, std::vector<std::string> vPath, int nStartSampler = PS_SLOT_SAMPLER, int nCountSampler = 1, YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT);

	void SetUIMaterial(ID3D11Device* pd3dDevice, std::string path, int nStartSampler = PS_SLOT_SAMPLER, int nCountSampler = 1, YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT);

	void SetMaterial_NormalNBump(ID3D11Device* pd3dDevice, std::string texture, std::string normal, std::string bump);


	//SpaceParitioning
	//virtual void SpacePartitioningRender(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	//virtual void SpacePartitioningFrustm(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);


};
