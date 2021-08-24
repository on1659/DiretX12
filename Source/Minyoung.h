#pragma once

#include "stdafx.h"

class CMinyoung : public Component
{
public:
	XMFLOAT4X4					m_d3dxmtxTexture;
	std::shared_ptr<CGameObject>	 m_pTargetObject;
public:

	CMinyoung(std::string _name = "")
	{
		name = "CMinyoung";
		m_pTargetObject.reset();
	}

	virtual ~CMinyoung(){}

public:

	virtual void Start(std::shared_ptr<CGameObject> m_gameObject)
	{
		Component::Start(m_gameObject);
		XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());
	}

	void SetOTargetbject(std::shared_ptr<CGameObject> object)
	{
		if (m_pTargetObject) m_pTargetObject.reset();
		m_pTargetObject = object;
	}

	virtual void Animate(float fTimeElapsed)
	{
		XMVECTOR vPosition = m_gameObject->GetPosition();
		XMVECTOR vTarget = m_pTargetObject->GetPosition();
		XMVECTOR vToTarget;
		vToTarget = vTarget - vPosition;

		XMFLOAT3 xmFloat3;
		XMStoreFloat3(&xmFloat3, XMVector3Length(vToTarget));
		if (xmFloat3.x < 5.0f)
		{
			XMVECTOR vLook = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
			vLook = XMVector3TransformNormal(vLook, m_gameObject->GetWorldMatrix());
			vToTarget = XMVector3Normalize(vToTarget);
			XMVECTOR vCrossProduct;
			vCrossProduct = XMVector3Cross(vLook, vToTarget);
			float fDotProduct;
			XMStoreFloat3(&xmFloat3, XMVector3Dot(vLook, vToTarget));
			fDotProduct = xmFloat3.x;

			float fAngle = (fDotProduct > 0.0f) ? D3DXToDegree(acos(fDotProduct)) : 90.0f;
			XMStoreFloat3(&xmFloat3, vCrossProduct);
			fAngle *= (xmFloat3.y > 0.0f) ? 1.0f : -1.0f;
			XMMATRIX mtxRotation;
			mtxRotation = XMMatrixRotationY(fAngle);
			m_gameObject->SetWorldMatrix(mtxRotation * m_gameObject->GetWorldMatrix());
			float fDeltaTime = 1 / 60.0f;

			XMFLOAT4X4 xmWorld;
			XMStoreFloat4x4(&xmWorld, m_gameObject->GetWorldMatrix());

			xmWorld._41 += m_gameObject->GetWorldMatrixFLOAT()._31 * (3.0f * fDeltaTime);
			xmWorld._42 += m_gameObject->GetWorldMatrixFLOAT()._32 * (3.0f * fDeltaTime);
			xmWorld._43 += m_gameObject->GetWorldMatrixFLOAT()._33 * (3.0f * fDeltaTime);

			m_gameObject->SetWorldMatrix(xmWorld);

		}

	}
};
