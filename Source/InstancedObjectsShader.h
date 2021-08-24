#pragma once

#include "Shader.h"

//KYT '16.01.20 plus
/*���� ����*/
/*
  1. Mesh�� InstanceShader �ϳ��� �Ѱ��� Mesh�� �ϴ��� �����̴�.
  2. �����ڿ��� Mesh�� ������ �� �ִ�.
*/
class CInstancedObjectsShader : public CObjectsShader
{

public:

	std::vector<std::shared_ptr<WorldMatrix>> m_vMtxWorld;

	CInstancedObjectsShader(std::unique_ptr<CMesh> pMesh, int nObjects);
	
	CInstancedObjectsShader(std::unique_ptr<CMesh> pMesh);
	
	CInstancedObjectsShader(int nObjects = 1);
	
	virtual ~CInstancedObjectsShader();


	//KYT '16.01.20 minus
	/*���� �ؾ���. �ֳ��ϸ� �����ڿ��� �ޱ� ������*/
	void SetMesh(std::unique_ptr<CMesh> pMesh);
	
	
	//KYT '16.01.20 plus
	/*�����Լ��� ��� ���̴����� ������, ���ó�� Type���� �ϳ��� Create���� ������ ����*/
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateBlendShader(ID3D11Device *pd3dDevice);

	//KYT '16.01.18 minus
	/*������ ������� �������� Cuz. ���̴� �˻��� �������� �Ҳ��� */
	//virtual void CreateShader(ID3D11Device *pd3dDevice, UINT nType);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	virtual void RenderNoClear(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	virtual void RenderPlayer(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	virtual void RenderTerrain(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	void ClearObject() { m_vRenderObject.clear(); }
	void ReSetShader();

protected:

	UINT							m_nInstanceBufferStride;
	UINT							m_nInstanceBufferOffset;

	ID3D11Buffer					*m_pd3dInstanceBuffer;


};