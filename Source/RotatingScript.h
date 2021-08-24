#pragma once

class CRotatingScript : public Component
{
public:
	CRotatingScript(std::string name = "CRotatingScript")
	{
		CRotatingScript::name = name;
		tag = TAG_TYPE::TAG_RotatingScript;
		m_d3dxvRotationAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		m_fRotationSpeed = 15.0f;
	}

	virtual ~CRotatingScript(){}

private:
	XMVECTOR					m_d3dxvRotationAxis;
	float						m_fRotationSpeed;

public:

	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
	}

	virtual void Animate(float fTimeElapsed)
	{
		Component::Rotate(m_gameObject, m_d3dxvRotationAxis, m_fRotationSpeed * fTimeElapsed);
	}

	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	
	void SetRotationAxis(XMVECTOR d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }

};
