#include "stdafx.h"
#include "HeadBangBangShader.h"
#include "BillBoardShader.h"
#include "QuadrangleMesh.h"
#include "PlayerHeadBangBang.h"
#include "MonsterState.h"
#include "MonsterHeadBangBang.h"

CHeadBangBangShader::CHeadBangBangShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(nObjects)
{
}

CHeadBangBangShader::CHeadBangBangShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(0)
{
}

CHeadBangBangShader::CHeadBangBangShader(int nObjects) : CInstancedObjectsShader(nObjects)
{
}

CHeadBangBangShader::~CHeadBangBangShader()
{
	m_pBillBoardShader.reset();
}

void CHeadBangBangShader::SetPlayer(CPlayer* player)
{
	m_pPlayer = nullptr;
	if(player) m_pPlayer = player;
}

void CHeadBangBangShader::CreateShader(ID3D11Device* pd3dDevice)
{
	CInstancedObjectsShader::CreateShader(pd3dDevice);

	m_pPlayer = nullptr;
	m_pBillBoardShader = std::make_unique<CBillBoardShader>();
	m_pBillBoardShader->CreateShader(pd3dDevice);
	m_pBillBoardShader->SetMesh(move(std::make_unique<CQuadrangleMesh>(pd3dDevice, 0.5, 0.5)));
	m_pBillBoardShader->SetMaterial(pd3dDevice, "../Assets/03_Image/Alpha/alpha.png");
	m_pBillBoardShader->SetBlendState("UI_Translucence");
}

void CHeadBangBangShader::RenderPlayer(ID3D11DeviceContext * pd3dDeviceContext)
{
	CInstancedObjectsShader::RenderNoClear(pd3dDeviceContext, nullptr);
}

void CHeadBangBangShader::BuildObjects(ID3D11Device* pd3dDeivce)
{
	CInstancedObjectsShader::BuildObjects(pd3dDeivce);
	m_pBillBoardShader->BuildObjects(pd3dDeivce);

}

void CHeadBangBangShader::RenderNoClear(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	CInstancedObjectsShader::RenderNoClear(pd3dDeviceContext, pCamera);
	
	if (m_pPlayer)
	{
		XMFLOAT3 f3 = m_pPlayer->GetPositionXMFLOAT3();
		f3.y += BLOCK_SCALE;
		m_pBillBoardShader->SetPosition(f3);
	}
	m_pBillBoardShader->Render(pd3dDeviceContext, pCamera);
}

void CHeadBangBangShader::RenderMonster(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
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
	
		auto pos = static_cast<MonsterHeadBangBang*>(pObject->GetComponent(0))->GetMonsterPosition();
		pos.y += BLOCK_SCALE * 1.3f;
		m_pBillBoardShader->SetPosition(pos);

	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	

	m_pBillBoardShader->Render(pd3dDeviceContext, pCamera);

}

void CHeadBangBangShader::AddHeadBangBang(XMFLOAT3 xmPivotPosition, MonsterState* state)
{
	std::shared_ptr<CGameObject> headObject = std::make_shared<CGameObject>();
	auto script = std::make_unique<MonsterHeadBangBang>();
	script->SetGameObject(headObject);
	script->SetMonsterState(state);
	xmPivotPosition.y *= 1.2f;
	script->SetPosition(xmPivotPosition);
	script->Start(headObject);

	headObject->SetPosition(xmPivotPosition);
	headObject->SetCompoent(move(script));
	m_lRenderObject.push_back(headObject);

}

void CHeadBangBangShader::AnimateHeadBangBang(float fTimeElapsed)
{
	for (auto &pObject : m_lRenderObject)
	{
		pObject->GetComponent(0)->Animate(fTimeElapsed);
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
