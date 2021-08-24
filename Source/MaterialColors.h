#pragma once
class CMaterialColors : public Object
{
public:
	CMaterialColors();

	virtual ~CMaterialColors();

public:
	XMFLOAT4						m_d3dxcAmbient;
	XMFLOAT4						m_d3dxcDiffuse;
	XMFLOAT4						m_d3dxcSpecular; //(r,g,b,a=power)
	XMFLOAT4						m_d3dxcEmissive;
};
