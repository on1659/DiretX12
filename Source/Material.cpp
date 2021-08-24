#include "stdafx.h"
#include "Material.h"
#include "MaterialColors.h"

CMaterial::CMaterial()
{
	m_pColors = std::make_shared<CMaterialColors>();
	m_pTexture = nullptr;
}


CMaterial::CMaterial(std::shared_ptr<CMaterialColors> pColors)
{
	m_pColors = move(pColors);
	pColors.reset();
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetTexture(std::shared_ptr<CTexture> pTexture)
{
	m_pTexture = pTexture;
}

void CMaterial::SetMaterialColor(std::shared_ptr<CMaterialColors> pMaterialColor)
{
	m_pColors = pMaterialColor;
}



void CMaterial::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pColors) CGameObject::UpdateConstBuffer(pd3dDeviceContext, m_pColors.get());
	if (m_pTexture) m_pTexture->UpdateConstBuffer(pd3dDeviceContext);
}
