#pragma once

#include "GameSystem.h"

class CBasicSystem : public CGameSystem
{

private:

	int			width;
	int			height;

	UINT m_iPlayerKillCount;
	UINT m_iPlayerDeathCount;
	UINT m_iPlayerScore;

public:

	CBasicSystem();

	virtual ~CBasicSystem();

	void Load(ID3D11Device *pd3dDevice);

	bool OnCollision();

	void Release();

	void Update(float fTimeElapsed);

	void Render(ID3D11DeviceContext *pd3dDeviceContext);

	void ChangeState();

	void RenderTabUIFont();
};

