#pragma once

#include "stdafx.h"
#include "HeightMapTerrain.h"

class CRangeMove : public Component
{
public:
	CRangeMove(std::string _name = "")
	{
		name = "RangeMove";
	}

	virtual ~CRangeMove(){}

private:
	bool						m_bInit;
	XMVECTOR					m_xmInitPosition;
	XMVECTOR					m_xmRange;


	//
	float						fSpeed;
	XMVECTOR					fDir;

	//ÁöÇü
	CHeightMapTerrain*			m_Terrain;


public:

	virtual void Start(std::shared_ptr<CGameObject> m_gameObject)
	{
		Component::Start(m_gameObject);
		m_bInit = true;
		fSpeed = rand() % 50 + 250;
		fDir = XMVectorSet(RandomRange(-99, 99) * 0.01f, 0.0f, RandomRange(-99, 99) * 0.01f, 1.0f);
		fDir = XMVector3Normalize(fDir);
	}
	void SetInitPosition(XMFLOAT3 position)
	{
		m_xmInitPosition = XMLoadFloat3(&position);
		m_bInit = false;
	}
	void SetTerrain(CHeightMapTerrain* terrain)
	{
		m_Terrain = terrain;
	}
	
	virtual void Animate(float fTimeElapsed)
	{
		if (m_bInit)
		{
			m_xmInitPosition = m_gameObject->GetPosition();
			m_bInit = false;
			return;
		}
		//XMVECTOR xm = fDir;
		//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		//XMVECTOR d3dxvRotationAxis = XMVector3Cross(xm, up);
		//d3dxvRotationAxis *= -1;
		//XMFLOAT3 ax;
		//XMStoreFloat3(&ax, d3dxvRotationAxis);
		//Component::Rotate(m_gameObject, ax.x * 150.0f * fTimeElapsed, 0.0f, ax.z * 150.0f * fTimeElapsed);
		//
		//fDir = XMVector3Normalize(fDir);
		//XMVECTOR rotaAxis = XMVector3Cross(fDir, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)) * -1;
		//Component::Rotate(m_gameObject, &rotaAxis, 150.0f * fTimeElapsed);

		Component::Trasnlate(m_gameObject, fSpeed * fTimeElapsed, fDir);

		XMFLOAT3 position = m_gameObject->GetPositionXMFLOAT3();
		XMFLOAT3 dis;
		XMVECTOR distance = XMVector3Length((m_gameObject->GetPosition() - m_xmInitPosition));
		XMStoreFloat3(&dis, distance);

		if (dis.x > 15.0f || dis.x < -15.0f)
		{
			//fDir = XMVectorSet(RandomRange(-10, 10) * 0.1f, 0.0f, RandomRange(-10, 10) * 0.1f, 1.0f);
			fDir *= -1;
		}
		XMFLOAT3 dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
		if (position.x < 0 || position.x > WORLDSIZE)
		{
			XMStoreFloat3(&dir, fDir);
			dir.x *= -1;
		}
		if (position.z < 0 || position.z > WORLDSIZE)
		{
			float x = 0.0f;
			x = dir.x;
			XMStoreFloat3(&dir, fDir);
			dir.z *= -1;
			if (x != 0.0f)dir.x = x;
		}
		if (dir.x != 0.0f || dir.z != 0.0f)
			fDir = XMLoadFloat3(&dir);

		if (m_Terrain)
		{
			float y = m_Terrain->GetHeight(position.x, position.z);
			if (y >= 0)	m_gameObject->m_d3dxmtxWorld._42 = y;
			else m_gameObject->m_d3dxmtxWorld._42 = 100.0f;

		}
	}

};
