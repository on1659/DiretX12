#include "stdafx.h"
#include "Warp3DOtherPlayerShader.h"
#include "FBXLoader.h"
#include "InstancedObjectsShader.h"
#include "WARP3DMesh.h"
#include "Warp3DModelManager.h"
#include "WarpAnimation.h"
#include "Timer.h"
#include "FBXSingleMesh.h"

#include "ShadowMapShader.h"
#include "QuadrangleMesh.h"
#include "HeadBangBangShader.h"
#include "PlayerHeadBangBang.h"


//----------------------------------------------------------------
CWarp3DOtherPlayerShader::CWarp3DOtherPlayerShader(std::unique_ptr<CMesh> pMesh) : CWarp3DSkinningShader(move(pMesh))
{
	m_pAnimData = nullptr;
	m_pModelData = nullptr;
	m_pAnimations.clear();

	bBoneTransformSetting = false;
	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;
	m_currnetAnimationState = AnimState::ANI_IDLE;
	m_currentKeyFrameNum = 0;
}

CWarp3DOtherPlayerShader::CWarp3DOtherPlayerShader(int nObjects) : CWarp3DSkinningShader(nObjects)
{
	m_pAnimData = nullptr;
	m_pModelData = nullptr;
	m_pAnimations.clear();

	bBoneTransformSetting = false;
	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;
	m_currnetAnimationState = AnimState::ANI_IDLE;
	m_currentKeyFrameNum = 0;
}

CWarp3DOtherPlayerShader::~CWarp3DOtherPlayerShader()
{

}

void CWarp3DOtherPlayerShader::CreateShader(ID3D11Device *pd3dDevice)
{
	//뼈대 변환행렬들의 상수버퍼 생성
	bBoneTransformSetting = true;
	m_pcbBoneTransform = nullptr;

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(GlobalMatrixBoneTransforms);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pcbBoneTransform);


	int nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//1
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//2
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,  nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "BONE_INDEICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromCompiledFile(pd3dDevice, TEXT("../Assets/07.Shader/OtherPlayerShader/Warp3D_VSOtherPlayerShader.cso"), d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, TEXT("../Assets/07.Shader/OtherPlayerShader/Warp3D_PSOtherPlayerShader.cso"), &m_pd3dPixelShader);


	//CreateVertexShaderFromFile(pd3dDevice, L"OtherPlayerSkinningInstanced.fx", "VSSkinnedInstanced", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"OtherPlayerSkinningInstanced.fx", "PSSkinnedInstanced_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"OtherPlayerSkinningInstanced.fx", "PSSkinnedInstanced", "ps_5_0", &m_pd3dPixelShader);
	//#endif

}

void CWarp3DOtherPlayerShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CObjectsShader::BuildObjects(pd3dDevice, pContext);

	if (!m_pModelData) return;

	int m_nControlPoints = m_pModelData->GetModelInfo().m_nIndices;

	XMUINT4 *puiBlendingindices;
	puiBlendingindices = new XMUINT4[m_nControlPoints];

	XMFLOAT3 *pxvBlendingWeights;
	pxvBlendingWeights = new XMFLOAT3[m_nControlPoints];

	for (int k = 0; k < m_nControlPoints; k++)
	{
		auto tmpBlendingInfo =
			m_pAnimations[0]->GetBlendingIndexWeightPair(m_pModelData->GetModelInfo().GetFaceIndex(k));

		puiBlendingindices[k] =
			XMUINT4(m_pAnimations[0]->GetBoneListIndex(tmpBlendingInfo[0].first),
				m_pAnimations[0]->GetBoneListIndex(tmpBlendingInfo[1].first),
				m_pAnimations[0]->GetBoneListIndex(tmpBlendingInfo[2].first),
				m_pAnimations[0]->GetBoneListIndex(tmpBlendingInfo[3].first));
		pxvBlendingWeights[k] =
			XMFLOAT3(tmpBlendingInfo[0].second, tmpBlendingInfo[1].second, tmpBlendingInfo[2].second);
	}

	m_pd3dBlendingIndexBuffer = CreateBuffer(pd3dDevice, sizeof(XMUINT4), m_nControlPoints, puiBlendingindices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dBlendingWeightBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nControlPoints, pxvBlendingWeights, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	if (m_pd3dBlendingIndexBuffer && m_pd3dBlendingWeightBuffer)
	{
		delete[] puiBlendingindices;
		delete[] pxvBlendingWeights;

		ID3D11Buffer *pd3dBlendingInfoBuffers[2] = { m_pd3dBlendingIndexBuffer, m_pd3dBlendingWeightBuffer };
		UINT pnBlendingInfoBufferStrides[2] = { sizeof(XMUINT4), sizeof(XMFLOAT3) };
		UINT pnBlendingInfoBufferOffsets[2] = { 0, 0 };
		m_pMesh->AssembleToVertexBuffer(2, pd3dBlendingInfoBuffers, pnBlendingInfoBufferStrides, pnBlendingInfoBufferOffsets);
	}

	m_pd3dInstanceBuffer = CreateBuffer(pd3dDevice, m_nInstanceBufferStride, m_nObjects, nullptr, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_pMesh->AssembleToVertexBuffer(1, &m_pd3dInstanceBuffer, &m_nInstanceBufferStride, &m_nInstanceBufferOffset);
}

void CWarp3DOtherPlayerShader::RenderPlayer(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_vRenderObject.size() != 0)
	{
		if (m_vRenderObject[0])
		{
			m_currentKeyFrameNum = m_vRenderObject.begin()[0]->GetCurrentKeyframe();
			if (m_currnetAnimationState != m_vRenderObject[0]->GetCurrentAnimationState())
			{
				m_currnetAnimationState = m_vRenderObject[0]->GetCurrentAnimationState();
				m_currentKeyFrameNum = m_pAnimations[m_currnetAnimationState]->GetStartKeyframe();
			}
		}
	}

	if (bBoneTransformSetting) CWarp3DOtherPlayerShader::UpdateConstBuffer(pd3dDeviceContext);

	CInstancedObjectsShader::RenderPlayer(pd3dDeviceContext, pCamera);

	//m_vRenderObject[0]->Render(pd3dDeviceContext, pCamera);

	//KYT '16.05.08
	/*
		OtherPlayer Cube View
	*/
	//JJY '16.05.08
	/*
		- 팀 식별용 메쉬
		메쉬 위치 수정
	*/

	if (nullptr != m_pCubeShader)
	{
		XMFLOAT3 pos = m_vRenderObject[0]->GetPositionXMFLOAT3(); pos.y += 1.3f;
		m_pCubeShader->GetvRenderObject()[0]->SetPosition(pos.x, pos.y, pos.z);
		m_pCubeShader->RenderPlayer(pd3dDeviceContext, pCamera);
	}

	m_pShadowShader->SetPosition(m_vRenderObject[0]->GetPositionXMFLOAT3());
	m_pShadowShader->Render(pd3dDeviceContext, pCamera);

	if (m_sPlayerState->IsHeadBang())
	{
		m_pHeadBangBangShader->SetPlayer(static_cast<CPlayer*>(m_vRenderObject[0].get()));
		m_pHeadBangBangShader->RenderNoClear(pd3dDeviceContext, pCamera);
	}

}

void CWarp3DOtherPlayerShader::AnimateObjects(float fTimeElapsed)
{

}

void CWarp3DOtherPlayerShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pcbBoneTransform, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	/*
	이 구간에서 상수버퍼로 들어갈 본 행렬을 정한다.
	애니메이션이 재생 중이라면, 키프레임 마다 저장되어있는 본 변환행렬을 끄집어 와야된다.
	*/
	GlobalMatrixBoneTransforms* gmtxbtf = (GlobalMatrixBoneTransforms*)d3dMappedResource.pData;

	FbxAMatrix tempBoneTransforms;

	for (int jointIndex = 0; jointIndex < m_pAnimations[m_currnetAnimationState]->GetBoneCount(); ++jointIndex)
	{
		tempBoneTransforms.SetIdentity();

		if (m_pAnimations.size())
		{
			//TransformationOfPoseAtTimeT
			tempBoneTransforms
				*= m_pAnimations[m_currnetAnimationState]->GetSkeletonKeyframeGlobalTransform(jointIndex, m_currentKeyFrameNum);

			//InverseOfGlobalBindPoseMatrix
			tempBoneTransforms
				*= m_pAnimations[m_currnetAnimationState]->GetBoneKeyframedata(jointIndex).mGlobalBindposeInverse;
		}

		XMStoreFloat4x4(&(gmtxbtf->gmtxBoneTransforms[jointIndex]),
			XMMatrixTranspose(XMMATRIX(
			(float)tempBoneTransforms.Get(0, 0), (float)tempBoneTransforms.Get(0, 1), (float)tempBoneTransforms.Get(0, 2), (float)tempBoneTransforms.Get(0, 3),
				(float)tempBoneTransforms.Get(1, 0), (float)tempBoneTransforms.Get(1, 1), (float)tempBoneTransforms.Get(1, 2), (float)tempBoneTransforms.Get(1, 3),
				(float)tempBoneTransforms.Get(2, 0), (float)tempBoneTransforms.Get(2, 1), (float)tempBoneTransforms.Get(2, 2), (float)tempBoneTransforms.Get(2, 3),
				(float)tempBoneTransforms.Get(3, 0), (float)tempBoneTransforms.Get(3, 1), (float)tempBoneTransforms.Get(3, 2), (float)tempBoneTransforms.Get(3, 3))));

		//XMStoreFloat4x4(&(gmtxbtf->gmtxBoneTransforms[jointIndex]),XMMatrixIdentity());
	}

	pd3dDeviceContext->Unmap(m_pcbBoneTransform, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_BONE_TRANSFORM, 1, &m_pcbBoneTransform);
}


//KYT '16.05.08
/*
	OtherPlayer Cube View
*/
//JJY '16.05.08
/*
	- 팀 식별용 메쉬
	팀 선택에 따라 팀 식별용 메쉬 바꾸기
	함수 일부 수정
*/
void CWarp3DOtherPlayerShader::CreateTeamMarkMesh(ID3D11Device * pd3dDevice, UINT team_line)
{
	m_pCubeShader = std::make_unique<CInstancedObjectsShader>();
	m_pCubeShader->CreateShader(pd3dDevice);
	//m_pCubeShader->SetMesh(std::move(std::make_unique<Default_Mesh>(pd3dDevice, 0.2f, 0.2f, 0.2f)));

	if (team_line == PLAYER_TEAM_LINE::A_TEAM_0 || team_line == PLAYER_TEAM_LINE::A_TEAM_1)
		m_pCubeShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/02_ModelData/04_Others/Mark/voxel_redDiamond.FBX", 0.016f, SH_FBXTYPE::FBX_VOXEL)));
	else if (team_line == PLAYER_TEAM_LINE::B_TEAM_0 || team_line == PLAYER_TEAM_LINE::B_TEAM_1)
		m_pCubeShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/02_ModelData/04_Others/Mark/voxel_blueDiamond.FBX", 0.016f, SH_FBXTYPE::FBX_VOXEL)));
	//m_pCubeShader->SetMaterial(pd3dDevice, "../Assets/Voxel/untitled.png");

	m_pCubeShader->BuildObjects(pd3dDevice);
	std::shared_ptr<CGameObject> pGameObject = std::make_shared<CGameObject>();
	m_pCubeShader->AddObject(pGameObject);


	////Shadow
	m_pShadowShader = std::make_unique <CShadowMapShader>();
	m_pShadowShader->CreateShader(pd3dDevice);
	m_pShadowShader->SetMesh(move(std::make_unique<CQuadrangleMesh>(pd3dDevice, 0.4, 0.4)));
	//m_pShadowShader->SetMaterial(pd3dDevice, "../Assets/Shadow.png"); 
	m_pShadowShader->SetMaterial(pd3dDevice, "../Assets/02_ModelData/04_Others/character_shadow_texture.png");

	auto pos = m_vRenderObject[0]->GetPositionXMFLOAT3();
	m_pShadowShader->SetFirstPosition(pos.x, 1, pos.z);
	m_pShadowShader->AddObject(pGameObject);
	m_pShadowShader->BuildObjects(pd3dDevice, nullptr);
	m_pShadowShader->SetBlendState("Default");

	//HeadBangBang
	m_pHeadBangBangShader = std::make_unique<CHeadBangBangShader>();
	m_pHeadBangBangShader->CreateShader(pd3dDevice);
	m_pHeadBangBangShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/Star.warp3d")));
	m_pHeadBangBangShader->BuildObjects(pd3dDevice);

	std::shared_ptr<CGameObject> headObject = std::make_shared<CGameObject>();
	CPlayer* player = (CPlayer*)m_vRenderObject[0].get();
	auto script = std::make_unique<PlayerHeadBangBang>();
	script->SetPlayer(player);
	script->SetGameObject(headObject);

	headObject->SetPosition(player->GetPositionXMFLOAT3());
	player->SetCompoent(move(script));
	m_pHeadBangBangShader->AddObject(headObject);

	m_sPlayerState = player->GetPlayerState();
	player = nullptr;
}

CQueryChanOtherPlayerShader::CQueryChanOtherPlayerShader(int nObjects) : CWarp3DOtherPlayerShader(nObjects)
{

	m_pModelData = new CWarp3DModel(&(MODELMGR->GetModelData("../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d")));

	m_pAnimations.clear();

	std::vector<std::string> animFilepaths;
	animFilepaths.reserve(9);

	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[idle].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[running_inPlace].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[falldown_front].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[right_strafe_inPlace].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[left_strafe_inPlace].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[jog_forwardRight_diagonal_inPlace].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[jog_forwardLeft_diagonal_inPlace].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[running_backward_inPlace].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[smash].warpanim");
	animFilepaths.push_back("../Assets/02_ModelData/06_Character_Query01/QueryChan01[falling_flat_impact].warpanim");

	for (auto &path : animFilepaths)
	{
		CWarpAnimation* m_pAnimLoader = new CWarpAnimation(&(MODELMGR->GetAnimationData(path)));

		m_pAnimations.push_back(m_pAnimLoader);
	}
}