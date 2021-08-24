#pragma once

#include "stdafx.h"
#include "Warp3DModelManager.h"
#include "WarpAnimation.h"
#include "Warp3DModel.h"
#include "Warp3DPlayerShader.h"

//RSH '16.07.16 
/*
CWarp3DPlayerShader 제작
*/

CWarp3DPlayerShader::CWarp3DPlayerShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(move(pMesh), nObjects)
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	bBoneTransformSetting = false;
	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;
	m_currnetAnimationState = AnimState::ANI_IDLE;
	m_currentKeyFrameNum = 0;
	m_keyframeStack = 0.0f;
}

CWarp3DPlayerShader::CWarp3DPlayerShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	bBoneTransformSetting = false;
	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;
	m_currnetAnimationState = AnimState::ANI_IDLE;
	m_currentKeyFrameNum = 0;
	m_keyframeStack = 0.0f;
}

CWarp3DPlayerShader::CWarp3DPlayerShader(int nObjects) : CInstancedObjectsShader(nObjects)
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	bBoneTransformSetting = false;
	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;
	m_currnetAnimationState = AnimState::ANI_RUN;
	m_currentKeyFrameNum = 0;
	m_keyframeStack = 0.0f;
}

CWarp3DPlayerShader::~CWarp3DPlayerShader()
{

}

void CWarp3DPlayerShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CObjectsShader::BuildObjects(pd3dDevice, pContext);

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

void CWarp3DPlayerShader::CreateShader(ID3D11Device *pd3dDevice)
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
		{ "BONE_WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/PlayerShader/Warp3D_VSPlayerShader.cso", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/PlayerShader/Warp3D_PSPlayerShader.cso", &m_pd3dPixelShader);


	//CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/PlayerShader/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	//CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/PlayerShader/PS.fxo", &m_pd3dPixelShader);

	

	//CreateVertexShaderFromFile(pd3dDevice, L"PlayerShader.fx", "VSSkinnedPlayer", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"PlayerShader.fx", "PSSkinnedPlayer_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"PlayerShader.fx", "PSSkinnedPlayer", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}	

void CWarp3DPlayerShader::AnimateObjects(float fTimeElapsed)
{
	//CInstancedObjectsShader::AnimateObjects(fTimeElapsed);
}

void CWarp3DPlayerShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
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

		//TransformationOfPoseAtTimeT
		tempBoneTransforms
			*= m_pAnimations[m_currnetAnimationState]->GetSkeletonKeyframeGlobalTransform(jointIndex, m_currentKeyFrameNum);

		//InverseOfGlobalBindPoseMatrix
		tempBoneTransforms
			*= m_pAnimations[m_currnetAnimationState]->GetBoneKeyframedata(jointIndex).mGlobalBindposeInverse;

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

void CWarp3DPlayerShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_vRenderObject.size() > 0)
	{
		if (m_vRenderObject[0].get())
		{
			m_currentKeyFrameNum = m_vRenderObject[0]->GetCurrentKeyframe();
			if (m_currnetAnimationState != m_vRenderObject[0]->GetCurrentAnimationState())
			{
				m_currnetAnimationState = m_vRenderObject[0]->GetCurrentAnimationState();
				m_currentKeyFrameNum = m_pAnimations[m_currnetAnimationState]->GetStartKeyframe();
			}
		}
	}

	if (bBoneTransformSetting) CWarp3DPlayerShader::UpdateConstBuffer(pd3dDeviceContext);

	CShader::Render(pd3dDeviceContext, pCamera);
	m_pMesh->Render(pd3dDeviceContext);
}

//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------

CQueryChanPlayerShader::CQueryChanPlayerShader(int nObjects) : CWarp3DPlayerShader(nObjects)
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

void CQueryChanPlayerShader::AddPlayer(std::shared_ptr<CPlayer> pPlayer)
{
	//m_pMesh = std::move(pPlayer->GetMesh());
}
