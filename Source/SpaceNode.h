

#pragma once

#ifdef _QURD_TREE
/* 주의할 점 */
// Object를 생성하고 이 Object에 Shader을 붙여야 한다.


#include <iostream>
#include <list>
#include <vector>

//#define _Bounding


class CGameObject;
class AABB;
class CCamera;
class CShader;

enum NODE_TYPE
{
	NODE_TYPE_TOP_LEFT_FRONT = 0,
	NODE_TYPE_TOP_RIGHT_FRONT,
	NODE_TYPE_TOP_LEFT_BACK,
	NODE_TYPE_TOP_RIGHT_BACK,
	NODE_TYPE_NUMBER_END,
};

struct SpaceNode : public Object		 //하나의 공간
{

public:
	//중앙지점
#ifdef _Bounding
	BoundingBox boundingbox;
#else
	XMFLOAT3 fCenter;
	//AABB* boundingbox;
#endif


	//포함되어있는 오브젝트들의 벡터리스트
	std::vector<std::shared_ptr<CGameObject>>		m_vectorObject;

	//Level
	int		nLevel; 

	//자식노드
	SpaceNode* next[NODE_TYPE_NUMBER_END];

	//크기
	XMFLOAT3	 fSize;

	//컬링여부
	bool	   bCulling;

	//태그
	//std::string		tag;

	XMFLOAT3	m_Position;
	AABB*		m_aabb;

	SpaceNode();

	SpaceNode(XMFLOAT3 fInputSize);

	SpaceNode(XMFLOAT3 _fCenter, XMFLOAT3 fInputSize);

	~SpaceNode();

	void SetLevel(int nInputLevel)
	{
		nLevel = nInputLevel;
	}

	void SetPosition(XMVECTOR fInputCenter);

	void SetPosition(XMFLOAT3 fInputCenter);

	void SetSize(XMVECTOR fInputSize);

	void SetSize(XMFLOAT3 fInputSize)
	{
		fSize = fInputSize;
	}

	//	void SetTag(std::string inpug){ tag = inpug; }

	AABB* GetAABB();

	void AABBUpdate();

	BoundingBox GetBoundingBox();

	std::vector<std::shared_ptr<CGameObject>> GetObjectsContainer(){ return m_vectorObject; }

	//오브젝트 추가
	void AddObject(std::shared_ptr<CGameObject> object);

	void AddObject(std::shared_ptr<CGameObject> object, std::string tag);

	void DeleteObject(std::shared_ptr<CGameObject> object);

	bool IsInner(std::shared_ptr<CGameObject> object);

};

#endif