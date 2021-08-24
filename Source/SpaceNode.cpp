

#include "stdafx.h"
#include "SpaceNode.h"
#include "Shader.h"
#include <algorithm>

#ifdef _QURD_TREE

//--------------------------------------SoaceBide

SpaceNode::SpaceNode()
{

#ifdef _Bounding
	boundingbox.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
#else
	fCenter = { 0.0f, 0.0f, 0.0f };
#endif

	//Default
	fSize = { 64.0f, 64.0f, 64.0f };

	m_vectorObject.clear();

	m_aabb = new AABB();
	m_aabb->m_d3dxvMinimum = fCenter;
	m_aabb->m_d3dxvMinimum = fSize;

}

SpaceNode::SpaceNode(XMFLOAT3 fInputSize)
{
	for (int i = 0; i < NODE_TYPE_NUMBER_END; i++)
		next[i] = new SpaceNode();

#ifdef _Bounding
	boundingbox.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
#else
	fCenter = { 0.0f, 0.0f, 0.0f };

	//boundingbox = new AABB();
#endif

	//Default
	if (fInputSize.x == -1 && fInputSize.y == -1 && fInputSize.z == -1)
		fSize = { 64.0f, 64.0f, 64.0f };

	else
		fSize = fInputSize;


	m_aabb = new AABB();
	m_aabb->m_d3dxvMinimum = fCenter;
	m_aabb->m_d3dxvMinimum = fSize;
}

SpaceNode::SpaceNode(XMFLOAT3 _fCenter, XMFLOAT3 fInputSize)
{
	for (int i = 0; i < NODE_TYPE_NUMBER_END; i++)
		next[i] = nullptr;// new SpaceNode();

#ifdef _Bounding
	boundingbox.Center = _fCenter;
#else
	fCenter = _fCenter;
	m_aabb = new AABB();
	//boundingbox = new AABB();
#endif

	//Default
	if (fInputSize.x == -1 && fInputSize.y == -1 && fInputSize.z == -1)
		fSize = { 64.0f, 64.0f, 64.0f };

	else
		fSize = fInputSize;

	nLevel = 0;

}

SpaceNode::~SpaceNode()
{
	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
		if (next[i])delete next[i];
	//delete object;
}

void SpaceNode::AddObject(std::shared_ptr<CGameObject> object)
{
	m_vectorObject.push_back(object);
}

void SpaceNode::AddObject(std::shared_ptr<CGameObject> object, std::string tag)
{
	m_vectorObject.push_back(object);
	//SetTag(tag);
}

bool SpaceNode::IsInner(std::shared_ptr<CGameObject> object)
{

	if (find(m_vectorObject.begin(), m_vectorObject.end(), object) == m_vectorObject.end())
		return false;
	else
		return true;


	for (auto p : m_vectorObject)
	{
		if (p == object)
			return true;
	}

	return false;
}

void SpaceNode::SetSize(XMVECTOR fInputSize)
{

#ifdef _Bounding
	XMFLOAT3 fSize;
	XMStoreFloat3(&fSize, fInputSize);
	boundingbox.Extern = fSize;
#else
	//XMVECTOR xmCenter = XMLoadFloat3(&fCenter);
	//XMVECTOR min = (xmCenter - (fInputSize * 0.5));
	//XMVECTOR max = (xmCenter + (fInputSize * 0.5));
	//XMStoreFloat3(&boundingbox->m_d3dxvMinimum, min);
	//XMStoreFloat3(&boundingbox->m_d3dxvMaximum, max);

	XMStoreFloat3(&fSize, fInputSize);


#endif
}

void SpaceNode::SetPosition(XMVECTOR fInputCenter)
{

	#ifdef _Bounding
		XMFLOAT3 fCenter;
		XMStoreFloat3(&fCenter, fInputCenter);
		boundingbox.Center = fCenter;
	#else
		XMStoreFloat3(&fCenter, fInputCenter);
		XMStoreFloat3(&m_Position, fInputCenter);

		m_Position.y = 0.0f;
		fCenter.y = 0.0f;
	#endif

}

void SpaceNode::AABBUpdate()
{
	m_aabb->m_d3dxvMinimum = XMFLOAT3(fCenter.x - fSize.x * 0.5f, 0, fCenter.z - fSize.z * 0.5f);
	m_aabb->m_d3dxvMaximum = XMFLOAT3(fCenter.x + fSize.x * 0.5f, fSize.y * NODE_DEPTH_LEVEL, fCenter.z + fSize.z * 0.5f);
}


void SpaceNode::SetPosition(XMFLOAT3 fInputCenter)
{
#ifdef _Bounding
	boundingbox.Center = fInputCenter;
#else
	fCenter = fInputCenter;
#endif

}

AABB* SpaceNode::GetAABB()
{
	return m_aabb;
}

BoundingBox SpaceNode::GetBoundingBox()
{
	return BoundingBox(fCenter, fSize);
}

//--------------------------------------DeleteObject

void SpaceNode::DeleteObject(std::shared_ptr<CGameObject> object)
{
	auto findobject = find(m_vectorObject.begin(), m_vectorObject.end(), object);
	m_vectorObject.erase(findobject, m_vectorObject.end());
}

//Find Node
SpaceNode* CSpacePartition::FindSpaceNode(XMFLOAT3 position, SpaceNode* parent, int nTreeLevel)
{

	if (nTreeLevel == m_nMaxTreeLevel)
	{
		return parent;
	}
	else
	{
		int nextSpaceNodeIndex = 0;



#ifdef _Bounding
		XMFLOAT3 d3dPos = parent->boundingbox.Center;
#else
		XMFLOAT3 d3dPos = parent->fCenter;
#endif

		if (d3dPos.z > position.z)
			nextSpaceNodeIndex += 2;

		if (d3dPos.x < position.x)
			nextSpaceNodeIndex += 1;

		//SpaceNode octData;
		//octData = FindNodeRange(position, parent->next[nextSpaceNodeIndex], nTreeLevel + 1);
		//return octData;
		return FindSpaceNode(position, parent->next[nextSpaceNodeIndex], nTreeLevel + 1);
	}


}

SpaceNode* CSpacePartition::FindSpaceNode(XMFLOAT3 position)
{
	return FindSpaceNode(position, m_head, 1);
}



#endif