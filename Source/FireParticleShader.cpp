#include "stdafx.h"
#include "FireParticleShader.h"
#include "InstancedObjectsShader.h"

//----------------------------------------------------------------
CFireParticleShader::CFireParticleShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CParticleShader(move(pMesh), nObjects)
{

}

CFireParticleShader::CFireParticleShader(std::unique_ptr<CMesh> pMesh) : CParticleShader(move(pMesh))
{

}

CFireParticleShader::CFireParticleShader(int nObjects) : CParticleShader(nObjects)
{
	//Distortion
}

CFireParticleShader::~CFireParticleShader()
{

}

void CFireParticleShader::CreateShader(ID3D11Device *pd3dDevice)
{

	int nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//2
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/FireShader/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/FireShader/PS.fxo", &m_pd3dPixelShader);


	//CreateVertexShaderFromFile(pd3dDevice, L"Fire.fx", "ParticleFire_VS_OUTPUT", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"Fire.fx", "ParticleFire_PS_OUTPUT_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"Fire.fx", "ParticleFire_PS_OUTPUT", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

void CFireParticleShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CParticleShader::BuildObjects(pd3dDevice);

	//RasterizerState
	Object::CreateRasterizerState(pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);
	Object::SetBlendState("Billboard");

	bInit = true;

	{
		//VS  NoiseBuffer
		m_pd3dcbNoiseBufferType = nullptr;
		D3D11_BUFFER_DESC d3dBufferDesc;
		ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
		d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		d3dBufferDesc.ByteWidth = sizeof(NoiseBufferType);
		pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pd3dcbNoiseBufferType);
	}

	{
		//PS Distor
		m_pd3dcbDistortion = nullptr;
		D3D11_BUFFER_DESC d3dBufferDesc;
		ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
		d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		d3dBufferDesc.ByteWidth = sizeof(Distortion);
		pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pd3dcbDistortion);
	}


}

void CFireParticleShader::CreateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	{
		//VS
		m_NoiseBufferType.frameTime = 0.0f;
		m_NoiseBufferType.scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);
		m_NoiseBufferType.scales = XMFLOAT3(1.0f, 2.0f, 3.0f);
		m_NoiseBufferType.padding = 0.0f;

		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		pd3dDeviceContext->Map(m_pd3dcbNoiseBufferType, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		NoiseBufferType *noise = (NoiseBufferType*)d3dMappedResource.pData;
		noise->frameTime = m_NoiseBufferType.frameTime;
		noise->scrollSpeeds = m_NoiseBufferType.scrollSpeeds;
		noise->scales = m_NoiseBufferType.scales;
		noise->padding = m_NoiseBufferType.padding;
		pd3dDeviceContext->Unmap(m_pd3dcbNoiseBufferType, 0);

		pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_PARTICLE, 1, &m_pd3dcbNoiseBufferType);

	}

	{
		//PS
		m_Distortion.distortion1 = XMFLOAT2(0.1, 0.2f);
		m_Distortion.distortion2 = XMFLOAT2(0.1, 0.3f);
		m_Distortion.distortion3 = XMFLOAT2(0.1, 0.1f);
		m_Distortion.distortionScale = 0.8f;
		m_Distortion.distortionBias = 0.5f;

		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		pd3dDeviceContext->Map(m_pd3dcbDistortion, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		Distortion *distortion = (Distortion*)d3dMappedResource.pData;
		distortion->distortion1 = m_Distortion.distortion1;
		distortion->distortion2 = m_Distortion.distortion2;
		distortion->distortion3 = m_Distortion.distortion3;
		distortion->distortionScale = m_Distortion.distortionScale;
		distortion->distortionBias = m_Distortion.distortionBias;
		pd3dDeviceContext->Unmap(m_pd3dcbDistortion, 0);

		pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_PARTICLE, 1, &m_pd3dcbDistortion);
	}

	bInit = false;
}

void CFireParticleShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	{
		//VS
		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

		pd3dDeviceContext->Map(m_pd3dcbNoiseBufferType, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		NoiseBufferType *noise = (NoiseBufferType*)d3dMappedResource.pData;
		noise->frameTime = m_NoiseBufferType.frameTime;
		noise->scrollSpeeds = m_NoiseBufferType.scrollSpeeds;
		noise->scales = m_NoiseBufferType.scales;
		noise->padding = m_NoiseBufferType.padding;
		pd3dDeviceContext->Unmap(m_pd3dcbNoiseBufferType, 0);

		pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_PARTICLE, 1, &m_pd3dcbNoiseBufferType);

		pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_PARTICLE, 1, &m_pd3dcbDistortion);

	}

}

void CFireParticleShader::AnimateObjects(float fTimeElapsed)
{
	m_NoiseBufferType.frameTime += 0.01f;
	if (m_NoiseBufferType.frameTime > 1000.0f) m_NoiseBufferType.frameTime = 0.0f;

}

void CFireParticleShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{

	if (bInit)
	{
		CFireParticleShader::CreateConstBuffer(pd3dDeviceContext);
		bInit = false;
	}
	CFireParticleShader::UpdateConstBuffer(pd3dDeviceContext);
	

	OnPrepareRender(pd3dDeviceContext);
	
	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);
	
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	
	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;
	
	for (auto &pObject : m_vRenderObject)
	{
		XMMATRIX matrix = SetLookAt(pObject->GetWorldMatrixFLOAT(), pCamera->GetPositionXMFLOAT3());
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(matrix);

		pObject->SetWorldMatrix(matrix);
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);
	
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
		m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	m_vRenderObject.clear();
}

XMMATRIX CFireParticleShader::SetLookAt(XMFLOAT4X4 &xmMatrix, XMFLOAT3 d3dxvTarget)
{
	XMMATRIX matrix;
	//XMStoreFloat4x4(&matirx, xmMatrix);

	XMVECTOR d3dxvUp = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR d3dxvPosition = XMVectorSet(xmMatrix._41, xmMatrix._42, xmMatrix._43, 1.0f);
	XMVECTOR d3dxvLook = XMVector3Normalize(d3dxvPosition - XMLoadFloat3(&d3dxvTarget));

	matrix.r[0] = XMVector3Normalize(XMVector3Cross(d3dxvUp, d3dxvLook));
	matrix.r[1] = d3dxvUp;
	matrix.r[2] = d3dxvLook;
	matrix.r[3] = d3dxvPosition;

	return matrix;
}
