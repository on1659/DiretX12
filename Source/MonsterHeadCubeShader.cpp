#include "stdafx.h"
#include "MonsterHeadCubeShader.h"


CMonsterHeadCubeShader::CMonsterHeadCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(std::move(pMesh), nObjects)
{
	m_fRenderObjectWorldMatrix.clear();
	Helper::RuntimeDataModify(&fHeight, "f_HeadCubeHeight");
}
CMonsterHeadCubeShader::CMonsterHeadCubeShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(std::move(pMesh))
{
	m_fRenderObjectWorldMatrix.clear();
	Helper::RuntimeDataModify(&fHeight, "f_HeadCubeHeight");
}
CMonsterHeadCubeShader::CMonsterHeadCubeShader(int nObjects) : CInstancedObjectsShader(nObjects)
{
	m_fRenderObjectWorldMatrix.clear();
	Helper::RuntimeDataModify(&fHeight, "f_HeadCubeHeight");
}

CMonsterHeadCubeShader::~CMonsterHeadCubeShader()
{
}

void CMonsterHeadCubeShader::AddPosition(const XMFLOAT3& xmPosition)
{
	AddPosition(xmPosition.x, xmPosition.y, xmPosition.z);
}

void CMonsterHeadCubeShader::AddPosition(const float& x, const float& y, const float& z)
{
	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, XMMatrixIdentity());
	mtx._41 = x;
	mtx._42 = fHeight;
	mtx._43 = z;
	m_fRenderObjectWorldMatrix.push_back(XMLoadFloat4x4(&mtx));
}

void CMonsterHeadCubeShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{

	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	//KYT '16.01.25 minus
	/*
	Shader에서 Object Vector 가지고 있는걸 포기한다.
	고로 Render도 변해야 한다.
	*/
	for (auto matrix : m_fRenderObjectWorldMatrix)
	{
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(matrix);

		XMFLOAT4X4 ff;
		XMStoreFloat4x4(&ff, matrix);
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);

	m_fRenderObjectWorldMatrix.clear();
}
