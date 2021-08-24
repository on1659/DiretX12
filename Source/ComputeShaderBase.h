#pragma once

#include "stdafx.h"

class CComputeShaderBase : public CShader
{
private:
	size_t m_ThreadGroupX;
	size_t m_ThreadGroupY;
	size_t m_ThreadGroupZ;

public:
	
	CComputeShaderBase() : CShader()
	{	
	}

	virtual ~CComputeShaderBase()
	{
	}

	virtual void ReleaseShaderVariables() {}

	virtual void CreateShader(ID3D11Device *pd3dDevice) {}

	virtual void UpdateShader(ID3D11DeviceContext *pd3dDeviceContext) {}

	virtual void UpdateComputeShader(ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView* pd3dsrvLDR) {}

protected:

	void SetThreadGroup(const size_t& x, const size_t& y, const size_t& z)
	{
		m_ThreadGroupX = x;
		m_ThreadGroupY = x;
		m_ThreadGroupZ = z;
	}

	void Dispatch(ID3D11DeviceContext *pd3dDeviceContext)
	{
		pd3dDeviceContext->Dispatch(m_ThreadGroupX, m_ThreadGroupY, m_ThreadGroupZ);

	}

};