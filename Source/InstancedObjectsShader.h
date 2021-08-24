#pragma once

#include "Shader.h"

//KYT '16.01.20 plus
/*수정 사항*/
/*
  1. Mesh는 InstanceShader 하나당 한개의 Mesh로 일대일 대응이다.
  2. 생성자에서 Mesh를 생성할 수 있다.
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
	/*삭제 해야함. 왜냐하면 생성자에서 받기 때문에*/
	void SetMesh(std::unique_ptr<CMesh> pMesh);
	
	
	//KYT '16.01.20 plus
	/*가상함수로 모든 쉐이더마다 써줄지, 경배처럼 Type으로 하나의 Create에서 해줄지 결정*/
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateBlendShader(ID3D11Device *pd3dDevice);

	//KYT '16.01.18 minus
	/*앞으로 사용하지 않을꺼임 Cuz. 쉐이더 검색은 수동으로 할꺼임 */
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