#pragma once

#include "InstancedObjectsShader.h"


class Component;
class UIState;

class CUIShader : public CInstancedObjectsShader
{

protected:

	struct UIPosition
	{
		std::shared_ptr<CGameObject>	gameObject;
		RECT							rect;
	};

	//최조 한번만 해주면되서 이걸로 가지고있는다.
	bool							m_bInitallyShader;
	UIPosition						m_uUIPosition;
	//KYT '16.05.15
	/*
	UI가 보일지 안보일지 확인
	*/
	bool							m_bVisible;

	float							m_fWidth;
	float							m_fHeight;

public:
	CUIShader(int nObjects = 1);

	virtual ~CUIShader();

	void ReleseInstance();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

	virtual void CreateAfterShader(ID3D11Device *pd3dDevice, bool isGaugeBar);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera = nullptr);

	void RenderTransparent(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera = nullptr);

	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	void OffSetPosition(float dx, float dy)
	{
		m_uUIPosition.gameObject->m_d3dxmtxWorld._41 += dx;
		m_uUIPosition.gameObject->m_d3dxmtxWorld._42 += dy;
		m_uUIPosition.gameObject->m_d3dxmtxWorld._43 = 0;


		m_uUIPosition.rect.left += dx;
		m_uUIPosition.rect.right += dx;
		m_uUIPosition.rect.top += dy;
		m_uUIPosition.rect.bottom += dy;

		m_bInitallyShader = true;
	}

	void SetPosition(float cx, float cy, float widht, float height);

	void SetPosition(float cx, float cy);

	void SetPosition(RECT rect);

	void ReSetPosition(RECT rect);

	bool Contain();

	bool Contain(POINT pt);

	bool Contain(int x, int y);

	bool MouseLeftClick();

	bool MouseRightClick();

	void SetActivie(bool flag) { m_bVisible = flag; }

	void Animate(float fTimeElapsed);

	void Start();

	void SetUIShader(std::shared_ptr<CUIShader> shader);

	void SetComponet(std::unique_ptr<Component> component);

	RECT GetPosition() const { return m_uUIPosition.rect; }

	UIState* GetUIStateComponet() const { return (UIState*)m_uUIPosition.gameObject->GetComponent(0); }

	virtual void OffSetSize(ID3D11DeviceContext* pd3dDeviceContext, float x, float y, float z = 0.0f);

	virtual void SetSize(ID3D11DeviceContext* pd3dDeviceContext, float x, float y, float z = 0.0f);

	XMFLOAT4 GetPosXMFLOAT4();


	float GetWidth() const { return m_fWidth; }
	float GetHeight() const { return m_fHeight; }
};

