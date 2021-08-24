#pragma once

#include "UIShader.h"


class CUIGauge : public Object
{
private:
	struct cbUIConstBuffer
	{
		float gUIGage_RIGHT;
		XMFLOAT3 padding;
	};

	ID3D11Buffer*						m_pd3dcbGaugeBuffer;
	float								m_fPercent;
	float								m_fSpeed;


	std::shared_ptr<CUIShader>			m_uiUIGauge;
	std::shared_ptr<CUIShader>			m_uiUIGaugeBackground;

public:
	CUIGauge();

	virtual ~CUIGauge();

	void Initialize(ID3D11Device * pd3dDevice, XMFLOAT2 position, XMFLOAT2 fSize, float frameFPS);

	void CreateConstBuffer(ID3D11Device* pd3dDevice);

	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	void Start() { m_fPercent = 1.0f; }

	bool IsEnd() const { return(m_fPercent < 0.0f); }

	int GetPercent() const { return m_fPercent * 100; }
	int SetPercent(int percent){ m_fPercent = percent * 0.01f; }

	void Render(ID3D11DeviceContext* pd3dDeviceContext);
};

