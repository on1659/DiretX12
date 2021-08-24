#pragma once

//KYT '16.02.26
/*
	Shader ���ο� Object�� Pos�� �������°� �ƴ϶�
	Matrix�� ������ ��ġ���� ������
*/

#include "InstancedObjectsShader.h"

class CGameObject;

class CBulletShader : public CInstancedObjectsShader
{
public:
	std::list<std::shared_ptr<CGameObject>>	m_lGameObject;

protected:
	bool									m_bConstBufferUpdate;

public:

	CBulletShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CBulletShader(std::unique_ptr<CMesh> pMesh);

	CBulletShader(int nObjects = 1);

	virtual ~CBulletShader();

	virtual void ReleaseObjects();

	std::vector<std::shared_ptr<WorldMatrix>> GetWorldMatrix() { return m_vMtxWorld; }

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera = nullptr);

	void AddBullet(std::shared_ptr<CGameObject> gameObject);

	void DestoryBullet(std::shared_ptr<CGameObject> gameObject);

	void AnimateBullet(float fTimeElapsed);

};

