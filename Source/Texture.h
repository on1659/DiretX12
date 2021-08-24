#pragma once

#include "MaterialColors.h"

class CTexture : public Object
{
public:
	CTexture(int nTextures = 1, int nSamplers = 1, int nTextureStartSlot = 0, int nSamplerStartSlot = 0);
	virtual ~CTexture();

private:

	int								m_nTextures;
	ID3D11ShaderResourceView		**m_ppd3dsrvTextures;
	int								m_nTextureStartSlot;

	int								m_nSamplers;
	ID3D11SamplerState				**m_ppd3dSamplerStates;
	int								m_nSamplerStartSlot;

public:
	void SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture);
	void SetSampler(int nIndex, ID3D11SamplerState *pd3dSamplerState);
	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);
	void UpdateTextureBuffer(ID3D11DeviceContext *pd3dDeviceContext, int nIndex = 0, int nSlot = 0);
	void UpdateComputeResource(ID3D11DeviceContext *pd3dDeviceContext, int nIndex = 0, int nSlot = 0);
	void UpdateSamplerBuffer(ID3D11DeviceContext *pd3dDeviceContext, int nIndex = 0, int nSlot = 0);

	static ID3D11Buffer				*m_pd3dcbTextureMatrix;

	static void CreateConstBuffers(ID3D11Device *pd3dDevice);
	static void ReleaseConstBuffers();
	static void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxTexture);
	static void UpdateDomainConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxTexture);
	static ID3D11ShaderResourceView *CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, _TCHAR(*ppstrFilePaths)[128], UINT nTextures);
};


