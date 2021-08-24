#pragma once

class CPositionObject : public Object
{
public:
	XMFLOAT4X4						m_d3dxmtxLocal;
	XMFLOAT4X4						m_d3dxmtxWorld;

public:
	CPositionObject();
	~CPositionObject();

public:
	void SetPosition(float x, float y, float z = 0);
};

