
#pragma once

#include "stdafx.h"
#include "Warp3DModelManager.h"
#include "Warp3DSkinningShaderforBunny.h"

CWarp3DSkinningShaderforBunny::CWarp3DSkinningShaderforBunny(int nObjects) : CWarp3DSkinningShader(nObjects)
{
	m_pModelData = new CWarp3DModel(&(MODELMGR->GetModelData("../Assets/02_ModelData/03_Monster/Zombunny.warp3d")));
	//*m_pModelData = MODELMGR->GetModelData("../Assets/02_ModelData/03_Monster/Zombunny00.warp3d");

	m_pAnimData = new CWarpAnimation(&(MODELMGR->GetAnimationData("../Assets/02_ModelData/03_Monster/Zombunny.warpanim")));

	m_currentKeyFrameNum = 0;
	//*m_pAnimData = MODELMGR->GetAnimationData("../Assets/02_ModelData/03_Monster/Zombunny.warpanim");

	m_pcbBoneTransform = nullptr;
}

void CWarp3DSkinningShaderforBunny::AnimateObjects(float fTimeElapsed)
{
#ifndef BUNNY_SKINNEDINSTANCING
	if (m_currentKeyFrameNum >= m_pAnimData->GetEndKeyframe())
	{
		m_currentKeyFrameNum = 0;
	}
	else
	{
		//RSH 16'.04.16
		/*
		키프레임 수정
		*/
	
		m_keyframeStack += 30.0f*fTimeElapsed;
		if (m_keyframeStack >= 1.0f)
		{
			m_currentKeyFrameNum++;
			m_keyframeStack = 0.0f;
		}
	}
#endif
	//CInstancedObjectsShader::AnimateObjects(fTimeElapsed);
}

#ifdef BUNNY_SKINNEDINSTANCING
void CWarp3DSkinningShaderforBunny::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
#pragma region[Vertex Texxture Fetch: 애니메이션 변환행렬을 담은 2D텍스쳐 구성]

	UINT uiAnimationTextureWidth = 440;
	UINT uiAnimationTextureHeight = m_pAnimData->GetBoneCount() * 3;
	UINT uiTexelDataSize = uiAnimationTextureWidth * 4 * uiAnimationTextureHeight;

	float *animTexel = new float[uiTexelDataSize];

	for (int i = 0; i<uiTexelDataSize; ++i)
	{
		animTexel[i] = 0.0f;
	}

	FbxAMatrix tempBoneTransforms;
	UINT uiIndex = 0;
	UINT boneIndex = 0;

	for (boneIndex = 0; boneIndex < m_pAnimData->GetBoneCount(); ++boneIndex)
	{
		uiIndex = (boneIndex * uiAnimationTextureWidth * 4 * 3);

		for (UINT keyframeNum = 0; keyframeNum < uiAnimationTextureWidth; ++keyframeNum)
		{
			tempBoneTransforms.SetIdentity();

			//TransformationOfPoseAtTimeT
			tempBoneTransforms
				*= m_pAnimData->GetSkeletonKeyframeGlobalTransform(boneIndex, keyframeNum);

			//GlobalBindposeInverse
			tempBoneTransforms
				*= m_pAnimData->GetBoneKeyframedata(boneIndex).mGlobalBindposeInverse;

			//RSH 16.07.26
			/*
			특정 본의 키프레임 행렬들의 정보를 넣는데, 3개의 행을 사용한다.
			ex- 본이 12개일 경우, 텍스쳐는 36개를 사용
			*/
			animTexel[uiIndex]
				= (float)tempBoneTransforms.Get(0, 0);
			animTexel[uiIndex + (uiAnimationTextureWidth * 4)]
				= (float)tempBoneTransforms.Get(1, 0);
			animTexel[uiIndex + (uiAnimationTextureWidth * 4 * 2)]
				= (float)tempBoneTransforms.Get(2, 0);
			uiIndex++;

			animTexel[uiIndex]
				= (float)tempBoneTransforms.Get(0, 1);
			animTexel[uiIndex + (uiAnimationTextureWidth * 4)]
				= (float)tempBoneTransforms.Get(1, 1);
			animTexel[uiIndex + (uiAnimationTextureWidth * 4 * 2)]
				= (float)tempBoneTransforms.Get(2, 1);
			uiIndex++;

			animTexel[uiIndex]
				= (float)tempBoneTransforms.Get(0, 2);
			animTexel[uiIndex + (uiAnimationTextureWidth * 4)]
				= (float)tempBoneTransforms.Get(1, 2);
			animTexel[uiIndex + (uiAnimationTextureWidth * 4 * 2)]
				= (float)tempBoneTransforms.Get(2, 2);
			uiIndex++;

			animTexel[uiIndex]
				= (float)tempBoneTransforms.Get(3, 0);
			animTexel[uiIndex + (uiAnimationTextureWidth * 4)]
				= (float)tempBoneTransforms.Get(3, 1);
			animTexel[uiIndex + (uiAnimationTextureWidth * 4 * 2)]
				= (float)tempBoneTransforms.Get(3, 2);
			uiIndex++;
		}
	}

	D3D11_TEXTURE2D_DESC AnimTextureDesc;
	ZeroMemory(&AnimTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	AnimTextureDesc.Width = uiAnimationTextureWidth;
	AnimTextureDesc.Height = uiAnimationTextureHeight;
	AnimTextureDesc.MipLevels = 1;
	AnimTextureDesc.ArraySize = 1;
	AnimTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	AnimTextureDesc.SampleDesc.Count = 1;
	AnimTextureDesc.SampleDesc.Quality = 0;
	AnimTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	AnimTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	AnimTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	AnimTextureDesc.MiscFlags = 0;

	ID3D11Texture2D *animationTex = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = (void*)animTexel;
	//RSH '16.07.30
	/*
	매우 중요! SysMemPitch는 텍스쳐의 row(행)의 바이트 크기를 설정하는 것이다.
	픽셀 크기가 아니라 바이트 크기임을 숙지하고 있어야한다!(사흘 동안 문제가 되었던 작업!)
	*/
	InitData.SysMemPitch = sizeof(float) * 4 * uiAnimationTextureWidth;
	pd3dDevice->CreateTexture2D(&AnimTextureDesc, &InitData, &animationTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC AnimSRViewDesc;
	ZeroMemory(&AnimSRViewDesc, sizeof(AnimSRViewDesc));
	AnimSRViewDesc.Format = AnimTextureDesc.Format;
	AnimSRViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	AnimSRViewDesc.Texture2D.MostDetailedMip = 0;
	AnimSRViewDesc.Texture2D.MipLevels = 1;
	animationTexSRV = new ID3D11ShaderResourceView*;
	pd3dDevice->CreateShaderResourceView(animationTex, &AnimSRViewDesc, animationTexSRV);

	if (animationTex)animationTex->Release();
	animationTex = nullptr;

	delete[] animTexel;
#pragma endregion

	CWarp3DSkinningShader::BuildObjects(pd3dDevice, pContext);
}

void CWarp3DSkinningShaderforBunny::CreateShader(ID3D11Device *pd3dDevice)
{

	int nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//1
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//2

		{ "BONE_INDEICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/MonsterSkinnedInstancing/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/MonsterSkinnedInstancing/PS.fxo", &m_pd3dPixelShader);



	//	CreateVertexShaderFromFile(pd3dDevice, L"FBXSkinningInstanced.fx", "VSSkinnedInstanced", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"FBXSkinningInstanced.fx", "PSSkinnedInstanced_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"FBXSkinningInstanced.fx", "PSSkinnedInstanced", "ps_5_0", &m_pd3dPixelShader);
	//#endif

	//CreateVertexShaderFromFile(pd3dDevice, L"MonsterSkinnedInstancing.fx", "VSSkinnedInstanced", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"MonsterSkinnedInstancing.fx", "PSSkinnedInstanced_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"MonsterSkinnedInstancing.fx", "PSSkinnedInstanced", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

void CWarp3DSkinningShaderforBunny::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (bBoneTransformSetting) CWarp3DSkinningShader::UpdateConstBuffer(pd3dDeviceContext);
	pd3dDeviceContext->VSSetShaderResources(VS_SLOT_ANIMATION_TEXTURE, 1, animationTexSRV);

	CTexture::UpdateConstBuffer(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxTexture));

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
	for (auto &pObject : m_vRenderObject)
	{
#ifdef _SPACEPARTITIONING
		pd3dxmtxInstances[nInstances] = XMMatrixTranspose(pObject->GetWorldMatrix());
#else
		if (pObject->IsVisible(pCamera))
			pd3dxmtxInstances[nInstances] = XMMatrixTranspose(pObject->GetWorldMatrix());
#endif
		pd3dxmtxInstances[nInstances++].r[3].m128_f32[3] = pObject->GetCurrentKeyframe();
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	if (m_pd3dBlendState) pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, nullptr, 0xffffffff);
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	if (m_pd3dBlendState) pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	//KYT '16.02.05 
	/*아직 삭제하기에는 이름*/
#ifdef _SPACEPARTITIONING
	m_vRenderObject.clear();
#endif
}
#endif