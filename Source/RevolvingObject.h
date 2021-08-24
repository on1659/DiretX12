#pragma once

class CRevolvingObject : public Component
{
public:
	
	CRevolvingObject() : Component()
	{
		name = "CRevolvingObject";
	}

	virtual ~CRevolvingObject()
	{

	}
private:
	XMVECTOR					m_d3dxvRevolutionAxis;
	float						m_fRevolutionSpeed;

public:
	
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
		m_d3dxvRevolutionAxis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		m_fRevolutionSpeed = 0.0f;
	}

	virtual void Animate(float fTimeElapsed)
	{
		XMMATRIX mtxRotate;
		mtxRotate = XMMatrixRotationAxis(m_d3dxvRevolutionAxis, XMConvertToRadians(m_fRevolutionSpeed * fTimeElapsed));
		SetWorldMatrix(m_gameObject, GetWorldMatrix(m_gameObject) * mtxRotate);
	}

	void SetRevolutionSpeed(float fRevolutionSpeed) { m_fRevolutionSpeed = fRevolutionSpeed; }

	void SetRevolutionAxis(XMVECTOR d3dxvRevolutionAxis) { m_d3dxvRevolutionAxis = d3dxvRevolutionAxis; }

};

