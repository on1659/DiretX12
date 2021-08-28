//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "Component.h"
#include "SpaceNodeQuad.h"

#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

class CShader;
class PlayerState;

class CGameObject : public Object
{
public:
	int nGID;

	CGameObject(int nMeshes = 0);
	CGameObject(std::string name, UINT tag = 0);
	virtual ~CGameObject();
	virtual void Release();

private:
	bool								m_bActive;
	bool								m_bIsVisible;

public:
	//XMFLOAT4X4						m_d3dxmtxLocal;
	XMFLOAT4X4							m_d3dxmtxWorld;

	static XMMATRIX						m_d3dxmtxIdentity;

	XMFLOAT4X4							m_d3dxmtxShadow;

protected:
	std::vector<
		std::unique_ptr<CMesh>>			m_vsMeshes;
	//int								m_nMeshes;

	AABB								m_bcMeshBoundingCube;
	BoundingBox							m_bcMeshBoundingbox;

	std::shared_ptr<CMaterial>			m_sMaterial;

	std::shared_ptr<CShader>			m_sShader;

	std::vector<CGameObject*> 			m_vChild;
	CGameObject* 						m_pParent;
	XMFLOAT3							m_xmf3Rotate{ 0.f,0.f, 0.f };


	XMFLOAT4							m_xmf4Quaternion;

public:
	void SetMesh(std::unique_ptr<CMesh> pMesh, int nIndex = 0);

	void SetMesh(CMesh* pMesh);

	CMesh *GetMesh(int nIndex = 0) const { return(m_vsMeshes[nIndex].get()); }
	UINT GetMeshType() const { return((m_vsMeshes.size() > 0) ? m_vsMeshes[0]->GetType() : 0x00); }

	void SetShader(std::shared_ptr<CShader> sShader, bool boundingbox = true);
	std::shared_ptr<CShader> GetShader() const { return(m_sShader ? m_sShader : nullptr); }

	void SetShadowMatrix(XMMATRIX& pd3dxmtxShadow) { XMStoreFloat4x4(&m_d3dxmtxShadow, pd3dxmtxShadow); }
	void SetShadowMatrix(XMVECTOR d3dxvLight, XMVECTOR d3dxPlane);

	void CGameObject::SetMaterial(std::shared_ptr<CMaterial> sMaterial);
	std::shared_ptr<CMaterial> GetMaterial() { return(m_sMaterial); }

	void SetChild(std::shared_ptr<CGameObject> pChild);
	void SetChild(CGameObject* pChild);
	void SetParent(CGameObject* m_pParent);

	CGameObject* GetParent() { return(m_pParent); }

	bool IsVisible(std::shared_ptr<CCamera> pCamera);
	bool IsVisible(std::shared_ptr<CCamera> pCamera, XMVECTOR camPosition);
	void Visible();

	virtual void Update(XMMATRIX *pd3dxmtxParent);
	virtual void Update(float fTimeElapsed);
	virtual void CameraUpdate(float fTimeElapsed) {}

	virtual void Animate(float fTimeElapsed);
	virtual void Animate(std::shared_ptr<CGameObject> gameObject, float fTimeElapsed);
	virtual void Animate(float fTimeElapsed, XMMATRIX *pd3dxmtxParent);

	virtual void OnPrepareRender();
	virtual void RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	void SetActive(bool bActive = true) { m_bActive = bActive; }
	bool IsActive() const { return m_bActive; }

	void GenerateRayForPicking(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxWorld, XMMATRIX *pd3dxmtxView, XMVECTOR *pd3dxvPickRayPosition, XMVECTOR *pd3dxvPickRayDirection);
	int PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);

	XMVECTOR GetQuaternion() { return XMLoadFloat4(&m_xmf4Quaternion); }
	void SetQuaternion(XMVECTOR xmv) { XMStoreFloat4(&m_xmf4Quaternion, xmv); }


public:
	static ID3D11Buffer				*m_pd3dcbWorldMatrix;
	static ID3D11Buffer				*m_pd3dcbMaterialColors;

public:
	//Create
	void Initialize();

	static void CreateConstBuffers(ID3D11Device* pd3dDevice);

	//Release
	static void ReleaseConstBuffers();

	//VS Shader
	static void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX& pd3dxmtxWorld);

	//PS Shader
	static void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, CMaterialColors *pMaterialColors);



	//Component 물리처리 및 로직담당
protected:
	std::vector<std::unique_ptr<Component>>		m_vComponent;
	PlayerState*								m_cPlayerState;

public:

	void SetCompoent(std::unique_ptr<Component> component);

	//KYT '16.04.26
	/*
	Componet를 빼오는 겁니다.
	*/
	void ComponentUpdate(float fTimeElasped);
	int GetComponetSize() const { return (int)m_vComponent.size(); }
	Component* GetComponent(int index);
	Component* GetComponent(std::string name);
	Component* GetComponent(TAG_TYPE tag);
	std::vector<Component*> GetComponents(TAG_TYPE tag);


	std::unique_ptr<Component> MoveCompoent(std::shared_ptr<CGameObject> other, TAG_TYPE tag);
	std::unique_ptr<Component> MoveCompoent(std::shared_ptr<CGameObject> other, int index);
	std::unique_ptr<Component> MoveCompoent(std::shared_ptr<CGameObject> other, std::string name);
	std::unique_ptr<Component> MoveCompoent(std::shared_ptr<CGameObject> other, std::string name, int slot);


	void DestoryComponet(std::string name);
	void DestoryComponetWithId(int obj_id);
	void DestoryComponet(TAG_TYPE tag);
	void DestoryComponet(int index);
	void DestoryItemComponet(std::string name, int slot);
	void DestoryItemComponet(int id, int slot);
	void DestoryChild();

	void StartComponet(std::shared_ptr<CGameObject> object)
	{
		int n = 0;
		for (auto &component : m_vComponent)
		{
			component->Start(object);
			component->SetContainerNumber(n++);
		}
	}



	//Move & Rotateion Fucntion
public:
	void SetPosition(float x, float y, float z = 0);
	
	void SetPositionX(const float& x){ m_d3dxmtxWorld._41 = x; }
	void SetPositionY(const float& y){ m_d3dxmtxWorld._42 = y; }
	void SetPositionZ(const float& z){ m_d3dxmtxWorld._43 = z; }

	void SetPosition(const XMFLOAT3& d3dxvPosition);
	void SetPosition(const XMVECTOR& d3dxvPosition);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	virtual void RotateWorldAxis(const float& x, const float& y, const float& z);
	virtual void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	virtual void Rotate(XMVECTOR& pd3dxvAxis, float fAngle);

	float& GetRotateX() { return m_xmf3Rotate.x; };
	float& GetRotateY() { return m_xmf3Rotate.y; };
	float& GetRotateZ() { return m_xmf3Rotate.z; };

	XMVECTOR& GetPosition(bool bIsLocal = true);

	float& GetPositionX(const bool& bIsLocal = true) {return m_d3dxmtxWorld._41;}
	float& GetPositionY(const bool& bIsLocal = true) {return m_d3dxmtxWorld._42;}
	float& GetPositionZ(const bool& bIsLocal = true) {return m_d3dxmtxWorld._43;}



	XMVECTOR& GetLook(bool bIsLocal = true);
	XMVECTOR& GetUp(bool bIsLocal = true);
	XMVECTOR& GetRight(bool bIsLocal = true);

	XMFLOAT3& GetPositionXMFLOAT3(bool bIsLocal = true);

	//Set Get
	//void SetLocalMatrix(XMMATRIX& matirx);
	//void SetLocalMatrix(XMFLOAT4X4& matirx);
	void SetWorldMatrix(XMMATRIX& matirx);
	void SetWorldMatrix(XMFLOAT4X4& matirx);

	//XMMATRIX& GetLocalMatrix() { m_xmMatrix = XMLoadFloat4x4(&m_d3dxmtxWorld); return m_xmMatrix; }
	//XMFLOAT4X4& GetLocalMatrixXMFLOAT() { return m_d3dxmtxWorld; }

	XMMATRIX& GetWorldMatrix() { m_xmMatrix = XMLoadFloat4x4(&m_d3dxmtxWorld); return m_xmMatrix; }
	XMFLOAT4X4& GetWorldMatrixFLOAT() { return m_d3dxmtxWorld; }




	//공간분할
#ifdef _QURD_ARRARY
private:
	CSpaceNodeQuad*										m_SpaceNodeQuad;

public:
	//공간분할
	CSpaceNodeQuad*	GetSpaceNodeQuad()const { return m_SpaceNodeQuad; }
	void SetSpaceNodeQuad(CSpaceNodeQuad* space) { m_SpaceNodeQuad = space; }
#else
private:
	SpaceNode*										m_SpaceNode;

public:
	void SetSpaceNode(SpaceNode* space) { m_SpaceNode = space; }

	SpaceNode*	 GetSpaceNode() { return m_SpaceNode; }
#endif

public:

	virtual std::shared_ptr<CShader> SpacePartitioningFrustm(CCamera *pCamera);

	void SetBoundingBox(CMesh* pMesh);

	void SetBoundingBox(BoundingBox boundingBox);

	virtual BoundingBox GetBoundingBox()
	{
		m_bcMeshBoundingbox.Center = XMFLOAT3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43);
		return m_bcMeshBoundingbox;
	}

	AABB GetAABB()
	{
		m_bcMeshBoundingCube.Update(XMLoadFloat4x4(&m_d3dxmtxWorld));
		return m_bcMeshBoundingCube;
	}

	//UI
	void UIRender();


	//이동속도
	float GetMovementspeed() const { return m_movementSpeed; }
	void SetMovementspeed(float newspeed) { m_movementSpeed = newspeed; }

	//애니메이션 정보
	int GetCurrentKeyframe() const { return (int)m_currentKeyframe; }
	AnimState GetCurrentAnimationState() const { return (AnimState)m_currentAnimationState; }
	float GetAnimationSpeed() const { return m_animPlaySpeed; }

	void SetAnimationSpeed(float speed) { m_animPlaySpeed = speed; };
	void SetCurrentKeyframe(int keyframe) { m_currentKeyframe = (float)keyframe; }
	void SetCurrentAnimationState(AnimState anim) { m_currentAnimationState = anim; }

protected:
	float						m_movementSpeed;

	float						m_currentKeyframe;
	float						m_animPlaySpeed;
	AnimState					m_currentAnimationState;


	//KYT '16.04.23
	/*
	충돌 했을 시 컴포넌트에 있는 충돌처리를 돌리기 위하여
	*/
public:
	void IntersectsDynamic(std::shared_ptr<CGameObject> other);
	void IntersectsStatic(std::shared_ptr<CGameObject> other);
	void IntersectsWorldMatrix(std::shared_ptr<WorldMatrix> matrix);
	void IntersectsPlayer(std::shared_ptr<CGameObject> other);

	void DynamicDestory(std::shared_ptr<CGameObject> gameObject);
	void StaticDestory(std::shared_ptr<CGameObject> gameObject);
	void BlendDestory(std::shared_ptr<CGameObject> gameObject);
	void ChildDestory(std::shared_ptr<CGameObject> gameObject);
	void WorldMatrixDestory(std::shared_ptr<WorldMatrix> matrix);

};


