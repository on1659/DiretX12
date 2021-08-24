#include "stdafx.h"
#include "GeoMetryInstanceShader.h"

//----------------------------------------------------------------

CGeoMetryInstanceShader::CGeoMetryInstanceShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(move(pMesh), nObjects)
{

}

CGeoMetryInstanceShader::CGeoMetryInstanceShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{

}

CGeoMetryInstanceShader::CGeoMetryInstanceShader(int nObjects) : CInstancedObjectsShader(nObjects)
{

}

CGeoMetryInstanceShader::~CGeoMetryInstanceShader()
{

}

void CGeoMetryInstanceShader::CreateShader(ID3D11Device *pd3dDevice)
{

}
