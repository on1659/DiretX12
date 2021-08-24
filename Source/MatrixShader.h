#pragma once

//KYT '16.02.26
/*
	Shader ���ο� Object�� Pos�� �������°� �ƴ϶�
	Matrix�� ������ ��ġ���� ������
	
*/

#include "InstancedObjectsShader.h"

class CMatrixShader : public CInstancedObjectsShader
{

protected:
	std::vector<std::shared_ptr<WorldMatrix>>	m_vMtxWorld;

	std::vector<std::shared_ptr<WorldMatrix>>	m_vRendermtxWorld;

	bool				m_bInit;

public:

	CMatrixShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CMatrixShader(std::unique_ptr<CMesh> pMesh);

	CMatrixShader(int nObjects = 1);

	virtual ~CMatrixShader();

	virtual void ReleaseObjects();

	std::vector<std::shared_ptr<WorldMatrix>> GetWorldMatrix(){ return m_vMtxWorld; }

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void SetPosition(float x, float y, float z);

	virtual void SetPosition(XMFLOAT3 position);

	virtual void AddMatrixObject(std::shared_ptr<WorldMatrix> matrix){ m_vRendermtxWorld.push_back(matrix); }



};

