
#include "stdafx.h"
#include "SpaceNodeQuad.h"

#ifdef _QURD_ARRARY

CSpaceNodeQuad::CSpaceNodeQuad()
{
	fCenter = { 0.0f, 0.0f, 0.0f };

	fSize = { 0.0f, 0.0f, 0.0f };

	bCulling = true;

	m_vDynamicObject.clear();
	m_vMatirx.clear();
	m_vStaticTerrainObject.clear();

	boundingbox.Center = { 0.0f, 0.0f, 0.0f };
	boundingbox.Extents = { 0.0f, 0.0f, 0.0f };

	nRoomContainerCount = 0;
	nGID = gInstanceID++;
}
CSpaceNodeQuad::CSpaceNodeQuad(XMFLOAT3 center, XMFLOAT3 size)
{
	bCulling = true;

	m_vDynamicObject.clear();
	
	nRoomContainerCount = 0;

	CSpaceNodeQuad::Set(center, size);
	nGID = gInstanceID++;

}
CSpaceNodeQuad::CSpaceNodeQuad(XMFLOAT3 center, XMFLOAT3 size, int nRoomCount)
{
	bCulling = true;

	m_vDynamicObject.clear();

	nRoomContainerCount = nRoomCount;

	CSpaceNodeQuad::Set(center, size);
	nGID = gInstanceID++;
}
CSpaceNodeQuad::~CSpaceNodeQuad()
{
	m_vDynamicObject.clear();
	m_vStaticObject.clear();
	m_vMatirx.clear();
	m_vStaticTerrainObject.clear();
	m_vBlendObject.clear();
	m_vChlidObject.clear();
}

void CSpaceNodeQuad::ReleseInstance()
{
	for(auto &obj: m_vDynamicObject)		{ obj->Release(); }
	for(auto &obj: m_vStaticObject)			{ obj->Release(); }
	for(auto &obj: m_vMatirx)				{ obj->Release(); }
	for(auto &obj: m_vStaticTerrainObject)	{ obj->Release(); }
	for(auto &obj: m_vBlendObject)			{ obj->Release(); }
	for(auto &obj: m_vChlidObject)			{ obj->Release(); }

	m_vDynamicObject.clear();
	m_vStaticObject.clear();
	m_vMatirx.clear();
	m_vStaticTerrainObject.clear();
	m_vBlendObject.clear();
	m_vChlidObject.clear();

}

void CSpaceNodeQuad::AddObject(std::shared_ptr<CGameObject> object)
{
	object->SetSpaceNodeQuad(this);
	m_vDynamicObject.push_back(object);
}
void CSpaceNodeQuad::AddObject(std::shared_ptr<WorldMatrix> matrix)
{
	matrix->SetSpaceNodeQuad(this);
	m_vMatirx.push_back(matrix);
}
void CSpaceNodeQuad::AddBlendbject(std::shared_ptr<CGameObject> object)
{
	object->SetSpaceNodeQuad(this);
	m_vBlendObject.push_back(object);
}
void CSpaceNodeQuad::AddStaticObject(std::shared_ptr<CGameObject> object)
{
	object->SetSpaceNodeQuad(this);
	m_vStaticObject.push_back(object);
}

void CSpaceNodeQuad::AddChlidObject(std::shared_ptr<CGameObject> object)
{
	object->SetSpaceNodeQuad(this);
	m_vChlidObject.push_back(object);
}

void CSpaceNodeQuad::AddStaticTerrainObject(std::shared_ptr<CGameObject> object)
{
	object->SetSpaceNodeQuad(this);
	m_vStaticTerrainObject.push_back(object);
}

void CSpaceNodeQuad::DeleteObject(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vDynamicObject.begin(), m_vDynamicObject.end(), object);
	m_vDynamicObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
}

void CSpaceNodeQuad::DeleteStaticObject(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vStaticObject.begin(), m_vStaticObject.end(), object);
	m_vStaticObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
}

void CSpaceNodeQuad::DeleteObject(std::shared_ptr<WorldMatrix> matrix)
{
	auto findobject = std::find(m_vMatirx.begin(), m_vMatirx.end(), matrix);
	m_vMatirx.erase(findobject);
	matrix->SetSpaceNodeQuad(nullptr);
}

void CSpaceNodeQuad::DeleteBlendObject(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vBlendObject.begin(), m_vBlendObject.end(), object);
	m_vBlendObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
}
void CSpaceNodeQuad::DeleteChlidObject(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vChlidObject.begin(), m_vChlidObject.end(), object);
	m_vChlidObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
}
void CSpaceNodeQuad::DeleteStaticTerrainObject(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vStaticTerrainObject.begin(), m_vStaticTerrainObject.end(), object);
	m_vStaticTerrainObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
}

bool CSpaceNodeQuad::IsInner(std::shared_ptr<CGameObject> object)
{
	return (std::find(m_vDynamicObject.begin(), m_vDynamicObject.end(), object) != m_vDynamicObject.end());
}
bool CSpaceNodeQuad::IsInner(std::shared_ptr<WorldMatrix> matrix)
{
	return (std::find(m_vMatirx.begin(), m_vMatirx.end(), matrix) != m_vMatirx.end());
}

bool CSpaceNodeQuad::IsBlendObjectInner(std::shared_ptr<CGameObject> object)
{
	return (std::find(m_vBlendObject.begin(), m_vBlendObject.end(), object) != m_vBlendObject.end());
}

bool CSpaceNodeQuad::IsChildObjectInner(std::shared_ptr<CGameObject> object)
{
	return (std::find(m_vChlidObject.begin(), m_vChlidObject.end(), object) != m_vChlidObject.end());
}

bool CSpaceNodeQuad::IsStaticTerrainInner(std::shared_ptr<CGameObject> object)
{
	return (std::find(m_vStaticTerrainObject.begin(), m_vStaticTerrainObject.end(), object) != m_vStaticTerrainObject.end());
}

void CSpaceNodeQuad::DynamicDestory(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vDynamicObject.begin(), m_vDynamicObject.end(), object);
	m_vDynamicObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
	object->SetShader(nullptr);
	object.reset();
}
void CSpaceNodeQuad::StaticDestory(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vStaticObject.begin(), m_vStaticObject.end(), object);
	m_vStaticObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
	object->SetShader(nullptr);
	object.reset();
}
void CSpaceNodeQuad::WorldMatrixDestory(std::shared_ptr<WorldMatrix> matrix)
{
	auto findobject = std::find(m_vMatirx.begin(), m_vMatirx.end(), matrix);
	m_vMatirx.erase(findobject);
	matrix->SetSpaceNodeQuad(nullptr);
	matrix.reset();
}

void CSpaceNodeQuad::StaticTerrainDestory(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vStaticTerrainObject.begin(), m_vStaticTerrainObject.end(), object);
	m_vStaticTerrainObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
	object->SetShader(nullptr);
	object.reset();
}

void CSpaceNodeQuad::ChlidObjectDestory(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vChlidObject.begin(), m_vChlidObject.end(), object);
	if (findobject == m_vChlidObject.end()) return;
	m_vChlidObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
	object->SetShader(nullptr);
	object.reset();
}

void CSpaceNodeQuad::BlendObjectDestory(std::shared_ptr<CGameObject> object)
{
	auto findobject = std::find(m_vBlendObject.begin(), m_vBlendObject.end(), object);
	m_vBlendObject.erase(findobject);
	object->SetSpaceNodeQuad(nullptr);
	object->SetShader(nullptr);
	object.reset();
}

std::shared_ptr<CSpaceNodeQuad> CSpaceNodeQuad::FindSpaceNode(XMFLOAT3 position)
{
		if (aabb.m_d3dxvMinimum.x < position.x && position.x < aabb.m_d3dxvMaximum.x)
		{
			if (aabb.m_d3dxvMinimum.y < position.y && position.y < aabb.m_d3dxvMaximum.y)
			{
				if (aabb.m_d3dxvMinimum.z < position.z && position.z < aabb.m_d3dxvMaximum.z)
					return (std::shared_ptr<CSpaceNodeQuad>)this;
			}
		}
		return nullptr;


		if (boundingbox.Center.x < position.x && position.x < (boundingbox.Center.x + boundingbox.Extents.x))
		{
				if (boundingbox.Center.z < position.z && position.z <  (boundingbox.Center.z + boundingbox.Extents.z))
					return (std::shared_ptr<CSpaceNodeQuad>)this;
		}
		return nullptr;
}

//Get & Set
void CSpaceNodeQuad::Set(XMFLOAT3 center, XMFLOAT3 size)
{
	CSpaceNodeQuad::SetSize(size);
	CSpaceNodeQuad::SetCenter(center);

	boundingbox.Center = fCenter;
	boundingbox.Extents = XMFLOAT3{(fSize.x * 0.5f), 0.0f, (fSize.z * 0.5f) };

	aabb.m_d3dxvMinimum = XMFLOAT3{ fCenter.x - (fSize.x * 0.5f), -100000.0f, fCenter.z - (fSize.z * 0.5f) };
	aabb.m_d3dxvMaximum = XMFLOAT3{ fCenter.x + (fSize.x * 0.5f),  100000.0f, fCenter.z + (fSize.z * 0.5f) };
}
void CSpaceNodeQuad::SetCenter(XMFLOAT3 center)
{
	fCenter = center;
	boundingbox.Center = fCenter;
	boundingbox.Extents = XMFLOAT3{ (fSize.x * 0.5f), 0.0f, (fSize.z * 0.5f) };

	if (fSize.x + fSize.y != 0)
	{
		aabb.m_d3dxvMinimum = XMFLOAT3{ fCenter.x - (fSize.x * 0.5f), 0.0f, fCenter.z - (fSize.z * 0.5f) };
		aabb.m_d3dxvMaximum = XMFLOAT3{ fCenter.x + (fSize.x * 0.5f), 0.0f, fCenter.z + (fSize.z * 0.5f) };
	}
}
void CSpaceNodeQuad::SetSize(XMFLOAT3 size)
{
	fSize = size;
	boundingbox.Extents = XMFLOAT3{ (fSize.x * 0.5f), 0.0f, (fSize.z * 0.5f) };

	//Center은 절대 0,0 일 수 없다
	if (fCenter.x + fCenter.y != 0)
	{
		aabb.m_d3dxvMinimum = XMFLOAT3{ fCenter.x - (fSize.x * 0.5f), 0.0f, fCenter.z - (fSize.z * 0.5f) };
		aabb.m_d3dxvMaximum = XMFLOAT3{ fCenter.x + (fSize.x * 0.5f), 0.0f, fCenter.z + (fSize.z * 0.5f) };
	}
}

#endif