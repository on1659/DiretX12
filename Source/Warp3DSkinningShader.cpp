#include "stdafx.h"
#include "Warp3DModelManager.h"
#include "Warp3DSkinningShader.h"
#include "Warp3DModel.h"
#include "WarpAnimation.h"

CWarp3DSkinningShader::CWarp3DSkinningShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	m_pModelData = nullptr;
	m_pAnimData = nullptr;

	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;
	bBoneTransformSetting = false;
	m_currentKeyFrameNum = 0;
	m_keyframeStack = 0.0f;
}

CWarp3DSkinningShader::CWarp3DSkinningShader(int nObjects) : CInstancedObjectsShader(nObjects)
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;
	bBoneTransformSetting = false;
	m_currentKeyFrameNum = 0;
	m_keyframeStack = 0.0f;
}


CWarp3DSkinningShader::~CWarp3DSkinningShader()
{
}

void CWarp3DSkinningShader::CreateShader(ID3D11Device *pd3dDevice)
{
	bBoneTransformSetting = true;
	m_pcbBoneTransform = nullptr;

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(GlobalMatrixBoneTransforms);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pcbBoneTransform);

	UINT nSlot = 0;
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
	
	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/FBXSkinningSkinningInstanced/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/FBXSkinningSkinningInstanced/PS.fxo", &m_pd3dPixelShader);


	//CreateVertexShaderFromFile(pd3dDevice, L"FBXSkinningInstanced.fx", "VSSkinnedInstanced", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"FBXSkinningInstanced.fx", "PSSkinnedInstanced_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"FBXSkinningInstanced.fx", "PSSkinnedInstanced", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

void CWarp3DSkinningShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pcbBoneTransform, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	/*
	이 구간에서 상수버퍼로 들어갈 본 행렬을 정한다.
	애니메이션이 재생 중이라면, 키프레임 마다 저장되어있는 본 변환행렬을 끄집어 와야된다.
	*/

	GlobalMatrixBoneTransforms* gmtxbtf = (GlobalMatrixBoneTransforms*)d3dMappedResource.pData;

	FbxAMatrix tempBoneTransforms;

	//UINT boneIndex;

	if (m_pAnimData != nullptr)
	{
		for (int index = 0; index < m_pAnimData->GetBoneCount(); index++)
		{
			//LSH 16'.04.17
			/*
			애니메이션 임시로 막음.
			*/

			tempBoneTransforms.SetIdentity();

			//TransformationOfPoseAtTimeT
			tempBoneTransforms
				*= m_pAnimData->GetSkeletonKeyframeGlobalTransform(index, m_currentKeyFrameNum);

			//GlobalBindposeInverse
			tempBoneTransforms
				*= m_pAnimData->GetBoneKeyframedata(index).mGlobalBindposeInverse;


			XMStoreFloat4x4(&(gmtxbtf->gmtxBoneTransforms[index]),
				XMMatrixTranspose(XMMATRIX(
				(float)tempBoneTransforms.Get(0, 0), (float)tempBoneTransforms.Get(0, 1), (float)tempBoneTransforms.Get(0, 2), (float)tempBoneTransforms.Get(0, 3),
					(float)tempBoneTransforms.Get(1, 0), (float)tempBoneTransforms.Get(1, 1), (float)tempBoneTransforms.Get(1, 2), (float)tempBoneTransforms.Get(1, 3),
					(float)tempBoneTransforms.Get(2, 0), (float)tempBoneTransforms.Get(2, 1), (float)tempBoneTransforms.Get(2, 2), (float)tempBoneTransforms.Get(2, 3),
					(float)tempBoneTransforms.Get(3, 0), (float)tempBoneTransforms.Get(3, 1), (float)tempBoneTransforms.Get(3, 2), (float)tempBoneTransforms.Get(3, 3))));
		}
	}

	pd3dDeviceContext->Unmap(m_pcbBoneTransform, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_BONE_TRANSFORM, 1, &m_pcbBoneTransform);
}

void CWarp3DSkinningShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
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
			m_pAnimData->GetBlendingIndexWeightPair(m_pModelData->GetModelInfo().GetFaceIndex(k));

		puiBlendingindices[k] =
			XMUINT4(m_pAnimData->GetBoneListIndex(tmpBlendingInfo[0].first),
				m_pAnimData->GetBoneListIndex(tmpBlendingInfo[1].first),
				m_pAnimData->GetBoneListIndex(tmpBlendingInfo[2].first),
				m_pAnimData->GetBoneListIndex(tmpBlendingInfo[3].first));
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

void CWarp3DSkinningShader::ReleaseObjects()
{
	CObjectsShader::ReleaseObjects();

	if (m_pd3dInstanceBuffer) m_pd3dInstanceBuffer->Release();
}

void CWarp3DSkinningShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (bBoneTransformSetting) CWarp3DSkinningShader::UpdateConstBuffer(pd3dDeviceContext);

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
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
#else
		if (pObject->IsVisible(pCamera))
			pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
#endif

	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	//KYT '16.02.05 
	/*아직 삭제하기에는 이름*/
#ifdef _SPACEPARTITIONING
	m_vRenderObject.clear();
#endif
}

void CWarp3DSkinningShader::AnimateObjects(float fTimeElapsed)
{
	CInstancedObjectsShader::AnimateObjects(fTimeElapsed);
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

CWarp3DSkinningShaderforQueryChan::CWarp3DSkinningShaderforQueryChan(int nObjects) : CWarp3DSkinningShader(nObjects)
{
	m_pModelData = new CWarp3DModel(&(MODELMGR->GetModelData("../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d")));
	//*m_pModelData = MODELMGR->GetModelData("../Assets/02_ModelData/03_Monster/Zombunny00.warp3d");

	m_pAnimData = new CWarpAnimation(&(MODELMGR->GetAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[running_backward_inPlace]_boneModify.warpanim")));

	m_currentKeyFrameNum = 0;
	//*m_pAnimData = MODELMGR->GetAnimationData("../Assets/02_ModelData/03_Monster/Zombunny.warpanim");
}

void CWarp3DSkinningShaderforQueryChan::AnimateObjects(float fTimeElapsed)
{
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

	//CInstancedObjectsShader::AnimateObjects(fTimeElapsed);
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
