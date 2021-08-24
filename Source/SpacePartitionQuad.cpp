#include "stdafx.h"
#include "SpacePartitionQuad.h"

#ifdef _QURD_ARRARY

CSpacePartitionQuad::CSpacePartitionQuad()
{
	m_bLoadingInit = false;
}
CSpacePartitionQuad::CSpacePartitionQuad(float width, float look, int nMaxDepth)
{
	m_xmWorldSize.x = width;
	m_xmWorldSize.y = 0.0f;
	m_xmWorldSize.z = look;


	//내부적으론 한개 있는게 1레벨, 네개 있는게 2레벨임
	m_nMaxDepth = (nMaxDepth - 1);

	m_nDepth = pow(2, m_nMaxDepth);

	m_xmSpaceSize.x = m_xmWorldSize.x / m_nDepth;
	m_xmSpaceSize.y = 0.0f;
	m_xmSpaceSize.z = m_xmWorldSize.z / m_nDepth;


}

CSpacePartitionQuad::~CSpacePartitionQuad()
{
	m_vRoom.clear();
}

void CSpacePartitionQuad::ReleseInstance()
{
	m_bLoadingInit = false;
	for (auto &room : m_vRoom) room->ReleseInstance();
	m_vDynamicGameObject.clear();
}

void CSpacePartitionQuad::Load(float width, float look, int nMaxDepth)
{
	m_xmWorldSize.x = width;
	m_xmWorldSize.y = 0.0f;
	m_xmWorldSize.z = look;


	//내부적으론 한개 있는게 1레벨, 네개 있는게 2레벨임
	m_nMaxDepth = (nMaxDepth - 1);

	m_nDepth = (int)pow(2, m_nMaxDepth);

	m_xmSpaceSize.x = m_xmWorldSize.x / m_nDepth;
	m_xmSpaceSize.y = 0.0f;
	m_xmSpaceSize.z = m_xmWorldSize.z / m_nDepth;
	
	float xStart = m_xmSpaceSize.x * 0.5f;
	float zStart = m_xmSpaceSize.z * 0.5f;

	int nCount = 0;
	for (int z = 0; z < m_nDepth; ++z)
	{
		for (int x = 0; x < m_nDepth; ++x)
		{
			std::shared_ptr<CSpaceNodeQuad> room =
				std::make_shared<CSpaceNodeQuad>
				(
					XMFLOAT3
					(
						  (x * m_xmSpaceSize.x) + xStart
						,						    0.0f
						, (z * m_xmSpaceSize.z) + zStart
					)
					, m_xmSpaceSize
					, nCount++
				);

			m_vRoom.push_back(room);
		}
	}

	m_vDynamicGameObject.clear();
}

void CSpacePartitionQuad::FinalLoad()
{

	std::list<std::shared_ptr<CGameObject>> objectContainer;

	for (auto &room : m_vRoom)
	{
		if (m_bLoadingInit)
		{
			objectContainer.clear();
			objectContainer = room->GetDynamicObjectsContainer();
			for (auto &object : objectContainer)
			{
				//object->id = gInstance_id;
				object->StartComponet(object);

				if (object->name == "Monster")
					m_vDynamicGameObject.push_back(object);
			}
		}

		else
		{
			objectContainer.clear();
			objectContainer = room->GetBlendObjectContainer();
			for (auto &object : objectContainer)
			{
				object->StartComponet(object);
			}

			objectContainer.clear();
			objectContainer = room->GetStaticObjectsContainer();
			for (auto &object : objectContainer)
				object->StartComponet(object);

			objectContainer.clear();
			objectContainer = room->GetStaticTerrainContainer();
			for (auto &object : objectContainer)
				object->StartComponet(object);
	

			objectContainer.clear();
			objectContainer = room->GetChlidObjectContainer();
			for (auto &object : objectContainer)
				object->StartComponet(object);
		}

	}
	std::cout << "m_bLoadingInit : " << m_bLoadingInit << std::endl;
	m_bLoadingInit = true;
}


CSpaceNodeQuad* CSpacePartitionQuad::FindSpaceNode(XMFLOAT3 position)
{
	int xNum = ((int)position.x / (int)m_xmSpaceSize.x);
	int zNum = ((int)position.z / (int)m_xmSpaceSize.z);

	if (xNum >= m_nDepth)xNum = m_nDepth - 1;
	if (zNum >= m_nDepth)zNum = m_nDepth - 1; 
	if (xNum  < 0)xNum = 0;
	if (zNum  < 0)zNum = 0;

	return m_vRoom[xNum + (zNum * m_nDepth)].get();
}

std::shared_ptr<CSpaceNodeQuad> CSpacePartitionQuad::FindSpaceNode(XMVECTOR pos)
{
	return std::shared_ptr<CSpaceNodeQuad>();
}

int CSpacePartitionQuad::FindSpaceNum(XMVECTOR xmPosition)
{
	XMFLOAT3 position;
	XMStoreFloat3(&position, xmPosition);
	int xNum = ((int)position.x / (int)m_xmSpaceSize.x);
	int zNum = ((int)position.z / (int)m_xmSpaceSize.z);

	if (xNum >= m_nDepth)xNum = m_nDepth - 1;
	if (zNum >= m_nDepth)zNum = m_nDepth - 1;
	if (xNum  < 0)xNum = 0;
	if (zNum  < 0)zNum = 0;

	return (xNum + (zNum * m_nDepth));
}

bool CSpacePartitionQuad::IsSameNode(std::shared_ptr<CGameObject> object)
{
	return (object->GetSpaceNodeQuad() == FindSpaceNode(object->GetPositionXMFLOAT3()));
}
bool CSpacePartitionQuad::IsSameNode(std::shared_ptr<WorldMatrix> matrix)
{
	return (matrix->GetSpaceNodeQuad() == FindSpaceNode(matrix->GetPositionXMFLOAT3()));
}

void CSpacePartitionQuad::Update(std::shared_ptr<CGameObject> object)
{
	if (object == nullptr)
	{
		std::cout << "CSpacePartitionQuad:Update->Object is nullptr" << std::endl;
		return;
	}
	CSpaceNodeQuad*  currentlyRoom = object->GetSpaceNodeQuad();
	CSpaceNodeQuad*   destinyRoom = FindSpaceNode(object->GetPositionXMFLOAT3());

	if (currentlyRoom == nullptr)
		return;

	if (currentlyRoom == destinyRoom)
		return;

	else
	{
		currentlyRoom->DeleteObject(object);
		destinyRoom->AddObject(object);
		object->SetSpaceNodeQuad(destinyRoom);
	}
}
void CSpacePartitionQuad::UpdateStaticObject(std::shared_ptr<CGameObject> object)
{
	CSpaceNodeQuad*  currentlyRoom = object->GetSpaceNodeQuad();
	CSpaceNodeQuad*   destinyRoom = FindSpaceNode(object->GetPositionXMFLOAT3());

	if (currentlyRoom == nullptr)
		return;

	if (currentlyRoom == destinyRoom)
		return;

	else
	{
		currentlyRoom->DeleteStaticObject(object);
		destinyRoom->AddStaticObject(object);
		object->SetSpaceNodeQuad(destinyRoom);
	}
}
void CSpacePartitionQuad::UpdateBlend(std::shared_ptr<CGameObject> object)
{
	CSpaceNodeQuad*  currentlyRoom = object->GetSpaceNodeQuad();
	CSpaceNodeQuad*   destinyRoom = FindSpaceNode(object->GetPositionXMFLOAT3());
	if (currentlyRoom == destinyRoom)
		return;
	else
	{
		currentlyRoom->DeleteBlendObject(object);
		destinyRoom->AddBlendbject(object);
		object->SetSpaceNodeQuad(destinyRoom);
	}
}
void CSpacePartitionQuad::UpdateChlid(std::shared_ptr<CGameObject> object)
{
	CSpaceNodeQuad*  currentlyRoom = object->GetSpaceNodeQuad();
	CSpaceNodeQuad*   destinyRoom = FindSpaceNode(object->GetPositionXMFLOAT3());
	if (currentlyRoom == destinyRoom)
		return;
	else
	{
		currentlyRoom->DeleteChlidObject(object);
		destinyRoom->AddChlidObject(object);
		object->SetSpaceNodeQuad(destinyRoom);
	}
}
void CSpacePartitionQuad::Update(std::shared_ptr<WorldMatrix> matrix)
{
	CSpaceNodeQuad*  currentlyRoom = matrix->GetSpaceNodeQuad();
	CSpaceNodeQuad*   destinyRoom = FindSpaceNode(matrix->GetPositionXMFLOAT3());
	if (currentlyRoom == destinyRoom)
		return;
	else
	{
		currentlyRoom->DeleteObject(matrix);
		destinyRoom->AddObject(matrix);
		matrix->SetSpaceNodeQuad(destinyRoom);
	}
}

//Add
void CSpacePartitionQuad::AddObject(std::shared_ptr<CGameObject> object)
{
	XMFLOAT3 position = object->GetPositionXMFLOAT3();
	FindSpaceNode(position)->AddObject(object);
}
void CSpacePartitionQuad::AddObject(std::shared_ptr<WorldMatrix> matrix)
{
	XMFLOAT3 position = matrix->GetPositionXMFLOAT3();
	FindSpaceNode(position)->AddObject(matrix);
}
void CSpacePartitionQuad::AddStaticObject(std::shared_ptr<CGameObject> object)
{
	XMFLOAT3 position = object->GetPositionXMFLOAT3();
	FindSpaceNode(position)->AddStaticObject(object);
}

void CSpacePartitionQuad::AddBlendObject(std::shared_ptr<CGameObject> object)
{
	XMFLOAT3 position = object->GetPositionXMFLOAT3();
	FindSpaceNode(position)->AddBlendbject(object);
}

void CSpacePartitionQuad::AddChlidObject(std::shared_ptr<CGameObject> object)
{
	XMFLOAT3 position = object->GetPositionXMFLOAT3();
	FindSpaceNode(position)->AddChlidObject(object);
}

void CSpacePartitionQuad::AddStatictTerrainObject(std::shared_ptr<CGameObject> object)
{
	XMFLOAT3 position = object->GetPositionXMFLOAT3();
	FindSpaceNode(position)->AddStaticTerrainObject(object);
}

//Delete
void CSpacePartitionQuad::DeleteObject(std::shared_ptr<CGameObject> object)
{
	object->GetSpaceNodeQuad()->DeleteObject(object);

	auto obj = find(m_vDynamicGameObject.begin(), m_vDynamicGameObject.end(), object);
	m_vDynamicGameObject.erase(obj);

}
void CSpacePartitionQuad::DeleteObject(std::shared_ptr<WorldMatrix> matrix)
{
	matrix->GetSpaceNodeQuad()->DeleteObject(matrix);
}

void CSpacePartitionQuad::DeleteBlendObject(std::shared_ptr<CGameObject> object)
{
	object->GetSpaceNodeQuad()->DeleteBlendObject(object);
}

void CSpacePartitionQuad::DeletChlidObject(std::shared_ptr<CGameObject> object)
{
	object->GetSpaceNodeQuad()->DeleteChlidObject(object);
}

void CSpacePartitionQuad::DeleteStaticTerrainObject(std::shared_ptr<CGameObject> object)
{
	object->GetSpaceNodeQuad()->DeleteStaticTerrainObject(object);
}

void CSpacePartitionQuad::DynamicDestory(std::shared_ptr<CGameObject> object)
{
	auto node = static_cast<CSpaceNodeQuad*>(object->GetSpaceNodeQuad());
	if(node) node->DynamicDestory(object);
}

void CSpacePartitionQuad::StaticDestory(std::shared_ptr<CGameObject> object)
{
	auto node = static_cast<CSpaceNodeQuad*>(object->GetSpaceNodeQuad());
	if (node) node->StaticDestory(object);
}

void CSpacePartitionQuad::ChlidDestory(std::shared_ptr<CGameObject> object)
{
	auto node = object->GetSpaceNodeQuad();
	if(node) node->ChlidObjectDestory(object);
}

void CSpacePartitionQuad::BlendObjectDestory(std::shared_ptr<CGameObject> object)
{
	object->GetSpaceNodeQuad()->BlendObjectDestory(object);
}

void CSpacePartitionQuad::WorldMatrixDestory(std::shared_ptr<WorldMatrix> matirx)
{
	matirx->GetSpaceNodeQuad()->WorldMatrixDestory(matirx);
}

#endif


