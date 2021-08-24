#include "stdafx.h"
#include "ParticleShader.h"
//----------------------------------------------------------------

CParticleShader::CParticleShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(move(pMesh), nObjects)
{

}

CParticleShader::CParticleShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{

}

CParticleShader::CParticleShader(int nObjects) : CInstancedObjectsShader(nObjects)
{

}

CParticleShader::~CParticleShader()
{

}

void CParticleShader::CreateShader(ID3D11Device *pd3dDevice)
{
}

