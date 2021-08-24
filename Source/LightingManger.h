#pragma once

class CLightingManger : public CSingleTonBase<CLightingManger>
{
private:
	
	LIGHTS_Dynamic*			m_pDynamicLights;
	static ID3D11Buffer*	m_pd3dcbDynamicLights;

	LIGHTS*					m_pStaticLights;
	static ID3D11Buffer*	m_pd3dcbStaticLights;
	
	bool					m_load;

	float					m_nShadowPosHeight;
public:
	
	CLightingManger();

	~CLightingManger();

	void ReleseInstance();

	void Load(ID3D11Device* pd3dDevice);

	void CreateConstBuffers(ID3D11Device *pd3dDevice);

	void Update(float fTimeElapsed, std::shared_ptr<CCamera> pCamera);

	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	void DebugLight(ID3D11DeviceContext *pd3dDeviceContext);

	static void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS* pLights);

	LIGHTS* GetStaticLights(){ return m_pStaticLights; }

	LIGHTS_Dynamic* GetDynamicLights() { return m_pDynamicLights; }


};

