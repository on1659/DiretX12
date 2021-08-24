#pragma once

#ifdef _QURD_ARRARY

#include "SpaceNodeQuad.h"

class CSpacePartitionQuad : public CSingleTonBase<CSpacePartitionQuad>
{
private:
	bool								m_bLoadingInit;

public:
	CSpacePartitionQuad();

	CSpacePartitionQuad(float width, float look, int nMaxDepth);

	~CSpacePartitionQuad();

	void ReleseInstance();

	void Load(float width, float look, int nMaxDepth);
	void FinalLoad();

	void AddObject(std::shared_ptr<CGameObject> object);
	void AddObject(std::shared_ptr<WorldMatrix> matrix);
	void AddStaticObject(std::shared_ptr<CGameObject> object);
	void AddBlendObject(std::shared_ptr<CGameObject> object);
	void AddChlidObject(std::shared_ptr<CGameObject> object);
	void AddStatictTerrainObject(std::shared_ptr<CGameObject> object);

	void DeleteObject(std::shared_ptr<CGameObject> object);
	void DeleteObject(std::shared_ptr<WorldMatrix> matrix);
	void DeleteBlendObject(std::shared_ptr<CGameObject> object);
	void DeletChlidObject(std::shared_ptr<CGameObject> object);
	void DeleteStaticTerrainObject(std::shared_ptr<CGameObject> object);

	bool IsSameNode(std::shared_ptr<CGameObject> object);
	bool IsSameNode(std::shared_ptr<WorldMatrix> object);

	void Update(std::shared_ptr<CGameObject> object);
	void UpdateStaticObject(std::shared_ptr<CGameObject> object);
	void Update(std::shared_ptr<WorldMatrix> object);
	void UpdateBlend(std::shared_ptr<CGameObject> object);
	void UpdateChlid(std::shared_ptr<CGameObject> object);

	void DynamicDestory(std::shared_ptr<CGameObject> object);
	void StaticDestory(std::shared_ptr<CGameObject> object);
	void ChlidDestory(std::shared_ptr<CGameObject> object);
	void BlendObjectDestory(std::shared_ptr<CGameObject> object);
	void WorldMatrixDestory(std::shared_ptr<WorldMatrix> matirx);

	CSpaceNodeQuad*					FindSpaceNode(XMFLOAT3 position);
	std::shared_ptr<CSpaceNodeQuad> FindSpaceNode(XMVECTOR position);
	int								 FindSpaceNum(XMVECTOR position);

	std::vector
		<std::shared_ptr
		<CSpaceNodeQuad >> GetRoomContainer() { return m_vRoom; }

private:
	XMFLOAT3  m_xmWorldSize;

	XMFLOAT3  m_xmSpaceSize;

	int		  m_nMaxDepth;

	int		  m_nDepth;

	std::vector
		<std::shared_ptr
		<CSpaceNodeQuad>>		m_vRoom;

private:

	std::vector
		<std::shared_ptr
		<CGameObject>>			m_vDynamicGameObject;

public:
	std::vector<std::shared_ptr<CGameObject>> GetDynamicObjectVector() { return m_vDynamicGameObject; }
};


#endif