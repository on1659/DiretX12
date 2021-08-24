#pragma once

#include "Texture.h"

class CMaterial : public Object
{
public:

	CMaterial();

	CMaterial(std::shared_ptr<CMaterialColors> pColors);

	virtual ~CMaterial();
	
	void SetTexture(std::shared_ptr<CTexture> pTexture);

	void SetMaterialColor(std::shared_ptr<CMaterialColors> pMaterialColor);

	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	std::shared_ptr<CMaterialColors>	m_pColors;

	std::shared_ptr<CTexture>			m_pTexture;

};