#include "stdafx.h"
#include "PositionObject.h"


CPositionObject::CPositionObject()
{
	XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixIdentity());

}

CPositionObject::~CPositionObject()
{
}

//Move & Rotateion Fucntion
void CPositionObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxLocal._41 = x;
	m_d3dxmtxLocal._42 = y;
	m_d3dxmtxLocal._43 = z;
}

