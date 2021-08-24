#pragma once

//KYT '16.02.26
/*
  UIShader, TerrainManager, BillBoardShader를 
  하나의 WorldMatrixShader로 묶을 의향이 있음
*/

#include "MatrixShader.h"

class CBillBoardShader : public CMatrixShader
{

public:

	CBillBoardShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CBillBoardShader(std::unique_ptr<CMesh> pMesh);

	CBillBoardShader(int nObjects = 1);

	virtual ~CBillBoardShader();

	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr) override;

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;

	virtual void CreatePixcelShader(ID3D11Device *pd3dDevice, WCHAR* path);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera) override;

	virtual void RenderNoClear(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	virtual void SetLookAt(XMMATRIX &xmMatrix, XMFLOAT3 d3dxvTarget);

	virtual void Blend(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void Initialize(ID3D11Device *pd3dDevice, std::string path, XMFLOAT3 position, XMFLOAT2 fSize, float frameFPS, int frameCount, int loop = -999) {}

	virtual void StartBillBoard(int loop = 99) {}
};

