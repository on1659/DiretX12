#pragma once

#ifdef _QURD_TREE

#include "SpaceNode.h"

struct octOutData
{
	XMFLOAT3 vPos;
	XMFLOAT3 vSize;
	int			nLevel;
	void Set(XMFLOAT3 pos, XMFLOAT3 size, int level)
	{
		vPos = pos; vSize = size; nLevel = level;
	}
};

class CSpacePartition : public CSingleTonBase<CSpacePartition>
{
private:

	//��Ʈ ����̴�
	SpaceNode			*m_head;

	//�ִ� ���̷����̴�
	int					m_nMaxTreeLevel;

	//------------------------------------------------------------
	//�Ÿ��̴�
	XMVECTOR			m_xmvDistance;
	//������ ũ���̴�						  //�����ҋ� 
	XMVECTOR			m_xmvSpaceSize;     //�ѹ��� ����ϴ�
	//���� �������̴�						  //XMVECOTR�� �Ѵ�
	XMVECTOR			m_xmvWorldSize;	  //GetWorldSize�ִ��� ���� ���X
	//------------------------------------------------------------

public:
	//�� ����� �����̴�
	int					m_nNodeNum;

	std::vector<SpaceNode*> m_vLastRoom;




	//������ ������ w�� h�� ������ �߽����� ����������    

	CSpacePartition()
	{
		m_name = "CSpacePartition";

		m_head = nullptr;
		m_nMaxTreeLevel = 0;
		
		m_nNodeNum = 0;
		m_vLastRoom.clear();
	}

	//BulidObejcts

	void Load(float worldSize, float nMaxTreeLevel);

	void Load(float worldWidth, float worldHeight, float Z, float nMaxTreeLevel);

	~CSpacePartition()
	{
	}

	//Add
	void AddChilde(SpaceNode *parent, int nTreeLevel);

	void Insert(SpaceNode *parent, int nodeType, int nTreeLevel);

	//GetOctData
	int GetNodeCount()  { return m_nNodeNum; }

	int GetMaxLevel() { return m_nMaxTreeLevel; }

	XMFLOAT3 GetWorldSize()
	{
		XMFLOAT3 xmfWorldSize;
		XMStoreFloat3(&xmfWorldSize, m_xmvWorldSize);
		return xmfWorldSize;
	}


	//GetNode XMFLOAT3
	void GetNodePosition(std::vector<XMFLOAT3> *vector, SpaceNode *parent, int nTreeLevel);

	void GetNode(std::vector<XMFLOAT3> *vector, SpaceNode* parent, int nTreeLevel);

	std::vector<XMFLOAT3> GetNode();


	//GetData octOutData
	void GetNode(std::vector<octOutData> *vector, SpaceNode* parent, int nTreeLevel);

	void GetNodePosition(std::vector<octOutData> *vector, SpaceNode *parent, int nTreeLevel);

	std::vector<octOutData> GetNodeOct();

	std::vector<SpaceNode*> GetRoomContainer(){return m_vLastRoom;}

	//Find Node
	SpaceNode* FindSpaceNode(XMFLOAT3 position, SpaceNode* parent, int nTreeLevel);

	SpaceNode* FindSpaceNode(XMFLOAT3 position);


	//AddObject
	bool AddObject(std::shared_ptr<CGameObject> object, SpaceNode* parent, int nTreeLevel);

	void AddObject(std::shared_ptr<CGameObject> pObject);


	//DeleteObject
	SpaceNode* DeleteObject(std::shared_ptr<CGameObject> object, SpaceNode* parent, int nTreeLevel);

	SpaceNode* DeleteObject(std::shared_ptr<CGameObject> pObject);

	//Update
	void Update(std::shared_ptr<CGameObject> object);


	//���� �־�ߵ� �ڸ�
	SpaceNode* FindAddObject(std::shared_ptr<CGameObject> object, SpaceNode* parent, int nTreeLevel);

	bool IsSameSpaceNode(std::shared_ptr<CGameObject> object, SpaceNode* parent);

};

#endif
