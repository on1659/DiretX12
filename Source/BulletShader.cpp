#include "stdafx.h"
#include "BulletShader.h"
#include "GameObject.h"
#include "BulletComponent.h"
#include "stdafx.h"
#include "MonsterComponent.h"

CBulletShader::CBulletShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(move(pMesh), nObjects)
{
	m_vMtxWorld.clear();
	m_bConstBufferUpdate = false;
}

CBulletShader::CBulletShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{
	m_bConstBufferUpdate = false;
}

CBulletShader::CBulletShader(int nObjects) : CInstancedObjectsShader(nObjects)
{
	m_bConstBufferUpdate = false;
}

CBulletShader::~CBulletShader()
{

}

void CBulletShader::ReleaseObjects()
{
	CInstancedObjectsShader::ReleaseObjects();

	for (auto &pObject : m_lGameObject)
	{
		pObject->Release();
		pObject->ReleaseConstBuffers();
	}
	m_lGameObject.clear();
}

void CBulletShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_bConstBufferUpdate)
	{
		m_lGameObject.remove_if
		(
			[]
			(const std::shared_ptr<CGameObject>& object)
			{
				return (object->GetComponetSize() == 0);
			}
		);

		if (m_lGameObject.size() <= 0)
		{
			m_bConstBufferUpdate = false;
			return;
		}
	
	}
	else
		return;

	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	for (auto &pObject : m_lGameObject)
	{
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);


	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	
	if( m_pMesh) m_pMesh->RenderInstancedWithBlending(pd3dDeviceContext, nInstances, 0);
	
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);

}

void CBulletShader::AddBullet(std::shared_ptr<CGameObject> gameObject)
{
	m_lGameObject.push_back(gameObject);
	m_bConstBufferUpdate = true;
}

void CBulletShader::DestoryBullet(std::shared_ptr<CGameObject> gameObject)
{
	auto findobject = std::find(m_lGameObject.begin(), m_lGameObject.end(), gameObject);
	m_lGameObject.erase(findobject);
	m_lGameObject.pop_front();
	SPACE->DynamicDestory(gameObject);
}

void CBulletShader::AnimateBullet(float fTimeElapsed)
{
	for (auto &pBullet : m_lGameObject)
	{
		pBullet->Animate(fTimeElapsed);

		CSpaceNodeQuad* room = pBullet->GetSpaceNodeQuad();
		if (nullptr == room)continue;

		auto vDynamicObject = room->GetDynamicObjectsContainer();
		for (auto &pObject : vDynamicObject)
		{
			if (pObject->tag == TAG_TYPE::TAG_BUNNY_MONSTER)
			{
				pBullet->IntersectsDynamic(pObject);
				//static_cast<MonsterComponent*>(pObject->GetComponent(1))->Collision(pBullet, ObjectType::BulletType);
			}
		}
	}

	bool check = false;
	m_lGameObject.remove_if(
	[&](shared_ptr<CGameObject>& object)
	{
		if (object->IsActive() == false)
		{
			SPACE->DynamicDestory(object);
			check = true;
			return true;
		}
		return false;
	}
	);
}
