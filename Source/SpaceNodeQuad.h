#pragma once

#ifdef _QURD_ARRARY

//KYT '12.02.07
/*Quad Tree를 배열로 만든다.*/

#include <iostream>
#include <list>
#include <vector>
#include "Mesh.h"
#include "MyStruct.h"
//#define _Bounding


class CGameObject;
class CCamera;
class CShader;

//X  = x
//Y  = z

class CSpaceNodeQuad : public Object
{
public:
	int nGID;

private:
	std::list<std::shared_ptr<CGameObject>>			m_vDynamicObject;
	std::list<std::shared_ptr<CGameObject>>			m_vStaticObject;
	std::list<std::shared_ptr<WorldMatrix>>			m_vMatirx;
	std::list<std::shared_ptr<CGameObject>>			m_vStaticTerrainObject;
	std::list<std::shared_ptr<CGameObject>>			m_vBlendObject;

	std::list<std::shared_ptr<CGameObject>>			m_vChlidObject;

	
	BoundingBox										boundingbox;
	AABB											aabb;
	XMFLOAT3										fCenter;
	XMFLOAT3										fSize;
	bool											bCulling;
	int												nRoomContainerCount;

public: 
	CSpaceNodeQuad();

	CSpaceNodeQuad(XMFLOAT3 center, XMFLOAT3 size);

	CSpaceNodeQuad(XMFLOAT3 center, XMFLOAT3 size, int nRoomCount);

	~CSpaceNodeQuad();

	int GetRoomContainerCount() const { return nRoomContainerCount; }

	void ReleseInstance();

	//Add
	void       AddObject(std::shared_ptr<CGameObject> object);
	void       AddObject(std::shared_ptr<WorldMatrix> object);
	void   AddBlendbject(std::shared_ptr<CGameObject> object);
	void AddStaticObject(std::shared_ptr<CGameObject> object);
	void  AddChlidObject(std::shared_ptr<CGameObject> object);
	void AddStaticTerrainObject(std::shared_ptr<CGameObject> object);

	//Delete
	void DeleteObject(std::shared_ptr<CGameObject> object);
	void DeleteStaticObject(std::shared_ptr<CGameObject> object);
	void DeleteObject(std::shared_ptr<WorldMatrix> object);
	void DeleteBlendObject(std::shared_ptr<CGameObject> object);
	void DeleteChlidObject(std::shared_ptr<CGameObject> object);
	void DeleteStaticTerrainObject(std::shared_ptr<CGameObject> object);

	//IsInner
	bool IsInner(std::shared_ptr<CGameObject> object);
	bool IsInner(std::shared_ptr<WorldMatrix> object);
	bool IsBlendObjectInner(std::shared_ptr<CGameObject> object);
	bool IsChildObjectInner(std::shared_ptr<CGameObject> object);
	bool IsStaticTerrainInner(std::shared_ptr<CGameObject> object);

	void DynamicDestory(std::shared_ptr<CGameObject> object);
	void StaticDestory(std::shared_ptr<CGameObject> object);
	void WorldMatrixDestory(std::shared_ptr<WorldMatrix> matrix);
	void StaticTerrainDestory(std::shared_ptr<CGameObject> object);
	void ChlidObjectDestory(std::shared_ptr<CGameObject> object);
	void BlendObjectDestory(std::shared_ptr<CGameObject> matrix);

	std::shared_ptr<CSpaceNodeQuad> FindSpaceNode(XMFLOAT3 position);


	//Get & Set
	void SetCenter(XMFLOAT3 center);

	void SetSize(XMFLOAT3 size);
	
	void Set(XMFLOAT3 center, XMFLOAT3 size);

	BoundingBox GetBoundingBox(){ return boundingbox; }

	AABB* GetAABB(){ return &aabb; }

	XMFLOAT3 GetCenter(){ return fCenter; }
	
	std::list<std::shared_ptr<CGameObject>> GetDynamicObjectsContainer()  { return m_vDynamicObject; }
	std::list<std::shared_ptr<CGameObject>> GetStaticObjectsContainer()   { return m_vStaticObject; }
	std::list<std::shared_ptr<WorldMatrix>> GetMatrixContainer()		  { return m_vMatirx; }
	std::list<std::shared_ptr<CGameObject>> GetStaticTerrainContainer()   { return m_vStaticTerrainObject; }
	std::list<std::shared_ptr<CGameObject>> GetBlendObjectContainer()	  { return m_vBlendObject; }
	std::list<std::shared_ptr<CGameObject>> GetChlidObjectContainer()	  { return m_vChlidObject; }
};


#endif