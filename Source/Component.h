#pragma once

class Component : public Object
{

public:
	Component();

	Component(std::string _name);

	//KYT '16.01.21 plus
	/* 
	  ������Ʈ�� ������Ʈ�� ã�ư��� ���̰���, ������ ��������
	 */
	//Component(std::shared_ptr<CGameObject> object, std::string _name = "");
	
	virtual ~Component();

	virtual void Start(std::shared_ptr<CGameObject> gmaeobject);
	virtual void Animate(Object* object, float fTimeElapsed);
	virtual void Animate(float fTimeElapsed);
	virtual void Animate(std::shared_ptr<CGameObject> object, float fTimeElapsed);
	virtual void CreateConstBuffer(ID3D11Device* pd3dDevice);
	virtual void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Destory();

	//KYT '16.05.05
	/*
	  OnPlayerUpdate Player���� �پ��ִ� Componet�鸸 ������ �˴ϴ�.
	*/
	virtual void OnPlayerUpdate(float fTimeElapsed);


	//KYT '16.05.05
	/*
	   Componet�� �Ѱ��ֱ� ������ ����˴ϴ�.
	*/
	virtual bool BeforeMove();

	//KYT '16.04.23
	/*
		�浹�� �� �Լ��� �Ҹ���.
		�׷��� ���� �Լ��� �浹ó�� ������ �����ε��ؼ� ����ϸ� �ȴ�.
	*/
	virtual void Collision(std::shared_ptr<CGameObject> other, ObjectType type);


	//Set Get
	void SetWorldMatrix(std::shared_ptr<CGameObject> gameObject, XMMATRIX& matirx);
	void SetWorldMatrix(std::shared_ptr<CGameObject> gameObject, XMFLOAT4X4& matirx);
	void SetObject(std::shared_ptr<CGameObject> pObject);
	void SetContainerNumber(int n);


	XMMATRIX& GetWorldMatrix(std::shared_ptr<CGameObject> gameObject) const;
	XMFLOAT4X4& GetWorldMatrixXMFLOAT(std::shared_ptr<CGameObject> gameObject) const;
	int GetContainerNumver()  const;

	//Basic Move
	void SetPosition(std::shared_ptr<CGameObject> object, float x, float y, float z);
	void SetPosition(std::shared_ptr<CGameObject> object, XMFLOAT3& d3dxvPosition);
	void SetPosition(std::shared_ptr<CGameObject> object, XMVECTOR& d3dxvPosition);

	void MoveStrafe(std::shared_ptr<CGameObject> object, float fDistance = 1.0f);
	void MoveUp(std::shared_ptr<CGameObject> object, float fDistance = 1.0f);
	void MoveForward(std::shared_ptr<CGameObject> object, float fDistance = 1.0f);

	void Rotate(std::shared_ptr<CGameObject> object, float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(std::shared_ptr<CGameObject> object, XMVECTOR& pd3dxvAxis, float fAngle);

	XMVECTOR& GetPosition(std::shared_ptr<CGameObject> object, bool bIsLocal = true);
	XMVECTOR& GetLook(std::shared_ptr<CGameObject> object, bool bIsLocal = true);
	XMVECTOR& GetUp(std::shared_ptr<CGameObject> object, bool bIsLocal = true);
	XMVECTOR& GetRight(std::shared_ptr<CGameObject> object, bool bIsLocal = true);
	XMFLOAT3& GetPositionXMFLOAT3(std::shared_ptr<CGameObject> object, bool bIsLocal = true);

	void Trasnlate(std::shared_ptr<CGameObject> object, float fSpeed, float x, float y, float z);
	void Trasnlate(std::shared_ptr<CGameObject> object, float fSpeed, XMVECTOR fDir);
	void Trasnlate(std::shared_ptr<CGameObject> object, float fSpeed, XMFLOAT3 fDir);


	//Set Get
	void SetWorldMatrix(XMMATRIX& matirx);
	void SetWorldMatrix(XMFLOAT4X4& matirx);

	  XMMATRIX& GetWorldMatrix() const;
	XMFLOAT4X4& GetWorldMatrixXMFLOAT() const;

	//Basic Move
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& d3dxvPosition);
	void SetPosition(XMVECTOR& d3dxvPosition);
 
	 void MoveStrafe(float fDistance = 1.0f);
	     void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMVECTOR& pd3dxvAxis, float fAngle);

		    XMVECTOR& GetPosition(bool bIsLocal = true);
			    XMVECTOR& GetLook(bool bIsLocal = true);
	              XMVECTOR& GetUp(bool bIsLocal = true);
	           XMVECTOR& GetRight(bool bIsLocal = true);
	XMFLOAT3& GetPositionXMFLOAT3(bool bIsLocal = true);


	void Trasnlate(float fSpeed, float x, float y, float z);
	void Trasnlate(float x, float y, float z);
	void Trasnlate(float fSpeed, XMVECTOR fDir);
	void Trasnlate(float fSpeed, XMFLOAT3 fDir);



protected:
	std::shared_ptr<CGameObject>						m_gameObject;
	int													m_nContainerNum;
};

