#pragma once

#include "UIShader.h"

class CBulletinShader : public CInstancedObjectsShader
{
private:
	std::vector<std::shared_ptr<CUIShader>>		m_vUIShader;

public:
	CBulletinShader();

	~CBulletinShader();

	template <class BulletInfo>
	void LoadPlayer(const BulletInfo& info) { m_vUIShader.push_back(info); }

	template <class First, class... Rest>
	void LoadPlayer(const First& first, const Rest&... rest)
	{
		m_vUIShader.push_back(first);
		LoadPlayer(rest...); // recursive call using pack expansion syntax
	}

	virtual void CreateShader(ID3D11Device *pd3dDevice);




};

