#include "stdafx.h"
#include "TrapShader.h"


CTrapShader::CTrapShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CHeadBangBangShader(std::move(pMesh), nObjects)
{
}
CTrapShader::CTrapShader(std::unique_ptr<CMesh> pMesh) : CHeadBangBangShader(std::move(pMesh))
{
}
CTrapShader::CTrapShader(int nObjects) : CHeadBangBangShader(nObjects)
{
}

CTrapShader::~CTrapShader()
{
}

void CTrapShader::RenderTrap(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);
	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;
	for (auto &pObject : m_lRenderObject)
	{
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);


}

void CTrapShader::AnimateTrap(float fTimeElapsed)
{
	for (auto &pObject : m_lRenderObject)
	{
		//MonsterHeadBangBang* component = (MonsterHeadBangBang*)(pObject->GetComponent(0));
		//component->Animate(fTimeElapsed);
		//if (component->GetTime() > 3.0f)
		//	pObject->SetActive(false);
	}

	m_lRenderObject.remove_if
	(
		[](const auto& pObject)
	{
		if (false == pObject->IsActive())
		{
			pObject->Release();
			return true;
		}
		else
			return false;
	}
	);
}
