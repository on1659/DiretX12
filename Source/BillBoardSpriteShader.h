#pragma once

//KYT '16.08.19
/*
UIShader, TerrainManager, BillBoardShader를
하나의 WorldMatrixShader로 묶을 의향이 있음
*/

#include "BillBoardShader.h"


class CUISpriteMesh;

class CBillBoardSpriteShader : public CBillBoardShader
{

protected:

	struct SpriteAnimation
	{
		float fSpriteAnimation_FPS;
		float fSpriteAnimation_Count;
		float fSpriteAnimation_Width;
		float fSpriteAnimation_Left;
	};

	int							m_nLoop;
	
	float						m_frameFPS;
	int							m_frameCount;
	float						m_frameWidth;
	float						m_frameLeft;

	float						m_imageWidth;

	float						m_fFPSCoount;
	int							m_nCurrentFrame;

private:
	ID3D11Buffer*				m_pd3dcbAnimationSprite;

public:

	CBillBoardSpriteShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CBillBoardSpriteShader(std::unique_ptr<CMesh> pMesh);

	CBillBoardSpriteShader(int nObjects = 1);

	virtual ~CBillBoardSpriteShader();

	void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr) override;

	void CreateShader(ID3D11Device *pd3dDevice) override;

	virtual void Initialize(ID3D11Device *pd3dDevice, std::string path,  XMFLOAT3 position, XMFLOAT2 fSize, float frameFPS, int frameCount,int loop = -999);

	virtual void CreateConstBuffer(ID3D11Device* pd3dDevice);

	virtual void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	virtual void StartBillBoard(int loop);

	void StartSprtieAnimation(int loop = 99);

	void StopSprtieAnimation() { m_nLoop = -1; }

};

