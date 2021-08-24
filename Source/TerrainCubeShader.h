#pragma once

//KYT '16.02.10
/*
  Terrain�� �������� Object�� ��������
*/

#include "InstancedObjectsShader.h"

//----------------------------------------------------------------
class CTerrainCubeShader : public CInstancedObjectsShader
{

public:

	CTerrainCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CTerrainCubeShader(std::unique_ptr<CMesh> pMesh);

	CTerrainCubeShader(int nObjects = 1);

	virtual ~CTerrainCubeShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

};
