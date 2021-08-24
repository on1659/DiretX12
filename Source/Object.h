//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

//추후에 UINT 바꿀 예정
//아니면 Define로 
class Object
{

public:
	//KYT '16.07.26
	/*
		GameObject Global ID;
	*/
	int				nGID;

	std::string name;
	UINT tag;
	UINT layer;

	//KYT '16.05.05
	/*
	  warning를 없애기 위하여 리턴형을 위한 xm형 지원
	*/
protected:
	XMMATRIX						m_xmMatrix;
	XMVECTOR						m_xmVector;
	XMFLOAT3						m_xmFloat3;
	XMFLOAT4X4						m_xmFLOAT4X4;
	std::string						m_strBlendstate;
//private:	
//	int								m_nReferences;
//
//public:
//	void AddRef() { m_nReferences++; }
//
//	void Release(){	if (--m_nReferences <= 0) delete this; }

public:

	Object();

	Object(std::string name, UINT tag = 0);

	virtual ~Object();

	virtual void Release();

	void SetTag(UINT tag){ this->tag = tag; }

	void SetLayer(UINT layer){ this->layer = layer; }

	UINT GetTag(){ return tag; };

	UINT GetLayer(){ return layer; };

protected:
	ID3D11RasterizerState			*m_pd3dRasterizerState;
	ID3D11DepthStencilState			*m_pd3dDepthStencilState;
	ID3D11BlendState				*m_pd3dBlendState;


//KYT '16.02.08
/*State*/

public:

	virtual void CreateBlendState(ID3D11Device *pd3dDevice);

	void CreateBlendState(ID3D11Device *pd3dDevice, D3D11_BLEND_DESC desc);

	virtual void CreateDepthStencilState(ID3D11Device *pd3dDevice);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice, D3D11_CULL_MODE cullMode = D3D11_CULL_NONE, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID);

	virtual void SetBlendState(ID3D11BlendState *pd3dBlendState);

	void SetBlendState(std::string name);

	std::string GetBlendStatePath() const { return m_strBlendstate;  }

};