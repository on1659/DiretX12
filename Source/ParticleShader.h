#pragma once

//KYT '16.03.05 plus 
/*
 CParticleShader 제작
 VS,PS만을 이용한 Shader임
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

	virtual void CreateShader(ID3D11Device *pd3dDevice);

};