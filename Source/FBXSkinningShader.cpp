#include "stdafx.h"
#include "FBXSkinningShader.h"
#include "FBXLoader.h"


CFBXSkinnedShader::CFBXSkinnedShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	//20160319 임시로 선언하여 사용. 원래는 씬에서 선언한 FBXAnimationModelLoader를 가져오거나 매니저를 만들어야한다.
	//m_pSkinnedModel = new FBXAnimationModelLoader(ANIM_MODEL_PATH, false);
	//m_pSkinnedModel->ProcessAnimationInfo();

	bBoneTransformSetting = false;

	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;

	m_currentKeyFrameNum = 0;
}

CFBXSkinnedShader::CFBXSkinnedShader(int nObjects) : CInstancedObjectsShader(nObjects)
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	//20160319 임시로 선언하여 사용. 원래는 씬에서 선언한 FBXAnimationModelLoader를 가져오거나 매니저를 만들어야한다.
	//m_pSkinnedModel = new FBXAnimationModelLoader(ANIM_MODEL_PATH, false);
	//m_pSkinnedModel->ProcessAnimationInfo();

	bBoneTransformSetting = false;

	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;

	m_currentKeyFrameNum = 0;
}

CFBXSkinnedShader::CFBXSkinnedShader(FBXAnimationModelLoader fbxLoader, int nObjects) : CInstancedObjectsShader(nObjects)
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	//20160319 임시로 선언하여 사용. 원래는 씬에서 선언한 FBXAnimationModelLoader를 가져오거나 매니저를 만들어야한다.
	m_pSkinnedModel = &fbxLoader;
	m_pSkinnedModel->ProcessAnimationInfo();

	bBoneTransformSetting = false;

	m_pd3dBlendingIndexBuffer = nullptr;
	m_pd3dBlendingWeightBuffer = nullptr;

	m_currentKeyFrameNum = 0;
}

CFBXSkinnedShader::~CFBXSkinnedShader()
{
}


void CFBXSkinnedShader::CreateShader(ID3D11Device *pd3dDevice)
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


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/SkinnedModel/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/SkinnedModel/PS.fxo", &m_pd3dPixelShader);


	//CreateVertexShaderFromFile(pd3dDevice, L"FBXSkinningInstanced.fx", "VSSkinnedInstanced", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"FBXSkinningInstanced.fx", "PSSkinnedInstanced_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"FBXSkinnedInstanced.fx", "PSSkinnedInstanced", "ps_5_0", &m_pd3dPixelShader);
	//#endif

}

void CFBXSkinnedShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pcbBoneTransform, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	/*
	이 구간에서 상수버퍼로 들어갈 본 행렬을 정한다.
	애니메이션이 재생 중이라면, 키프레임 마다 저장되어있는 본 변환행렬을 끄집어 와야된다.
	*/

	GlobalMatrixBoneTransforms* gmtxbtf = (GlobalMatrixBoneTransforms*)d3dMappedResource.pData;

	FbxAMatrix tempBoneTransforms;

	if (m_pSkinnedModel->HasAnitamionKeyframe())
	{
		for (int jointIndex = 0; jointIndex < m_pSkinnedModel->GetSkeletonJointCount(); jointIndex++)
		{
			//LSH 16'.04.17
			/*
			애니메이션 임시로 막음.
			*/
			tempBoneTransforms.SetIdentity();
			
			//TransformationOfPoseAtTimeT
			tempBoneTransforms
				*= m_pSkinnedModel->GetSkeletonJointGlobalTransform(jointIndex, (int)m_currentKeyFrameNum);
			
			//InverseOfGlobalBindPoseMatrix
			tempBoneTransforms
				*= m_pSkinnedModel->GetSkeletonJoint(jointIndex).mGlobalBindposeInverse;
			
			XMStoreFloat4x4(&(gmtxbtf->gmtxBoneTransforms[jointIndex]),
				XMMatrixTranspose(XMMATRIX(
					(float)tempBoneTransforms.Get(0, 0), (float)tempBoneTransforms.Get(0, 1), (float)tempBoneTransforms.Get(0, 2), (float)tempBoneTransforms.Get(0, 3),
					(float)tempBoneTransforms.Get(1, 0), (float)tempBoneTransforms.Get(1, 1), (float)tempBoneTransforms.Get(1, 2), (float)tempBoneTransforms.Get(1, 3),
					(float)tempBoneTransforms.Get(2, 0), (float)tempBoneTransforms.Get(2, 1), (float)tempBoneTransforms.Get(2, 2), (float)tempBoneTransforms.Get(2, 3),
					(float)tempBoneTransforms.Get(3, 0), (float)tempBoneTransforms.Get(3, 1), (float)tempBoneTransforms.Get(3, 2), (float)tempBoneTransforms.Get(3, 3))));

			XMStoreFloat4x4(&(XMFLOAT4X4)(gmtxbtf->gmtxBoneTransforms[jointIndex]),XMMatrixIdentity());
		}
	}
	pd3dDeviceContext->Unmap(m_pcbBoneTransform, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_BONE_TRANSFORM, 1, &m_pcbBoneTransform);
}

void CFBXSkinnedShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CObjectsShader::BuildObjects(pd3dDevice, pContext);

	int m_nControlPoints = m_pSkinnedModel->GetIndiecesCount();

	XMUINT4 *puiBlendingindices;
	puiBlendingindices = new XMUINT4[m_nControlPoints];

	XMFLOAT3 *pxvBlendingWeights;
	pxvBlendingWeights = new XMFLOAT3[m_nControlPoints];

	int *m_fbxIndices = m_pSkinnedModel->GetIndices();
	int nBlendingInfo = 0;

	for (int k = 0; k < m_nControlPoints; k++)
	{
		auto tmpBlendingInfo = m_pSkinnedModel->GetBlendingIndexWeightPair(m_fbxIndices[k]);

		nBlendingInfo = tmpBlendingInfo.size();


		switch (nBlendingInfo)
		{
			case 1:
				puiBlendingindices[k] =
					XMUINT4(tmpBlendingInfo[0].first, 0, 0, 0);
				pxvBlendingWeights[k] =
					XMFLOAT3(tmpBlendingInfo[0].second, 0.0f, 0.0f);
				break;
			case 2:
				puiBlendingindices[k] =
					XMUINT4(tmpBlendingInfo[0].first, tmpBlendingInfo[1].first, 0, 0);
				pxvBlendingWeights[k] =
					XMFLOAT3(tmpBlendingInfo[0].second, tmpBlendingInfo[1].second, 0.0f);
				break;
			case 3:
				puiBlendingindices[k] =
					XMUINT4(tmpBlendingInfo[0].first, tmpBlendingInfo[1].first, tmpBlendingInfo[2].first, 0);
				pxvBlendingWeights[k] =
					XMFLOAT3(tmpBlendingInfo[0].second, tmpBlendingInfo[1].second, tmpBlendingInfo[2].second);
				break;
			case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
				puiBlendingindices[k] =
					XMUINT4(tmpBlendingInfo[0].first, tmpBlendingInfo[1].first, tmpBlendingInfo[2].first, tmpBlendingInfo[3].first);
				pxvBlendingWeights[k] =
					XMFLOAT3(tmpBlendingInfo[0].second, tmpBlendingInfo[1].second, tmpBlendingInfo[2].second);
				break;
			case 0:
			default:
				puiBlendingindices[k] = XMUINT4(0, 0, 0, 0);
				pxvBlendingWeights[k] = XMFLOAT3(0.0f, 0.0f, 0.0f);
				break;
		}
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

void CFBXSkinnedShader::ReleaseObjects()
{
	CObjectsShader::ReleaseObjects();

	if (m_pd3dInstanceBuffer) m_pd3dInstanceBuffer->Release();
}

//요기야
void CFBXSkinnedShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	return;

	if (bBoneTransformSetting) CFBXSkinnedShader::UpdateConstBuffer(pd3dDeviceContext);

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
	m_vRenderObject.clear();
}

void CFBXSkinnedShader::AnimateObjects(float fTimeElapsed)
{
	//if (m_currentKeyFrameNum >= m_pSkinnedModel->GetAnimEndTime() - 1)
	//{
	//	m_currentKeyFrameNum = m_pSkinnedModel->GetAnimStartTime();
	//}
	//else
	//{
	//	//RSH 16'.04.16
	//	/*
	//	키프레임 수정
	//	*/
	//	m_currentKeyFrameNum += 30.0f*fTimeElapsed;
	//	if (m_currentKeyFrameNum >= m_pSkinnedModel->GetAnimEndTime() - 1)
	//	{
	//		m_currentKeyFrameNum = m_pSkinnedModel->GetAnimEndTime() - 1;
	//	}
	//}

	CInstancedObjectsShader::AnimateObjects(fTimeElapsed);
}

CFBXSkinnedShaderforZombunny::CFBXSkinnedShaderforZombunny(int nObjects)
	: CFBXSkinnedShader(nObjects)
{
	m_pSkinnedModel = nullptr;

	//20160319 임시로 선언하여 사용. 원래는 씬에서 선언한 FBXAnimationModelLoader를 가져오거나 매니저를 만들어야한다.
	m_pSkinnedModel = new FBXAnimationModelLoader("../Assets/02_ModelData/03_Monster/Zombunny.FBX", BUNNY_SCALE, false);
	m_pSkinnedModel->ProcessAnimationInfo();


	m_currentKeyFrameNum = 0;
}

CFBXSkinnedShaderforRunningZombunny::CFBXSkinnedShaderforRunningZombunny(int nObjects)
	: CFBXSkinnedShader(nObjects)
{
	m_pSkinnedModel = nullptr;

	//20160319 임시로 선언하여 사용. 원래는 씬에서 선언한 FBXAnimationModelLoader를 가져오거나 매니저를 만들어야한다.
	m_pSkinnedModel = new FBXAnimationModelLoader("../Assets/02_ModelData/03_Monster/Zombunny_running.fbx", BUNNY_SCALE, SH_FBXTYPE::FBX_MODEL);
	m_pSkinnedModel->ProcessAnimationInfo();


	m_currentKeyFrameNum = 0;
}

void CFBXSkinnedShaderforRunningZombunny::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (bBoneTransformSetting) CFBXSkinnedShader::UpdateConstBuffer(pd3dDeviceContext);

	CTexture::UpdateConstBuffer(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxTexture));

	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	std::cout << "name : " << name << std::endl;
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

void CFBXSkinnedShaderforRunningZombunny::AnimateObjects(float fTimeElapsed)
{
	if (m_currentKeyFrameNum >= m_pSkinnedModel->GetAnimEndTime() - 1)
	{
		m_currentKeyFrameNum = m_pSkinnedModel->GetAnimStartTime();
	}
	else
	{
		//RSH 16'.04.16
		/*
		키프레임 수정
		*/
		m_currentKeyFrameNum += 30.0f*fTimeElapsed;
		if (m_currentKeyFrameNum >= m_pSkinnedModel->GetAnimEndTime() - 1)
		{
			m_currentKeyFrameNum = m_pSkinnedModel->GetAnimEndTime() - 1;
		}
	}

	CInstancedObjectsShader::AnimateObjects(fTimeElapsed);
}

CFBXSkinnedShaderforPlayerCharacter::CFBXSkinnedShaderforPlayerCharacter(int nObjects)
	: CFBXSkinnedShader(nObjects)
{
	m_pSkinnedModel = nullptr;

	//20160319 임시로 선언하여 사용. 원래는 씬에서 선언한 FBXAnimationModelLoader를 가져오거나 매니저를 만들어야한다.
	m_pSkinnedModel = new FBXAnimationModelLoader("../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_running.fbx", PLAYER_SCALE, SH_FBXTYPE::FBX_MODEL);
	m_pSkinnedModel->ProcessAnimationInfo();


	m_currentKeyFrameNum = 0;
}

