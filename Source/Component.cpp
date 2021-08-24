#include "stdafx.h"
#include "Component.h"

Component::Component() : Object()
{
	nGID = gInstanceID++;
}
	 
Component::Component(std::string _name)
{
	name = _name;
	m_gameObject.reset();
}

//Component::Component(std::shared_ptr<CGameObject> object, std::string _name)
//{
//	std::shared_ptr<Component> p = std::make_shared<Component>();
//	p.reset(std::move(this));
//	object->SetCompoent(p);
//	name = _name;
//}

Component::~Component()
{
}

void Component::Start(std::shared_ptr<CGameObject> gameobject)
{
	if(gameobject) m_gameObject = gameobject;
}
void Component::SetContainerNumber(int n)
{
	m_nContainerNum = n;
}
void Component::Animate(Object* object, float fTimeElapsed)
{
}

void Component::Animate(float fTimeElapsed)
{
}

void Component::Animate(std::shared_ptr<CGameObject> object, float fTimeElapsed)
{
}

void Component::CreateConstBuffer(ID3D11Device * pd3dDevice)
{
}

void Component::UpdateConstBuffer(ID3D11DeviceContext * pd3dDeviceContext)
{
}

void Component::Destory()
{
	m_gameObject = nullptr;
}

void Component::OnPlayerUpdate(float fTimeElapsed)
{
}

bool Component::BeforeMove()
{
	return true;
}

void Component::Collision(std::shared_ptr<CGameObject> other, ObjectType type)
{
}

//Set Get
void Component::SetWorldMatrix(std::shared_ptr<CGameObject> gameObject, XMMATRIX& matirx)
{
	gameObject->SetWorldMatrix(matirx);
}

void Component::SetWorldMatrix(std::shared_ptr<CGameObject> gameObject, XMFLOAT4X4& matirx)
{
	gameObject->SetWorldMatrix(matirx);
}

XMMATRIX& Component::GetWorldMatrix(std::shared_ptr<CGameObject> gameObject) const
{
	return gameObject->GetWorldMatrix();
}

XMFLOAT4X4& Component::GetWorldMatrixXMFLOAT(std::shared_ptr<CGameObject> gameObject) const
{
	return gameObject->GetWorldMatrixFLOAT();
}

int Component::GetContainerNumver() const
{
	return m_nContainerNum;
}

void Component::SetObject(std::shared_ptr<CGameObject> pObject)
{
	if (pObject)
		m_gameObject = pObject;
}

//Basic Move & Rota
void Component::SetPosition(std::shared_ptr<CGameObject> gameObject, float x, float y, float z)
{
	gameObject->SetPosition(x, y, z);
}

void Component::SetPosition(std::shared_ptr<CGameObject> gameObject, XMVECTOR& d3dxvPosition)
{
	gameObject->SetPosition(d3dxvPosition);
}

void Component::SetPosition(std::shared_ptr<CGameObject> gameObject, XMFLOAT3& d3dxvPosition)
{
	gameObject->SetPosition(d3dxvPosition.x, d3dxvPosition.y, d3dxvPosition.z);
}

void Component::MoveStrafe(std::shared_ptr<CGameObject> gameObject, float fDistance)
{
	gameObject->MoveStrafe(fDistance);
}

void Component::MoveUp(std::shared_ptr<CGameObject> gameObject, float fDistance)
{
	gameObject->MoveUp(fDistance);
}

void Component::MoveForward(std::shared_ptr<CGameObject> gameObject, float fDistance)
{
	gameObject->MoveForward(fDistance);
}

/* Rotaaton*/
void Component::Rotate(std::shared_ptr<CGameObject> gameObject, float fPitch, float fYaw, float fRoll)
{
	gameObject->Rotate(fPitch, fYaw,fRoll);
}

void Component::Rotate(std::shared_ptr<CGameObject> gameObject, XMVECTOR& pd3dxvAxis, float fAngle)
{
	gameObject->Rotate(pd3dxvAxis, fAngle);
}

XMVECTOR& Component::GetPosition(std::shared_ptr<CGameObject> gameObject, bool bIsLocal)
{
	return gameObject->GetPosition(bIsLocal);
}

XMFLOAT3& Component::GetPositionXMFLOAT3(std::shared_ptr<CGameObject> gameObject, bool bIsLocal)
{
	return gameObject->GetPositionXMFLOAT3(bIsLocal);
}

XMVECTOR& Component::GetRight(std::shared_ptr<CGameObject> gameObject, bool bIsLocal)
{
	return gameObject->GetRight(bIsLocal);
}

XMVECTOR& Component::GetUp(std::shared_ptr<CGameObject> gameObject, bool bIsLocal)
{
	return gameObject->GetUp(bIsLocal);
}

XMVECTOR& Component::GetLook(std::shared_ptr<CGameObject> gameObject, bool bIsLocal)
{
	return gameObject->GetLook(bIsLocal);
}

void Component::Trasnlate(std::shared_ptr<CGameObject> gameObject, float fSpeed, float x, float y, float z)
{
	gameObject->MoveStrafe(x * fSpeed);
	gameObject->MoveUp(y * fSpeed);
	gameObject->MoveForward(z * fSpeed);
}
void Component::Trasnlate(std::shared_ptr<CGameObject> gameObject, float fSpeed, XMVECTOR fDir)
{
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, fDir);
	XMFLOAT3 pos = gameObject->GetPositionXMFLOAT3();
	gameObject->SetPosition(pos.x + dir.x * fSpeed, pos.y + dir.y * fSpeed, pos.z + dir.z * fSpeed);
}
void Component::Trasnlate(std::shared_ptr<CGameObject> gameObject, float fSpeed, XMFLOAT3 fDir)
{
	XMFLOAT3 pos = gameObject->GetPositionXMFLOAT3();
	gameObject->SetPosition(pos.x + fDir.x * fSpeed, pos.y + fDir.y * fSpeed, pos.z + fDir.z * fSpeed);
}








//Set Get
void Component::SetWorldMatrix(XMMATRIX& matirx){m_gameObject->SetWorldMatrix(matirx);}
void Component::SetWorldMatrix(XMFLOAT4X4& matirx){	m_gameObject->SetWorldMatrix(matirx);}
XMMATRIX&   Component::GetWorldMatrix() const { return m_gameObject->GetWorldMatrix(); }
XMFLOAT4X4& Component::GetWorldMatrixXMFLOAT() const { 	return m_gameObject->GetWorldMatrixFLOAT(); }

//Basic Move & Rota
void Component::SetPosition(float x, float y, float z){	m_gameObject->SetPosition(x, y, z);}
void Component::SetPosition(XMVECTOR& d3dxvPosition){m_gameObject->SetPosition(d3dxvPosition);}
void Component::SetPosition(XMFLOAT3& d3dxvPosition){m_gameObject->SetPosition(d3dxvPosition.x, d3dxvPosition.y, d3dxvPosition.z);}
void Component::MoveStrafe(float fDistance){m_gameObject->MoveStrafe(fDistance);}
void Component::MoveUp(float fDistance){m_gameObject->MoveUp(fDistance);}
void Component::MoveForward(float fDistance){m_gameObject->MoveForward(fDistance);}

/* Rotaaton*/
void Component::Rotate(float fPitch, float fYaw, float fRoll){m_gameObject->Rotate(fPitch, fYaw, fRoll);}
void Component::Rotate(XMVECTOR& pd3dxvAxis, float fAngle) { m_gameObject->Rotate(pd3dxvAxis, fAngle); }

XMVECTOR& Component::GetPosition(bool bIsLocal){return m_gameObject->GetPosition(bIsLocal);}
XMFLOAT3& Component::GetPositionXMFLOAT3(bool bIsLocal){return m_gameObject->GetPositionXMFLOAT3(bIsLocal);}
XMVECTOR& Component::GetRight(bool bIsLocal){return m_gameObject->GetRight(bIsLocal); }
XMVECTOR& Component::GetUp(bool bIsLocal){return m_gameObject->GetUp(bIsLocal);}
XMVECTOR& Component::GetLook(bool bIsLocal){ return m_gameObject->GetLook(bIsLocal); }

void Component::Trasnlate(float x, float y, float z)
{
	m_gameObject->MoveStrafe(x);
	m_gameObject->MoveUp(y);
	m_gameObject->MoveForward(z);
}
void Component::Trasnlate(float fSpeed, float x, float y, float z)
{
	m_gameObject->MoveStrafe(x * fSpeed);
	m_gameObject->MoveUp(y * fSpeed);
	m_gameObject->MoveForward(z * fSpeed);
}
void Component::Trasnlate(float fSpeed, XMVECTOR fDir)
{
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, fDir);
	XMFLOAT3 pos = m_gameObject->GetPositionXMFLOAT3();
	m_gameObject->SetPosition(pos.x + dir.x * fSpeed, pos.y + dir.y * fSpeed, pos.z + dir.z * fSpeed);
}
void Component::Trasnlate(float fSpeed, XMFLOAT3 fDir)
{
	XMFLOAT3 pos = m_gameObject->GetPositionXMFLOAT3();
	m_gameObject->SetPosition(pos.x + fDir.x * fSpeed, pos.y + fDir.y * fSpeed, pos.z + fDir.z * fSpeed);
}
