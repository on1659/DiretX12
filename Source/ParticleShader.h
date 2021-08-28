#pragma once

//KYT '16.03.05 plus 
/*
 CParticleShader ����
 VS,PS���� �̿��� Shader��
*/
// ex) Player

#include "InstancedObjectsShader.h"

//----------------------------------------------------------------
class CParticleShader : public CInstancedObjectsShader
{

public:

	CParticleShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CParticleShader(std::unique_ptr<CMesh> pMesh);

	CParticleShader(int nObjects = 1);

	virtual ~CParticleShader();

	virtual void CreateShader(ID3D11Device* pd3dDevice);

};