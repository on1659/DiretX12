#pragma once
#include "CharacterObject.h"
#include "CharacterNormalShader.h"
#include "RifleGunWeapon.h"

class CTerroristCharacterObject : public CCharacterObject
{
public:
	CTerroristCharacterObject();
	virtual ~CTerroristCharacterObject();

	virtual void CreateMaterial();
	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateAnimation();
	virtual void CreateWeapon(ID3D11Device *pd3dDevice);
};

