//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"
#include "Object.h"

#include "ItemBasic.h"
#include "PlayerState.h"

ID3D11Buffer *CGameObject::m_pd3dcbWorldMatrix = nullptr;
ID3D11Buffer *CGameObject::m_pd3dcbMaterialColors = nullptr;
XMMATRIX CGameObject::m_d3dxmtxIdentity;

CGameObject::CGameObject(int nMeshes) : Object()
{

	//KYT '16.07.26
	/*
		GameObject Global ID;
	*/
	nGID = gInstanceID++;

	//XMStoreFloat4x4(&m_d3dxmtxLocal, XMMatrixIdentity());
	XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixIdentity());

	m_d3dxmtxIdentity = XMMatrixIdentity();

	//m_pShader = nullptr;
	//m_pMaterial = nullptr;

	//m_nMeshes = nMeshes;
	//m_ppMeshes = nullptr;

	m_sShader = nullptr;
	m_vsMeshes.reserve(nMeshes);

	//if (m_nMeshes > 0) m_ppMeshes = new CMesh*[m_nMeshes];
	//for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = nullptr;

	m_bcMeshBoundingCube = AABB();
	m_bcMeshBoundingbox = BoundingBox();

	m_bActive = true;
	m_bIsVisible = true;

	m_vChild.clear();
	m_pParent = nullptr;

	//Component
	m_vComponent.clear();

	//InitC
	CGameObject::Initialize();

	m_movementSpeed = CHARACTER_SPEED * BLOCK_SCALE;
	m_currentKeyframe = 0;
	m_animPlaySpeed = PLAYER_ANISPEED;
	m_currentAnimationState = AnimState::ANI_IDLE;

	SetQuaternion(XMQuaternionRotationMatrix(GetWorldMatrix()));

}

CGameObject::CGameObject(std::string name, UINT tag) : Object(name, tag)
{
	//KYT '16.07.26
	/*
	GameObject Global ID;
	*/
	nGID = gInstanceID++;


	//XMStoreFloat4x4(&m_d3dxmtxLocal, XMMatrixIdentity());
	XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixIdentity());

	m_d3dxmtxIdentity = XMMatrixIdentity();

	//m_pShader = nullptr;
	//m_pMaterial = nullptr;

	//m_nMeshes = nMeshes;
	//m_ppMeshes = nullptr;

	m_sShader = nullptr;
	m_vsMeshes.reserve(1);

	//if (m_nMeshes > 0) m_ppMeshes = new CMesh*[m_nMeshes];
	//for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = nullptr;

	m_bcMeshBoundingCube = AABB();
	m_bcMeshBoundingbox = BoundingBox();

	m_bActive = true;
	m_bIsVisible = true;

	m_vChild.clear();
	m_pParent = nullptr;

	//Component
	m_vComponent.clear();

	//InitC
	CGameObject::Initialize();

	m_movementSpeed = CHARACTER_SPEED * BLOCK_SCALE;
	m_currentKeyframe = 0;
	m_animPlaySpeed = PLAYER_ANISPEED;
	m_currentAnimationState = AnimState::ANI_IDLE;
}

CGameObject::~CGameObject()
{
	//우태질문 정말로 삭제 안해도 되나?
	//if (m_pShader) m_pShader->Release();

	//if (m_pMaterial) m_pMaterial->Release();

	//if (m_ppMeshes)
	//{
	//	for (int i = 0; i < m_nMeshes; i++)
	//	{
	//		if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
	//		m_ppMeshes[i] = nullptr;
	//	}
	//	delete[] m_ppMeshes;
	//}
}

void CGameObject::Release()
{
	Object::Release();


	if (m_pParent) m_pParent = nullptr;

	for (auto &object : m_vChild)
	{
		//if (object)delete object;
		object = nullptr;
	}
	m_vChild.clear();

	for (auto &componet : m_vComponent)
	{
		componet->Destory();
		componet->Release();
		componet = nullptr;
	}
	m_vComponent.clear();

	if (m_sMaterial.use_count())m_sMaterial = nullptr;
	if (m_sShader.use_count())m_sShader = nullptr;

}


//Create
/*WorldMatrix , MaterialClor Matrix*/
void CGameObject::CreateConstBuffers(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pd3dcbWorldMatrix);

	d3dBufferDesc.ByteWidth = sizeof(XMVECTOR) * 4;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pd3dcbMaterialColors);

	//KYT '16.01.24 plus 
	/* GS 추가 */
	ID3D11DeviceContext* pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);

	//GSShader
	pd3dDeviceContext->GSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);

	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_MATERIAL, 1, &m_pd3dcbMaterialColors);
	pd3dDeviceContext->Release();
}

void CGameObject::Initialize()
{
	m_bActive = true;

	//KYT '16.02.07
	/*생성자 수정*/

#ifdef _QURD_ARRARY
	m_SpaceNodeQuad = nullptr;
#else
	m_SpaceNode = nullptr;
#endif
	SetQuaternion(XMQuaternionRotationMatrix(GetWorldMatrix()));

}


//Set Functino
void CGameObject::SetMesh(std::unique_ptr<CMesh> pMesh, int nIndex)
{
	if (pMesh)
	{
#ifdef _AABB_
		AABB bcBoundingCube;
		bcBoundingCube = pMesh->GetBoundingCube();
		m_bcMeshBoundingCube.Merge(&bcBoundingCube);

		m_bcMeshBoundingbox.Center.x = (bcBoundingCube.m_d3dxvMaximum.x + bcBoundingCube.m_d3dxvMinimum.x) * 0.5f;
		m_bcMeshBoundingbox.Center.y = (bcBoundingCube.m_d3dxvMaximum.y + bcBoundingCube.m_d3dxvMinimum.y) * 0.5f;
		m_bcMeshBoundingbox.Center.z = (bcBoundingCube.m_d3dxvMaximum.z + bcBoundingCube.m_d3dxvMinimum.z) * 0.5f;

		m_bcMeshBoundingbox.Extents.x = (bcBoundingCube.m_d3dxvMaximum.x - bcBoundingCube.m_d3dxvMinimum.x) * 0.5f;
		m_bcMeshBoundingbox.Extents.y = (bcBoundingCube.m_d3dxvMaximum.y - bcBoundingCube.m_d3dxvMinimum.y) * 0.5f;
		m_bcMeshBoundingbox.Extents.z = (bcBoundingCube.m_d3dxvMaximum.z - bcBoundingCube.m_d3dxvMinimum.z) * 0.5f;

#else
		XMFLOAT3 xmMin, xmMax; // 기존값
		XMFLOAT3 input_xmMin, input_xmMax;		//새로 들어온거
		XMFLOAT3 fCenter, fExtern;

		fCenter = pMesh->GetBoundingCube().Center;
		fExtern = pMesh->GetBoundingCube().Extents;

		XMStoreFloat3(&input_xmMin, XMVectorSubtract(XMLoadFloat3(&fCenter), XMLoadFloat3(&fExtern)));
		XMStoreFloat3(&input_xmMax, XMVectorAdd(XMLoadFloat3(&fCenter), XMLoadFloat3(&fExtern)));

		XMStoreFloat3(&xmMin, XMVectorSubtract(XMLoadFloat3(&m_bcMeshBoundingCube.Center), XMLoadFloat3(&m_bcMeshBoundingCube.Extents)));
		XMStoreFloat3(&xmMax, XMVectorAdd(XMLoadFloat3(&m_bcMeshBoundingCube.Center), XMLoadFloat3(&m_bcMeshBoundingCube.Extents)));

		if (input_xmMin.x < xmMin.x) xmMin.x = input_xmMin.x;
		if (input_xmMin.y < xmMin.y) xmMin.y = input_xmMin.y;
		if (input_xmMin.z < xmMin.z) xmMin.z = input_xmMin.z;
		if (input_xmMax.x > xmMax.x) xmMax.x = input_xmMax.x;
		if (input_xmMax.y > xmMax.y) xmMax.y = input_xmMax.y;
		if (input_xmMax.z > xmMax.z) xmMax.z = input_xmMax.z;




		//XMStoreFloat3(&m_bcMeshBoundingCube.Center, XMVectorSubtract(XMLoadFloat3(&input_xmMax), XMLoadFloat3(&input_xmMin)) * 0.5f);
		//XMStoreFloat3(&m_bcMeshBoundingCube.Extents, XMVectorAdd(XMLoadFloat3(&input_xmMax), XMLoadFloat3(&input_xmMin)) * 0.5f);

		XMStoreFloat3(&m_bcBoundingCube.Center, XMVectorSubtract(XMLoadFloat3(&input_xmMax), XMLoadFloat3(&input_xmMin)) * 0.5f);
		XMStoreFloat3(&m_bcBoundingCube.Extents, XMVectorAdd(XMLoadFloat3(&input_xmMax), XMLoadFloat3(&input_xmMin)) * 0.5f);


		/*
		XMVECTOR xCenter = XMLoadFloat3(&m_bcMeshBoundingCube.Center);
		XMVECTOR xExtern = XMLoadFloat3(&m_bcMeshBoundingCube.Extents);

		XMFLOAT3 xmMin;// = m_bcMeshBoundingCube.Center;
		XMFLOAT3 xmMax;// = m_bcMeshBoundingCube.Extents;


		XMStoreFloat3(&xmMin, XMVectorSubtract(xCenter, xExtern));
		XMStoreFloat3(&xmMax, XMVectorAdd(xCenter, xExtern));


		XMFLOAT3  xmfCenter = pMesh->GetBoundingCube().Center;
		XMFLOAT3 xmfExtern = pMesh->GetBoundingCube().Extents;

		XMLoadFloat3(&xmfCenter);
		XMLoadFloat3(&xmfExtern);

		XMFLOAT3 mesh_Min, mesh_mMax;
		XMStoreFloat3(&mesh_Min, XMVectorSubtract(xCenter, xExtern));
		XMStoreFloat3(&mesh_mMax, XMVectorAdd(xCenter, xExtern));

		if (mesh_Min.x < xmMin.x) xmMin.x = mesh_Min.x;
		if (mesh_Min.y < xmMin.y) xmMin.y = mesh_Min.y;
		if (mesh_Min.z < xmMin.z) xmMin.z = mesh_Min.z;
		if (mesh_mMax.x > xmMax.x) xmMax.x = mesh_mMax.x;
		if (mesh_mMax.y > xmMax.y) xmMax.y = mesh_mMax.y;
		if (mesh_mMax.z > xmMax.z) xmMax.z = mesh_mMax.z;


		m_bcBoundingCube.Center = xmMin;
		m_bcBoundingCube.Extents = xmMax;
		*/

		//BoundingBox::CreateMerged(m_bcMeshBoundingCube, m_bcMeshBoundingCube, pMesh->GetBoundingCube());
#endif
	}

	if (nIndex >= m_vsMeshes.size())
		m_vsMeshes.push_back(move(pMesh));
	else if (m_vsMeshes[nIndex])
		m_vsMeshes[nIndex] = move(pMesh);

}

void CGameObject::SetMesh(CMesh* pMesh)
{
	if (pMesh)
	{
		AABB bcBoundingCube;
		bcBoundingCube = pMesh->GetBoundingCube();
		m_bcMeshBoundingCube.Merge(&bcBoundingCube);

		m_bcMeshBoundingbox.Center.x = (bcBoundingCube.m_d3dxvMaximum.x + bcBoundingCube.m_d3dxvMinimum.x) * 0.5f;
		m_bcMeshBoundingbox.Center.y = (bcBoundingCube.m_d3dxvMaximum.y + bcBoundingCube.m_d3dxvMinimum.y) * 0.5f;
		m_bcMeshBoundingbox.Center.z = (bcBoundingCube.m_d3dxvMaximum.z + bcBoundingCube.m_d3dxvMinimum.z) * 0.5f;

		m_bcMeshBoundingbox.Extents.x = (bcBoundingCube.m_d3dxvMaximum.x - bcBoundingCube.m_d3dxvMinimum.x) * 0.5f;
		m_bcMeshBoundingbox.Extents.y = (bcBoundingCube.m_d3dxvMaximum.y - bcBoundingCube.m_d3dxvMinimum.y) * 0.5f;
		m_bcMeshBoundingbox.Extents.z = (bcBoundingCube.m_d3dxvMaximum.z - bcBoundingCube.m_d3dxvMinimum.z) * 0.5f;
	}
}

//KYT '16.04.22
/*
Shader를 Set하면서 BoundingBox도 같이 너어둠
*/
void CGameObject::SetShader(std::shared_ptr<CShader> sShader, bool boundingbox)
{
	if (sShader == nullptr)return;
	if (boundingbox)SetBoundingBox(sShader->GetMesh());
	m_sShader = sShader;
	//SetMesh(sShader->GetMesh());
}

void CGameObject::SetMaterial(std::shared_ptr<CMaterial> sMaterial)
{
	m_sMaterial = sMaterial;
}

//KYT '16.06.25
/*
Parent - Child 연결
*/
void CGameObject::SetChild(std::shared_ptr<CGameObject> pChild)
{
	m_vChild.push_back(pChild.get());
	pChild->SetParent(this);
	SPACE->AddChlidObject(pChild);
}

void CGameObject::SetChild(CGameObject* pChild)
{
	m_vChild.push_back(pChild);
	pChild->SetParent(this);
}

void CGameObject::SetParent(CGameObject* pParent)
{
	m_pParent = pParent;
}

void CGameObject::SetShadowMatrix(XMVECTOR d3dxvLight, XMVECTOR d3dxPlane)
{
	XMStoreFloat4x4(&m_d3dxmtxShadow, XMMatrixShadow(d3dxPlane, d3dxvLight));
}

//Destory
void CGameObject::ReleaseConstBuffers()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterialColors) m_pd3dcbMaterialColors->Release();
}

//Map & UnMap
void CGameObject::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX& pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	XMStoreFloat4x4(&pcbWorldMatrix->m_d3dxmtxWorld, XMMatrixTranspose(pd3dxmtxWorld));

	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);

	//GeoShader
	pd3dDeviceContext->GSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

//Matteial -> Map UnMap
void CGameObject::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, CMaterialColors* pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterialColors, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	D3DXCOLOR *pcbMaterial = (D3DXCOLOR *)d3dMappedResource.pData;
	memcpy(pcbMaterial, &pMaterial->m_d3dxcAmbient, sizeof(D3DXCOLOR));
	memcpy(pcbMaterial + 1, &pMaterial->m_d3dxcDiffuse, sizeof(D3DXCOLOR));
	memcpy(pcbMaterial + 2, &pMaterial->m_d3dxcSpecular, sizeof(D3DXCOLOR));
	memcpy(pcbMaterial + 3, &pMaterial->m_d3dxcEmissive, sizeof(D3DXCOLOR));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterialColors, 0);
	//쑤정 
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_MATERIAL, 1, &m_pd3dcbMaterialColors);
}

//View
bool CGameObject::IsVisible(std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender();
	Update(nullptr);

	m_bIsVisible = (m_bActive) ? true : false;

#ifdef _WITH_FRUSTUM_CULLING_BY_OBJECT
	if (m_bActive)
	{
#ifdef _AABB_
		AABB bcBoundingCube = m_bcMeshBoundingCube;
		bcBoundingCube.Update(XMLoadFloat4x4(&m_d3dxmtxWorld));
		//if (pCamera) m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
		if (pCamera)m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);

#else	
		BoundingBox bcBoundingCube = m_bcMeshBoundingCube;
		m_bcMeshBoundingCube.Transform(bcBoundingCube, XMLoadFloat4x4(&m_d3dxmtxWorld));
		if (pCamera) m_bIsVisible = pCamera->IsInFrustum(&m_bcMeshBoundingCube);
#endif

		//if (m_pChild) m_bIsVisible |= m_pChild->IsVisible(pCamera);
	}
#endif
	return (m_bIsVisible);
}

bool CGameObject::IsVisible(std::shared_ptr<CCamera> pCamera, XMVECTOR camPosition)
{
	OnPrepareRender();
	Update(nullptr);

	m_bIsVisible = (m_bActive) ? true : false;
	XMFLOAT3 xm3;
	XMStoreFloat3(&xm3, XMVector3Length(camPosition - GetPosition()));
	if (xm3.x > CULL_CAMERA_DISTANCE)return false;

#ifdef _WITH_FRUSTUM_CULLING_BY_OBJECT
	if (m_bActive)
	{
#ifdef _AABB_
		AABB bcBoundingCube = m_bcMeshBoundingCube;
		bcBoundingCube.Update(XMLoadFloat4x4(&m_d3dxmtxWorld));
		//if (pCamera) m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
		if (pCamera)m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);

#else	
		BoundingBox bcBoundingCube = m_bcMeshBoundingCube;
		m_bcMeshBoundingCube.Transform(bcBoundingCube, XMLoadFloat4x4(&m_d3dxmtxWorld));
		if (pCamera) m_bIsVisible = pCamera->IsInFrustum(&m_bcMeshBoundingCube);
#endif

		//if (m_pSibling) m_bIsVisible |= m_pSibling->IsVisible(pCamera);
		//if (m_pChild) m_bIsVisible |= m_pChild->IsVisible(pCamera);
	}
#endif
	return (m_bIsVisible);
}

void CGameObject::Visible()
{
	OnPrepareRender();
	Update(nullptr);
}

//Picking
void CGameObject::GenerateRayForPicking(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxWorld, XMMATRIX *pd3dxmtxView, XMVECTOR *pd3dxvPickRayPosition, XMVECTOR *pd3dxvPickRayDirection)
{
	XMMATRIX d3dxmtxInverse;
	XMMATRIX d3dxmtxWorldView = *pd3dxmtxView;
	if (pd3dxmtxWorld) d3dxmtxWorldView = XMMatrixMultiply(*pd3dxmtxWorld, *pd3dxmtxView);
	d3dxmtxInverse = XMMatrixInverse(nullptr, d3dxmtxWorldView);

	/*
	pd3dxvPickRayDirection->x = pd3dxvPickPosition->x * d3dxmtxInverse._11 + pd3dxvPickPosition->y * d3dxmtxInverse._21 + pd3dxvPickPosition->z * d3dxmtxInverse._31;
	pd3dxvPickRayDirection->y = pd3dxvPickPosition->x * d3dxmtxInverse._12 + pd3dxvPickPosition->y * d3dxmtxInverse._22 + pd3dxvPickPosition->z * d3dxmtxInverse._32;
	pd3dxvPickRayDirection->z = pd3dxvPickPosition->x * d3dxmtxInverse._13 + pd3dxvPickPosition->y * d3dxmtxInverse._23 + pd3dxvPickPosition->z * d3dxmtxInverse._33;
	pd3dxvPickRayPosition->x = d3dxmtxInverse._41;
	pd3dxvPickRayPosition->y = d3dxmtxInverse._42;
	pd3dxvPickRayPosition->z = d3dxmtxInverse._43;
	*/
	XMVECTOR d3dxvCameraOrigin = XMVectorZero();
	*pd3dxvPickRayPosition = XMVector3TransformCoord(d3dxvCameraOrigin, d3dxmtxInverse);
	*pd3dxvPickRayDirection = XMVector3TransformCoord(*pd3dxvPickPosition, d3dxmtxInverse);
	*pd3dxvPickRayDirection = *pd3dxvPickRayDirection - *pd3dxvPickRayPosition;
}

int CGameObject::PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo)
{

	XMVECTOR d3dxvPickRayPosition, d3dxvPickRayDirection;
	int nIntersected = 0;
	if (m_bActive && m_bIsVisible && m_vsMeshes.size() > 0)
	{
		GenerateRayForPicking(pd3dxvPickPosition, &XMLoadFloat4x4(&m_d3dxmtxWorld), pd3dxmtxView, &d3dxvPickRayPosition, &d3dxvPickRayDirection);
		XMFLOAT3 XF3_PRP, XF3_PRD;
		XMStoreFloat3(&XF3_PRP, d3dxvPickRayPosition);
		XMStoreFloat3(&XF3_PRD, d3dxvPickRayDirection);
		D3DXVECTOR3 d3dvxprp = D3DXVECTOR3(XF3_PRP.x, XF3_PRP.y, XF3_PRP.z);
		D3DXVECTOR3 d3dvxprd = D3DXVECTOR3(XF3_PRD.x, XF3_PRD.y, XF3_PRD.z);
		for (auto &p : m_vsMeshes)
		{
			nIntersected = p->CheckRayIntersection(&d3dxvPickRayPosition, &d3dxvPickRayDirection, pd3dxIntersectInfo);
			if (nIntersected > 0) break;
		}
	}
	return (nIntersected);
	//XMVECTOR d3dxvPickRayPosition, d3dxvPickRayDirection;
	//int nIntersected = 0;
	//if (m_bActive && m_bIsVisible && m_ppMeshes)
	//{
	//	GenerateRayForPicking(pd3dxvPickPosition, &XMLoadFloat4x4(&m_d3dxmtxWorld), pd3dxmtxView, &d3dxvPickRayPosition, &d3dxvPickRayDirection);
	//	for (int i = 0; i < m_nMeshes; i++)
	//	{
	//		//nIntersected = m_ppMeshes[i]->CheckRayIntersection(&d3dxvPickRayPosition, &d3dxvPickRayDirection, pd3dxIntersectInfo);
	//		if (nIntersected > 0) break;
	//	}
	//}
	//return (nIntersected);
}


// Udpate
void CGameObject::OnPrepareRender()
{
}

void CGameObject::Animate(float fTimeElapsed)
{
	for (auto &component : m_vComponent)
	{
		component->Animate(fTimeElapsed);
	}
	for (auto &child : m_vChild)
	{
		child->Update(&GetWorldMatrix());
		//child->Update(&XMLoadFloat4x4(&m_d3dxmtxWorld));
		//child->Animate(fTimeElapsed);
	}

	//if ( m_vChild) m_pChild->Animate(fTimeElapsed);

}

void CGameObject::Animate(std::shared_ptr<CGameObject> gameObject, float fTimeElapsed)
{
	for (auto &component : m_vComponent)
		component->Animate(gameObject, fTimeElapsed);

	//if (m_pChild) m_pChild->Animate(fTimeElapsed);
}

void CGameObject::Animate(float fTimeElapsed, XMMATRIX *pd3dxmtxParent)
{

	Animate(fTimeElapsed);

	Update(pd3dxmtxParent);
}

void CGameObject::Update(XMMATRIX *pd3dxmtxParent)
{
	if (pd3dxmtxParent)
	{
		XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixMultiply(GetWorldMatrix(), *pd3dxmtxParent));
	}

	//if (m_pChild) m_pChild->Update(&XMLoadFloat4x4(&m_d3dxmtxWorld));
}

void CGameObject::Update(float fTimeElapsed)
{
}

//Render
void CGameObject::RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_vsMeshes.size() > 0)
	{
		for (auto &pMesh : m_vsMeshes)
		{
			if (pMesh) pMesh->Render(pd3dDeviceContext);
		}
	}
}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender();
	Update(nullptr);

	if (m_sShader) m_sShader->Render(pd3dDeviceContext, pCamera);

	CGameObject::UpdateConstBuffer(pd3dDeviceContext, XMLoadFloat4x4(&m_d3dxmtxWorld));
	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);



	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());

	RenderMesh(pd3dDeviceContext, pCamera);

	//if (m_pChild) m_pChild->Render(pd3dDeviceContext, pCamera);

	if (m_pd3dDepthStencilState) pd3dDeviceContext->OMSetDepthStencilState(nullptr, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

std::shared_ptr<CShader> CGameObject::SpacePartitioningFrustm(CCamera *pCamera)
{
	if (pCamera->IsInFrustum(&m_bcMeshBoundingCube))
	{
		SPACE->Update((std::shared_ptr<CGameObject>)this);
		return m_sShader;
	}
	else
		return nullptr;
}

void CGameObject::SetBoundingBox(CMesh* pMesh)
{
	if (pMesh)m_bcMeshBoundingbox = pMesh->GetBoundingBox();
}

void CGameObject::SetBoundingBox(BoundingBox boundingBox)
{
	m_bcMeshBoundingbox = boundingBox;

	static bool asdfsfasdf = true;
	if (asdfsfasdf)
	{
		::ConsoleShow(m_bcMeshBoundingbox.Center, "B-Center : ");
		::ConsoleShow(m_bcMeshBoundingbox.Extents, "B-Extens : ");
		asdfsfasdf = false;
	}
}


//Compoente
void CGameObject::SetCompoent(std::unique_ptr<Component> component)
{
	if (component == nullptr)return;
	if (m_vComponent.size() == 0) m_cPlayerState = static_cast<PlayerState*>(component.get());
	m_vComponent.push_back(move(component));// push_back(component.get());
}

void CGameObject::ComponentUpdate(float fTimeElasped)
{
	for (auto &component : m_vComponent)
		component->Animate(fTimeElasped);
}

Component* CGameObject::GetComponent(int index)
{
	return m_vComponent[index].get() ? m_vComponent[index].get() : nullptr;
}

Component* CGameObject::GetComponent(std::string name)
{
	for (auto &componet : m_vComponent)
	{
		if (componet->name == name)
		{
			return componet.get();
		}
	}
	return nullptr;
}

Component* CGameObject::GetComponent(TAG_TYPE tag)
{
	for (auto &componet : m_vComponent)
	{
		if (componet->tag == tag)
		{
			return componet.get();
		}
	}
	return nullptr;
}

std::vector<Component*> CGameObject::GetComponents(TAG_TYPE tag)
{
	std::vector<Component*> vComponent;
	bool nothing = true;
	for (auto &componet : m_vComponent)
	{
		if (componet->tag == tag)
		{
			nothing = false;
			vComponent.push_back(componet.get());
		}
	}
	if (nothing) return vComponent;

	return vComponent;
}

std::unique_ptr<Component> CGameObject::MoveCompoent(std::shared_ptr<CGameObject> other, TAG_TYPE tag)
{
	bool check = false;
	int n = 0;
	for (auto &componet : m_vComponent)
	{
		if (componet->tag == tag)
		{
			check = true;
			break;
		}
		n++;
	}
	if (false == check)
		return nullptr;

	int size = (int)m_vComponent.size() - 1;
	swap(m_vComponent[size], m_vComponent[n]);
	std::unique_ptr<Component> component = move(m_vComponent[size]);
	m_vComponent.pop_back();
	component->SetObject(other);

	if (false == component->BeforeMove())
	{
		component.reset(nullptr);
		return nullptr;
	}

	return move(component);
}

std::unique_ptr<Component> CGameObject::MoveCompoent(std::shared_ptr<CGameObject> other, std::string name)
{
	bool check = false;
	int n = 0;
	for (auto &component : m_vComponent)
	{
		if (component->name == name)
		{
			check = true;
			break;
		}
		n++;
	}
	if (false == check)
		return nullptr;

	int size = (int)(m_vComponent.size() - 1);
	swap(m_vComponent[size], m_vComponent[n]);
	std::unique_ptr<Component> component = move(m_vComponent[size]);
	m_vComponent.pop_back();
	component->SetObject(other);

	if (false == component->BeforeMove())
	{
		component.reset(nullptr);
		return nullptr;
	}
	return move(component);
}

std::unique_ptr<Component> CGameObject::MoveCompoent(std::shared_ptr<CGameObject> other, std::string name, int slot)
{
	bool check = false;
	int n = 0;
	int s = 0;
	for (auto &component : m_vComponent)
	{
		if (component->name == name)
		{
			if (s <= slot)
			{
				s++;
			}
			else
			{
				check = true;
				break;
			}
		}
		n++;
	}
	if (false == check)
		return nullptr;

	int size = (int)(m_vComponent.size() - 1);
	swap(m_vComponent[size], m_vComponent[n]);
	std::unique_ptr<Component> component = move(m_vComponent[size]);
	m_vComponent.pop_back();
	component->SetObject(other);

	if (false == component->BeforeMove())
	{
		component.reset(nullptr);
		return nullptr;
	}
	return move(component);
}

std::unique_ptr<Component> CGameObject::MoveCompoent(std::shared_ptr<CGameObject> other, int index)
{
	int size = (int)(m_vComponent.size() - 1);
	swap(m_vComponent[size], m_vComponent[index]);
	std::unique_ptr<Component> component = move(m_vComponent[size]);
	component->SetObject(other);
	m_vComponent.pop_back();

	if (false == component->BeforeMove())
	{
		component.reset(nullptr);
		return nullptr;
	}
	return move(component);
}

void CGameObject::DestoryComponet(std::string name)
{
	bool check = false;
	int n = 0;
	for (auto &component : m_vComponent)
	{
		if (component->name == name)
		{
			check = true;
			break;
		}
		n++;
	}
	if (false == check)
		return;

	int size = (int)m_vComponent.size() - 1;
	swap(m_vComponent[size], m_vComponent[n]);
	m_vComponent[size]->Destory();
	m_vComponent.pop_back();
}
void CGameObject::DestoryComponetWithId(int obj_id)
{
	bool check = false;
	int n = 0;
	for (auto &component : m_vComponent)
	{
		if (component->nGID == obj_id)
		{
			check = true;
			break;
		}
		n++;
	}
	if (false == check)
		return;

	int size = (int)m_vComponent.size() - 1;
	swap(m_vComponent[size], m_vComponent[n]);
	m_vComponent[size]->Destory();
	m_vComponent.pop_back();
}
void CGameObject::DestoryComponet(TAG_TYPE tag)
{
	bool check = false;
	int n = 0;
	for (auto &componet : m_vComponent)
	{
		if (componet->tag == tag)
		{
			check = true;
			break;
		}
		n++;
	}
	if (false == check)
		return;

	int size = (int)m_vComponent.size() - 1;
	std::swap(m_vComponent[size], m_vComponent[n]);
	m_vComponent[size]->Destory();
	m_vComponent.pop_back();
}
void CGameObject::DestoryComponet(int index)
{
	int size = (int)m_vComponent.size() - 1;
	std::swap(m_vComponent[size], m_vComponent[index]);
	m_vComponent[size]->Destory();
	m_vComponent.pop_back();
}
void CGameObject::DestoryItemComponet(std::string name, int slot)
{
	bool check = false;
	int n = 0;
	for (auto &component : m_vComponent)
	{
		if (component->name == name)
		{
			ItemBasic* item = (ItemBasic*)component.get();
			if (item->GetSlot() == slot)
			{
				check = true;
				break;
			}
		}
		n++;
	}
	if (false == check)
		return;

	auto size = m_vComponent.size() - 1;
	swap(m_vComponent[size], m_vComponent[n]);
	m_vComponent[size]->Destory();
	m_vComponent.pop_back();
}

void CGameObject::DestoryItemComponet(int id, int slot)
{
	bool check = false;
	int n = 0;
	for (auto &component : m_vComponent)
	{
		if (component->nGID == id)
		{
			ItemBasic* item = (ItemBasic*)component.get();
			if (item->GetSlot() == slot)
			{
				check = true;
				break;
			}
		}
		n++;
	}
	if (false == check)
		return;

	auto size = m_vComponent.size() - 1;
	swap(m_vComponent[size], m_vComponent[n]);
	m_vComponent[size]->Destory();
	m_vComponent.pop_back();
}

void CGameObject::DestoryChild()
{
	for (auto &child : m_vChild) child->SetActive(false);
}

//Set Get
//void CGameObject::SetLocalMatrix(XMMATRIX& matirx)
//{
//	XMStoreFloat4x4(&m_d3dxmtxLocal, matirx);
//}
//void CGameObject::SetLocalMatrix(XMFLOAT4X4& matirx)
//{
//	m_d3dxmtxLocal = matirx;
//}
void CGameObject::SetWorldMatrix(XMMATRIX& matirx)
{
	XMStoreFloat4x4(&m_d3dxmtxWorld, matirx);
}
void CGameObject::SetWorldMatrix(XMFLOAT4X4& matirx)
{
	m_d3dxmtxWorld = matirx;
}

//Move & Rotateion Fucntion
void CGameObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;
	//KYT '16.03.31
	/*
	Update!
	*/
	Update(nullptr);
}

void CGameObject::SetPosition(const XMVECTOR& d3dxvPosition)
{
	XMFLOAT4 f4vPosition;
	XMStoreFloat4(&f4vPosition, d3dxvPosition);
	SetPosition(f4vPosition.x, f4vPosition.y, f4vPosition.z);
}

void CGameObject::SetPosition(const XMFLOAT3& d3dxvPosition)
{
	SetPosition(d3dxvPosition.x, d3dxvPosition.y, d3dxvPosition.z);
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMVECTOR d3dxvPosition = GetPosition();
	XMVECTOR d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	XMVECTOR d3dxvPosition = GetPosition();
	XMVECTOR d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	XMVECTOR d3dxvPosition = GetPosition();
	XMVECTOR d3dxvLookAt = GetLook();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::RotateWorldAxis(const float& x, const float& y, const float& z)
{
	XMMATRIX xmmtxRotate;
	if (x != 0.0f)
	{
		xmmtxRotate = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), (float)XMConvertToRadians(x));
		XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_d3dxmtxWorld)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetRight(), x));
	}
	if (y != 0.0f)
	{
		//플레이어의 로컬 y-축을 기준으로 회전하는 행렬을 생성한다.
		xmmtxRotate = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), (float)XMConvertToRadians(y));
		XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_d3dxmtxWorld)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetUp(), y));
	}
	if (z != 0.0f)
	{
		//플레이어의 로컬 z-축을 기준으로 회전하는 행렬을 생성한다.
		xmmtxRotate = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), (float)XMConvertToRadians(z));
		XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_d3dxmtxWorld)));
		//SetRotationQuaternion(XMQuaternionRotationAxis(GetLook(), z));
	}


	m_xmf3Rotate = XMFLOAT3{ m_xmf3Rotate.x + x, m_xmf3Rotate.y + y, m_xmf3Rotate.z + z };
	SetQuaternion(XMQuaternionRotationMatrix(GetWorldMatrix()));
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	XMStoreFloat4x4(&m_d3dxmtxWorld, mtxRotate * XMLoadFloat4x4(&m_d3dxmtxWorld));
	//	if (!m_pParent) m_d3dxmtxWorld = m_d3dxmtxLocal;

	SetQuaternion(XMQuaternionRotationMatrix(GetWorldMatrix()));

}

void CGameObject::Rotate(XMVECTOR& pd3dxvAxis, float fAngle)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationAxis(pd3dxvAxis, XMConvertToRadians(fAngle));
	XMStoreFloat4x4(&m_d3dxmtxWorld, mtxRotate * XMLoadFloat4x4(&m_d3dxmtxWorld));
	//	if (!m_pParent) m_d3dxmtxWorld = m_d3dxmtxLocal;

	SetQuaternion(XMQuaternionRotationMatrix(GetWorldMatrix()));

}

XMVECTOR& CGameObject::GetPosition(bool bIsLocal)
{
	m_xmVector = XMVectorSet(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43, 0.0f);
	return  m_xmVector;
}

XMFLOAT3& CGameObject::GetPositionXMFLOAT3(bool bIsLocal)
{
	return m_xmFloat3 = XMFLOAT3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43);
}

XMVECTOR& CGameObject::GetRight(bool bIsLocal)
{
	m_xmVector = XMVectorSet(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13, 0.0f);
	m_xmVector = XMVector3Normalize(m_xmVector);
	return (m_xmVector);
}

XMVECTOR& CGameObject::GetUp(bool bIsLocal)
{
	m_xmVector = XMVectorSet(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23, 0.0f);
	m_xmVector = XMVector3Normalize(m_xmVector);
	return (m_xmVector);
}

XMVECTOR& CGameObject::GetLook(bool bIsLocal)
{
	m_xmVector = XMVectorSet(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33, 0.0f);
	m_xmVector = XMVector3Normalize(m_xmVector);
	return (m_xmVector);
}


//------------------------------------------직교투영 JJY
void CGameObject::UIRender()
{
	OnPrepareRender();
	Update(nullptr);
}

//------------------------------------------직교투영 JJY

//KYT '16.04.23
void CGameObject::IntersectsDynamic(std::shared_ptr<CGameObject> other)
{
	if (true == m_bcMeshBoundingbox.Intersects(other->GetBoundingBox()))
	{
		for (auto &componet : m_vComponent)
			componet->Collision(other, ObjectType::DynamicObject);
	}
}
void CGameObject::IntersectsStatic(std::shared_ptr<CGameObject> other)
{
	if (true == m_bcMeshBoundingbox.Intersects(other->GetBoundingBox()))
	{
		size_t n = m_vComponent.size();
		for (int i = 0; i < m_vComponent.size(); ++i)
			m_vComponent[i]->Collision(other, ObjectType::StaticObject);
	}
}
void CGameObject::IntersectsWorldMatrix(std::shared_ptr<WorldMatrix> matrix)
{
}

//KYT '16.07.08
void CGameObject::IntersectsPlayer(std::shared_ptr<CGameObject> other)
{
}

//삭제 원리  -> 공간분할 ObjectContainer에서 gameObject를 삭제
void CGameObject::DynamicDestory(std::shared_ptr<CGameObject> gameObject)
{
	for (auto &component : m_vComponent) component->Destory();
	m_vComponent.clear();
	gameObject->DestoryChild();
	SPACE->DynamicDestory(gameObject);
}
void CGameObject::StaticDestory(std::shared_ptr<CGameObject> gameObject)
{
	for (auto &component : m_vComponent) component->Destory();
	m_vComponent.clear();
	gameObject->DestoryChild();
	SPACE->StaticDestory(gameObject);
}
void CGameObject::BlendDestory(std::shared_ptr<CGameObject> gameObject)
{
	for (auto &component : m_vComponent) component->Destory();
	m_vComponent.clear();
	gameObject->DestoryChild();
	SPACE->BlendObjectDestory(gameObject);
}
void CGameObject::ChildDestory(std::shared_ptr<CGameObject> gameObject)
{
	for (auto &component : m_vComponent) component->Destory();
	m_vComponent.clear();
	SPACE->ChlidDestory(gameObject);
}
void CGameObject::WorldMatrixDestory(std::shared_ptr<WorldMatrix> matirx)
{
	for (auto &component : m_vComponent) component->Destory();
	m_vComponent.clear();
	SPACE->WorldMatrixDestory(matirx);
}


