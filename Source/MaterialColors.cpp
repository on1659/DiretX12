#include "stdafx.h"
#include "MaterialColors.h"


CMaterialColors::CMaterialColors() : Object()
{
	m_d3dxcAmbient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_d3dxcDiffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_d3dxcEmissive = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}


CMaterialColors::~CMaterialColors()
{
}


