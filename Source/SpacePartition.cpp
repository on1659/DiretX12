

#include "stdafx.h"
#include "SpacePartition.h"

#ifdef _QURD_TREE

/*--------------------------------------OctTree -------------------------------------*/

//BulidObejcts
void CSpacePartition::Load(float worldSize, float nMaxTreeLevel)
{
	m_xmvWorldSize = XMVectorSet(worldSize, worldSize, worldSize, 1.0f);

	m_nMaxTreeLevel = nMaxTreeLevel;

	m_xmvSpaceSize = m_xmvWorldSize;

	m_xmvDistance = m_xmvSpaceSize;


#ifdef __DEBUG__
	level = new int[NODE_TYPE_NUMBER_END];
	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
		level[i] = 0;
#endif

	if (m_head)m_head = nullptr;
	m_head = new SpaceNode();
	m_head->nLevel = 1;

	//----------------------------------------------------------------------------------------------
	m_head->SetPosition(m_xmvWorldSize / pow(2.0f, 1.0f));
	//----------------------------------------------------------------------------------------------

	m_head->SetSize(m_xmvSpaceSize);
	m_nNodeNum = 1;

	CSpacePartition::AddChilde(m_head, 1);

}

void CSpacePartition::Load(float worldWidth, float worldHeight, float Z, float nMaxTreeLevel)
{
	m_xmvWorldSize = XMVectorSet(worldWidth, worldHeight, Z, 1.0f);

	m_nMaxTreeLevel = nMaxTreeLevel;

	m_xmvSpaceSize = m_xmvWorldSize;

	m_xmvDistance = m_xmvSpaceSize;


#ifdef __DEBUG__
	level = new int[NODE_TYPE_NUMBER_END];
	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
		level[i] = 0;
#endif

	if (m_head)m_head = nullptr;
	m_head = new SpaceNode();
	m_head->nLevel = 1;
	
	//----------------------------------------------------------------------------------------------
	m_head->SetPosition(m_xmvWorldSize / pow(2.0f, 1.0f));
	//----------------------------------------------------------------------------------------------
	
	m_head->SetSize(m_xmvSpaceSize);
	m_nNodeNum = 1;

	this->AddChilde(m_head, 1);

}

//Add
void CSpacePartition::AddChilde(SpaceNode *parent, int nTreeLevel)
{
	if (nTreeLevel >= m_nMaxTreeLevel)return;

	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
	{
		if (i == 0)
			CSpacePartition::Insert(parent, i, nTreeLevel + 1);

		CSpacePartition::AddChilde(parent->next[i], nTreeLevel + 1);
	}

}

void CSpacePartition::Insert(SpaceNode *parent, int nodeType, int nTreeLevel)
{
	float TOP = 1.0f;
	float BOT = -1.0f;

	float LEFT = -1.0f;
	float RIGHT = 1.0f;

	float FRONT = 1.0f;
	float BACK = -1.0f;

	float fPow = pow(2.0f, nTreeLevel);



	FXMVECTOR xmvDistance =  m_xmvDistance / pow(2, nTreeLevel);
	XMVECTOR xmvCenter = xmvDistance;
	
	#ifdef _Bounding
		XMVECTOR xmvParentCenter = XMLoadFloat3(&(parent->boundingbox.Center));
	#else
		XMVECTOR xmvParentCenter = XMLoadFloat3(&(parent->fCenter));
	#endif
	
	XMVECTOR xmvSize = XMLoadFloat3(&(parent->fSize)) * 0.5f;
	XMFLOAT3 xmfSize;
	XMStoreFloat3(&xmfSize, xmvSize);

	int cnt = 0;
	XMVECTOR d3dNodePosition;

	SpaceNode* tmpNode[NODE_TYPE_NUMBER_END];
	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
	{
		tmpNode[i] = new SpaceNode();
	}
			//NODE_TYPE_TOP_LEFT_FRONT
			{
				XMVECTOR childPos = xmvCenter * XMVectorSet(LEFT, TOP, FRONT, 1.0f);
				d3dNodePosition = xmvParentCenter + childPos;
				tmpNode[NODE_TYPE_TOP_LEFT_FRONT]->nLevel = nTreeLevel;
				tmpNode[NODE_TYPE_TOP_LEFT_FRONT]->SetPosition(d3dNodePosition);
				tmpNode[NODE_TYPE_TOP_LEFT_FRONT]->SetSize(xmfSize);
				tmpNode[NODE_TYPE_TOP_LEFT_FRONT]->AABBUpdate();
				parent->next[NODE_TYPE_TOP_LEFT_FRONT] = tmpNode[NODE_TYPE_TOP_LEFT_FRONT];
				m_nNodeNum++;
			}
			//NODE_TYPE_TOP_RIGHT_FRONT
			{
				XMVECTOR childPos = xmvCenter * XMVectorSet(RIGHT, TOP, FRONT, 1.0f);
				d3dNodePosition = (xmvParentCenter + childPos);
				tmpNode[NODE_TYPE_TOP_RIGHT_FRONT]->nLevel = nTreeLevel;
				tmpNode[NODE_TYPE_TOP_RIGHT_FRONT]->SetPosition(d3dNodePosition);
				tmpNode[NODE_TYPE_TOP_RIGHT_FRONT]->SetSize(xmfSize);
				tmpNode[NODE_TYPE_TOP_RIGHT_FRONT]->AABBUpdate();
				parent->next[NODE_TYPE_TOP_RIGHT_FRONT] = tmpNode[NODE_TYPE_TOP_RIGHT_FRONT];
				m_nNodeNum++;
			}
			//NODE_TYPE_TOP_LEFT_BACK
			{
				XMVECTOR childPos = xmvCenter * XMVectorSet(LEFT, TOP, BACK, 1.0f);
				d3dNodePosition = (xmvParentCenter + childPos);
				tmpNode[NODE_TYPE_TOP_LEFT_BACK]->nLevel = nTreeLevel;
				tmpNode[NODE_TYPE_TOP_LEFT_BACK]->SetPosition(d3dNodePosition);
				tmpNode[NODE_TYPE_TOP_LEFT_BACK]->SetSize(xmfSize);
				tmpNode[NODE_TYPE_TOP_LEFT_BACK]->AABBUpdate();
				parent->next[NODE_TYPE_TOP_LEFT_BACK] = tmpNode[NODE_TYPE_TOP_LEFT_BACK];
				m_nNodeNum++;
			}

			//NODE_TYPE_TOP_RIGHT_BACK
			{
				XMVECTOR childPos = xmvCenter * XMVectorSet(RIGHT, TOP, BACK, 1.0f);
				d3dNodePosition = (xmvParentCenter + childPos);
				tmpNode[NODE_TYPE_TOP_RIGHT_BACK]->nLevel = nTreeLevel;
				tmpNode[NODE_TYPE_TOP_RIGHT_BACK]->SetPosition(d3dNodePosition);
				tmpNode[NODE_TYPE_TOP_RIGHT_BACK]->SetSize(xmfSize);
				tmpNode[NODE_TYPE_TOP_RIGHT_BACK]->AABBUpdate();
				parent->next[NODE_TYPE_TOP_RIGHT_BACK] = tmpNode[NODE_TYPE_TOP_RIGHT_BACK];
				m_nNodeNum++;
			}

	
	/* 맨 자식노드 레벨에서 리스트에 추가해줌 */
	if (nTreeLevel == m_nMaxTreeLevel)
	{
		for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
			m_vLastRoom.push_back(parent->next[i]);
	}
	//*tmpNode = nullptr;
}

//GetNode XMFLOAT3
void CSpacePartition::GetNodePosition(std::vector<XMFLOAT3> *vector, SpaceNode *parent, int nTreeLevel)
{
	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
	{
		#ifdef _Bounding
			vector->push_back(parent->next[i]->boundingbox.Center);
		#else
			vector->push_back(parent->next[i]->fCenter);
		#endif
	}
}

void CSpacePartition::GetNode(std::vector<XMFLOAT3> *vector, SpaceNode* parent, int nTreeLevel)
{
	if (nTreeLevel >= m_nMaxTreeLevel)return;

	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
	{
		if (i == 0)
			GetNodePosition(vector, parent, nTreeLevel + 1);

		GetNode(vector, parent->next[i], nTreeLevel + 1);
	}
}

std::vector<XMFLOAT3> CSpacePartition::GetNode()
{
	std::vector<XMFLOAT3> vector;
	#ifdef _Bounding
		vector.push_back(m_head->boundingbox.Center);
	#else	
		vector.push_back(m_head->fCenter);
	#endif

	GetNode(&vector, m_head, 1);
	return vector;
}

//GetData octOutData
void CSpacePartition::GetNode(std::vector<octOutData> *vector, SpaceNode* parent, int nTreeLevel)
{
	if (nTreeLevel >= m_nMaxTreeLevel)return;

	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
	{
		if (i == 0)
			GetNodePosition(vector, parent, nTreeLevel + 1);

		GetNode(vector, parent->next[i], nTreeLevel + 1);
	}
}

void CSpacePartition::GetNodePosition(std::vector<octOutData> *vector, SpaceNode *parent, int nTreeLevel)
{
	for (int i = 0; i < NODE_TYPE_NUMBER_END; ++i)
	{
		//XMFLOAT4 a = { parent->fCenter.x, parent->fCenter.y, parent->fCenter.z, (float)nTreeLevel };
		octOutData a;
		#ifdef _Bounding
			a.Set(parent->next[i]->boundingbox.Center, parent->next[i]->fSize, (float)nTreeLevel);
		#else
			a.Set(parent->next[i]->fCenter, parent->next[i]->fSize, (float)nTreeLevel);
		#endif
		vector->push_back(a);
	}
}

std::vector<octOutData> CSpacePartition::GetNodeOct()
{
	std::vector<octOutData> vector;
	octOutData oct;
	#ifdef _Bounding
		oct.Set(m_head->boundingbox.Center, m_head->fSize, 1);
	#else
		oct.Set(m_head->fCenter, m_head->fSize, 1);
	#endif
	vector.push_back(oct);
	GetNode(&vector, m_head, 1);
	return vector;
}


//AddObject
bool CSpacePartition::AddObject(std::shared_ptr<CGameObject> object, SpaceNode* parent, int nTreeLevel)
{
	if (nTreeLevel == m_nMaxTreeLevel)
	{
		parent->AddObject(object);
		object->SetSpaceNode(parent);
		//parent->m_vectorObject.push_back(object);
		return true;
	}
	else
	{
		int nextSpaceNodeIndex = 0;
		XMFLOAT3 position = object->GetPositionXMFLOAT3();

	#ifdef _Bounding
		XMFLOAT3 d3dPos = parent->boundingbox.Center;
	#else
			XMFLOAT3 d3dPos = parent->fCenter;
	#endif

		if (d3dPos.z > position.z)
			nextSpaceNodeIndex += 2;
	
		if (d3dPos.x < position.x)
			nextSpaceNodeIndex += 1;		
		
		return AddObject(object, parent->next[nextSpaceNodeIndex], nTreeLevel + 1);;
	}

}

void CSpacePartition::AddObject(std::shared_ptr<CGameObject> pObject)
{
	if (AddObject(pObject, m_head, 1))
		return;
	else
		MessageBox(g_hWnd, L"노드추가", L"트리", MB_OK);
	
}

/* 원래 있어야될 자리 원래 있어야 될 자리와 현재 노드를 비교한다.*/ 
SpaceNode* CSpacePartition::FindAddObject(std::shared_ptr<CGameObject> object, SpaceNode* parent, int nTreeLevel)
{
	if (nTreeLevel == m_nMaxTreeLevel) 
		return parent;

	else
	{
		int nextSpaceNodeIndex = 0;
		XMFLOAT3 position = object->GetPositionXMFLOAT3();
		#ifdef _Bounding
				XMFLOAT3 d3dPos = parent->boundingbox.Center;
		#else
			XMFLOAT3 d3dPos = parent->fCenter;
		#endif

		if (d3dPos.z > position.z)
			nextSpaceNodeIndex += 2;

		if (d3dPos.x < position.x)
			nextSpaceNodeIndex += 1;

		return CSpacePartition::FindAddObject(object, parent->next[nextSpaceNodeIndex], nTreeLevel + 1);;
	}
}

//bool -> int 변경하여 어느쪽으로 이동했는지를 알아낸다.
bool CSpacePartition::IsSameSpaceNode(std::shared_ptr<CGameObject> object, SpaceNode* parent)
{
	XMFLOAT3 position = object->GetPositionXMFLOAT3();

	#ifdef _Bounding	
		XMVECTOR xmvMin = XMLoadFloat3(&(parent->boundingbox.Center)) - XMLoadFloat3(&(parent->fSize));
		XMVECTOR xmvMax = XMLoadFloat3(&(parent->boundingbox.Center)) + XMLoadFloat3(&(parent->fSize));
	#else
		XMVECTOR xmvMin = XMLoadFloat3(&(parent->fCenter)) - XMLoadFloat3(&(parent->fSize));
		XMVECTOR xmvMax = XMLoadFloat3(&(parent->fCenter)) + XMLoadFloat3(&(parent->fSize));
	#endif

	XMFLOAT3  xmfMin, xmfMax;
	XMStoreFloat3(&xmfMin, xmvMin);
	XMStoreFloat3(&xmfMax, xmvMax);

	if (xmfMin.x > position.x || xmfMax.x < position.x)
	{
		if (xmfMin.y > position.y || xmfMax.y < position.y)
		{
			if (xmfMin.z > position.z || xmfMax.z < position.z)
			{
				return true;
			}
		
		}
	}

	parent->DeleteObject(object);
	return false;
}

//DeleteObject
SpaceNode* CSpacePartition::DeleteObject(std::shared_ptr<CGameObject> object, SpaceNode* parent, int nTreeLevel)
{
	if (nTreeLevel == m_nMaxTreeLevel)
	{
		parent->DeleteObject(object);
		//parent->m_vectorObject.push_back(object);
		return parent;
	}
	else
	{
		int nextSpaceNodeIndex = 0;
		XMFLOAT3 position = object->GetPositionXMFLOAT3();
		
		#ifdef _Bounding
			XMFLOAT3 d3dPos = parent->boundingbox.Center;
		#else
			XMFLOAT3 d3dPos = parent->fCenter;
		#endif

		if (d3dPos.z > position.z)
			nextSpaceNodeIndex += 2;

		if (d3dPos.x < position.x)
			nextSpaceNodeIndex += 1;

		return DeleteObject(object, parent->next[nextSpaceNodeIndex], nTreeLevel + 1);;
	}

}

SpaceNode* CSpacePartition::DeleteObject(std::shared_ptr<CGameObject> pObject)
{
	SpaceNode* space = CSpacePartition::DeleteObject(pObject, m_head, 1);
	if (!space)
		MessageBox(g_hWnd, L"노드추가", L"트리", MB_OK);
	return space;
}

//Update
void CSpacePartition::Update(std::shared_ptr<CGameObject> object)
{
	auto  currentlyRoom = object->GetSpaceNode();
	auto   destinyRoom = FindAddObject(object, m_head, 1);

	if (currentlyRoom == destinyRoom)
		return;
	else
	{
		currentlyRoom->DeleteObject(object);
		destinyRoom->AddObject(object);
		object->SetSpaceNode(destinyRoom);
	}
}

#endif