#pragma once

#include "Shader.h"

struct CB_PARTICLEINFO
{
	XMFLOAT3 m_d3dxvEmitPosition;
	float m_fGameTime;
	XMFLOAT3 m_d3dxvEmitDirection;
	float m_fTimeStep;
	XMFLOAT3 m_d3dxvAcceleration;
	UINT m_uiTextureType;
	float fSpeed;
	float fSize;
	float fDuringTime;
	float fEmitCount;
	//XMFLOAT4 m_xmCameraPosition;
};

struct CParticleVertex
{
	XMFLOAT3 m_d3dxvPosition;
	XMFLOAT3 m_d3dxvVelocity;
	XMFLOAT2 m_d3dxvSize;
	float m_fAge;
	UINT m_nType;
};

class CCubeParticleShader : public CShader
{
protected:
	UINT							m_nMaxParticles;			// 최대 파티클
	bool							m_bInitializeParticle;		// Emitter 파티클 생성 신호
	float							m_fGameTime;				// ElapsedTime 저장
	float							m_fTimeStep;				// 흘러간 시간 기록
	float							m_fAge;						// 파티클의 소멸 Age
	XMFLOAT3						m_d3dxvAcceleration;		// 파티클 속도
	XMFLOAT3						m_d3dxvEmitPosition;		// 파티클 포지션
	XMFLOAT3						m_d3dxvEmitDirection;		// 파티클 방향

																//ID3D11Buffer *m_pd3dcbCameraPosition;
	ID3D11Buffer					*m_pd3dcbParticleInfo;

	UINT							 m_nOffset;
	UINT							 m_nStride;

	// 이하는 쉐이더 설정과 관련된 객체들이다.
	ID3D11Buffer					*m_pd3dInitialVertexBuffer;	// 시작점 버텍스w버퍼
	ID3D11Buffer					*m_pd3dStreamOutVertexBuffer;	// 스트림아웃 버텍스버퍼
	ID3D11Buffer					*m_pd3dDrawVertexBuffer;		//

	ID3D11VertexShader				*m_pd3dSOVertexShader;	// StreamOut용도의 VS
	ID3D11GeometryShader			*m_pd3dSOGeometryShader;

	ID3D11DepthStencilState			*m_pd3dSODepthStencilState;
	ID3D11DepthStencilState			*m_pd3dDepthStencilState;

	ID3D11ShaderResourceView		*m_pd3dsrvRandomTexture;
	ID3D11SamplerState				*m_pd3dSamplerState;
	ID3D11ShaderResourceView		*m_pd3dsrvTextureArray;


	float							 m_fSize;
	float							 m_fSpeed;
	float							 m_fDuringTime;
	float							 m_fEmitCount;
	XMFLOAT3						m_fRandPosition;


public:
	operator const bool() { return (m_fGameTime > m_fDuringTime); }

	bool IsActivity()
	{
		if (m_fGameTime > m_fDuringTime)
			return true;
		return false;
	}

public:
	CCubeParticleShader();
	virtual ~CCubeParticleShader();
	virtual void ReleaseShader();

	virtual void Initialize(ID3D11Device *pd3dDevice, TCHAR *pszFileName, XMFLOAT3 d3dxvPosition, UINT nMaxParticles);
	virtual void CreateParticle(ID3D11Device *pd3dDevice, XMFLOAT3 &pd3dxvPosition, XMFLOAT3 &pd3dxvDirection, XMFLOAT3 &pd3dxvAccelerator);

	virtual void SetParitlceData(float fSpeed, float fSize, float fAge, float fEmitCount = 1, XMFLOAT3 fRandPosition = XMFLOAT3(0.0f, 0.0f, 0.0f))
	{
		m_fSpeed = fSpeed;
		m_fSize = fSize;
		m_fDuringTime = fAge;
		m_fRandPosition = fRandPosition;
		m_fEmitCount = fEmitCount;
	}
	virtual bool Update(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void SetPosition(const XMFLOAT3& d3dxvPosition)
	{
		m_d3dxvEmitPosition.x = d3dxvPosition.x + m_fRandPosition.x;
		m_d3dxvEmitPosition.y = d3dxvPosition.y + m_fRandPosition.y;
		m_d3dxvEmitPosition.z = d3dxvPosition.z + m_fRandPosition.z;
	}
	virtual void SetPosition(const float& x, const float& y, const float& z)
	{
		m_d3dxvEmitPosition.x = x + m_fRandPosition.x;
		m_d3dxvEmitPosition.y = y + m_fRandPosition.y;
		m_d3dxvEmitPosition.z = z + m_fRandPosition.z;
	}
	virtual void ParticleRestart() { m_bInitializeParticle = true; m_fGameTime = 0.0f; }
	virtual void ParticleStop() { m_bInitializeParticle = false; }

	// 이하는 쉐이더 설정과 관련된 함수들이다.
	virtual void CreateConstBuffer(ID3D11Device *pd3dDevice);
	virtual void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dSOGeometryShader);
	virtual void CreateSOGeoMoetryShaderFromCompiledFile(ID3D11Device * pd3dDevice, WCHAR * pszFileName, ID3D11GeometryShader** ppd3dGeomeotryShader);

};