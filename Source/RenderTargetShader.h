#pragma once


//KYT '17.04.15
/*
����
1. SetScreenPos���� ȭ���� ���� �ؽ��� ��ġ�� �����(center,size �Լ�)
�� -1.0 ~ 1.0���� ���� �ʾƵ� ��,
��  center,size�� �־��ָ� UV�� �˾Ƽ� �����
�� �� �Լ��� ȣ������ ������ �˾Ƽ� -1 ~ 1�� ������
2. SetSRV�� ȭ�鿡 ����� SRV�� �Է¹���
3. Render���� ���� RTV�� Render�� ������
4. Shader HLSL�� ÷�ε� HLSL ��� ����
4. FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT�� ȭ�� ��ü ũ�⸦ �޾ƾ���.
*/


class CRenderTargetShader : public CShader
{

	struct VS_INPUT
	{
		XMFLOAT4 xf4_Pos;
		XMFLOAT2 xf2_UV;
	};


	WCHAR*	 m_wcVSPath;
	WCHAR*	 m_wcPSPath;

public:

	void SetVSName(WCHAR* path) { m_wcVSPath = path; }

	void SetPSName(WCHAR* path) { m_wcPSPath = path; }

public:

	CRenderTargetShader();

	~CRenderTargetShader();

	void ReleaseShaderVariables() override;

	void CreaetShader(ID3D11Device* pd3dDevice);

	void Render(ID3D11DeviceContext* pd3dDeviceContext);

	void SetSRV(ID3D11ShaderResourceView* pd3dSRV)
	{
		if (m_pd3dShaderResourceView) m_pd3dShaderResourceView->Release();
		m_pd3dShaderResourceView = nullptr;
		m_pd3dShaderResourceView = pd3dSRV;
	}

	// NDC��ǥ�谡 �ƴ� �׳� ȭ�� ��ǥ�踦 �Է��ϸ� �˾Ƽ� �����
	void SetScreenPos(const XMFLOAT2& center, const XMFLOAT2& size, bool init = true)
	{

		XMFLOAT2 half_size;
		half_size.x = size.x * 0.5f;
		half_size.y = size.y * 0.5f;

		SetScreenPos(m_inputVS_Screen[0], XMFLOAT2{ center.x - half_size.x, center.y + half_size.y });
		SetScreenPos(m_inputVS_Screen[1], XMFLOAT2{ center.x + half_size.x, center.y + half_size.y });
		SetScreenPos(m_inputVS_Screen[2], XMFLOAT2{ center.x - half_size.x, center.y - half_size.y });
		SetScreenPos(m_inputVS_Screen[3], XMFLOAT2{ center.x + half_size.x, center.y - half_size.y });

		if (init)
		{
			for (int i = 0; i < 4; ++i)
			{
				m_initVS_Screen[i] = m_inputVS_Screen[i];
			}
			rect_update();
		}


	}

	void SetDeviceContext(ID3D11DeviceContext* pd3dDeviceContext)
	{
		m_pd3dDeviceContext = pd3dDeviceContext;
	}

	void SetScreenPos(XMFLOAT2* f2)
	{
		if (nullptr == f2) return;

		for (int i = 0; i < 4; ++i)
		{
			SetScreenPos(m_inputVS_Screen[i], f2[i]);
		}
	}

	void SetScreenPos(XMFLOAT4& f4, const XMFLOAT2& xmf2)
	{
		if (xmf2.x > 1.0f)
		{
			f4.x = (xmf2.x * 2 / FRAME_BUFFER_WIDTH) - 1.0f;
			f4.y = (xmf2.y * 2 / FRAME_BUFFER_HEIGHT) - 1.0f;
			f4.y *= -1;
		}
		f4.z = 0.0f;
		f4.w = 1.0f;
	}

	void ExpandRTV()
	{
		ExpandRTV(INPUT->GetMousePoint());
	}

	void ExpandRTV(const POINT& pt)
	{
		ExpandRTV(pt.x, pt.y);
	}

	void ExpandRTV(const float& x, const float& y);

	bool Contain(const float& x, const float& y)
	{
		return (m_rRect.left <= x) && (x < m_rRect.right) && (m_rRect.top <= y) && (y < m_rRect.bottom);
	}

	bool IsExapnd() const { return m_bExpand; }

	bool GetIsNewRTV() const { return m_bUseNewRTV; }

	void SetNewRTV(const bool& use = true) { m_bUseNewRTV = use; }

	void SetRTV(ID3D11RenderTargetView* pd3dRTV) { m_pd3dDestRTV = pd3dRTV; }

	ID3D11RenderTargetView* GetRTV() { return m_pd3dDestRTV; }

private:

	void rect_update()
	{
		float rx, ry, hw, hh;
		rx = m_inputVS_Screen[0].x;
		ry = m_inputVS_Screen[3].y;
		hw = FRAME_BUFFER_WIDTH / 2;
		hh = FRAME_BUFFER_HEIGHT / 2;

		m_rRect.left = hw * (rx + 1);
		m_rRect.top = hh * (1 - ry);

		//m_rRect.left   = ( 1 + m_inputVS_Screen[0].x) * FRAME_BUFFER_WIDTH;
		//m_rRect.top    = ( 1 - m_inputVS_Screen[0].y) * FRAME_BUFFER_HEIGHT;

		rx = m_inputVS_Screen[3].x;
		ry = m_inputVS_Screen[0].y;

		m_rRect.right = hw * (rx + 1);
		m_rRect.bottom = hh * (1 - ry);


		//std::cout << "Rect (" << m_rRect.left << "," << m_rRect.top << ") - (" << m_rRect.right << "," << m_rRect.bottom << ")'\n";

		//m_rRect.right  = ( 1 + m_inputVS_Screen[3].x) * FRAME_BUFFER_WIDTH;
		//m_rRect.bottom = ( 1 - m_inputVS_Screen[3].y) * FRAME_BUFFER_HEIGHT;

		//m_rRect.top *= -1;
		//m_rRect.bottom *= -1;
	}

private:
	//CTexture�� �ϴ� �ݴ�پ餷���γ���
	ID3D11ShaderResourceView			*m_pd3dShaderResourceView;
	ID3D11SamplerState					*m_pd3dSamplerStates;

	ID3D11Buffer						*m_pd3dVertexBuffer;
	UINT								*m_pnVertexStrides;
	UINT								*m_pnVertexOffsets;

	XMFLOAT4							m_inputVS_Screen[4];
	XMFLOAT4							m_initVS_Screen[4];

	RECT								m_rRect;
	bool								m_bExpand;

	ID3D11DeviceContext*				m_pd3dDeviceContext;


	//RTV Set
	bool								m_bUseNewRTV;


	ID3D11RenderTargetView				*m_pd3dDestRTV;

};
