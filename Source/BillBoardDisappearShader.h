#pragma once

//KYT '16.02.26
/*
UIShader, TerrainManager, BillBoardShader를
하나의 WorldMatrixShader로 묶을 의향이 있음
*/

#include "BillBoardShader.h"

class  CBillBoardDisappearShader : public CBillBoardShader
{
private:
	struct cbBillBoaredAlpha
	{
		float padding1;
		float alpha;
		float padding2;
		float padding3;
	};
	
	ID3D11Buffer*							m_pd3dcbBillBoardAlpha;
	float									m_fAlpha;
	float									m_fSpeed;

	XMFLOAT4X4								m_mtxUpdateMatrix;

	std::list<std::shared_ptr<WorldMatrix>>	m_lMtxWorld;

public:
	

	CBillBoardDisappearShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CBillBoardDisappearShader(std::unique_ptr<CMesh> pMesh);

	CBillBoardDisappearShader(int nObjects = 1);

	virtual ~CBillBoardDisappearShader();

	void Start() { m_fAlpha = 1.0f; }

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	
	virtual void CreateConstBuffer(ID3D11Device* pd3dDevice);

	virtual void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	
	virtual void SetPosition(float x, float y, float z);

	virtual void SetLookAt(XMMATRIX &xmMatrix, XMFLOAT3 d3dxvTarget, XMFLOAT3 look);

};

