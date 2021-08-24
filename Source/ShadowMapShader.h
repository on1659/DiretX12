#pragma once

#include "MatrixShader.h"


struct ShadowsMap_VS_INPUT
{
	XMFLOAT4	d3dxPosition;
	XMFLOAT2	d3dxTextCoord;
};

class CShadowMapShader : public CMatrixShader
{
private:
	XMFLOAT4							m_d3dxPosition[4];
	ID3D11Buffer						*m_pd3dVertexBuffer;
	UINT								*m_pnVertexStrides;
	UINT								*m_pnVertexOffsets;
	int									 m_nMaxCount;
	int									 m_nCount;

	XMMATRIX							 m_xmDefaultMatrix;

	
public:
	
	CShadowMapShader(std::unique_ptr<CMesh> pMesh, int nObjects);
	
	CShadowMapShader(std::unique_ptr<CMesh> pMesh);
	
	CShadowMapShader(int nObjects = 1);

	~CShadowMapShader();
	
	void CreateShader(ID3D11Device *pd3dDevice);
	
	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera = nullptr);

	void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera = nullptr);

	void SetFirstPosition(const float& x, const  float& y, const  float& z);

	void SetPosition(const XMFLOAT3& position);

	void SetPosition(const XMVECTOR& position);

	void SetPosition_Ver2(const XMFLOAT3& position);

};

