#pragma once

#include "stdafx.h"
#include <thread>
#include "MonsterState.h"

#define SingleThreadMoving

class CMonsterPatrol : public Component
{
public:
	XMVECTOR				m_xmTargetPosition;
	//XMVECTOR				m_xmDir;
	float					m_fSpeed;

	std::thread*			m_tSearchPathThread;
	int						m_i;
	
	std::deque<XMFLOAT2>	m_qMovePath;
	std::deque<XMFLOAT2>    m_qNextMovePath;
	int						m_visited[MAR_RANGE * 2][MAR_RANGE * 2] = { 0, 0 };

	MonsterState*			cMonsterState;

public:

	CMonsterPatrol(std::string _name = "")
	{
		name = "CMonsterPatrol";
		tag = TAG_TYPE::TAG_MonsterPatrol;
		cMonsterState = nullptr;
	}

	virtual ~CMonsterPatrol()
	{
		m_tSearchPathThread = nullptr;
		cMonsterState = nullptr;
	}

public:

	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		#ifndef _CONNECT_SERVER_
			Component::Start(gameObject);
			cMonsterState = static_cast<MonsterState*>(m_gameObject->GetComponent(0));

			m_fSpeed = 5.5f;
			m_qNextMovePath.clear();

			InitiallizeGetPath();
			SetDirection(TIMEMGR->GetTimeElapsed());
		#endif

	}

	virtual void Animate(float fTimeElapsed)
	{
		#ifndef _CONNECT_SERVER_
			if (cMonsterState->GetTarget())return;
			if (cMonsterState->IsHeadBang())return;

			XMFLOAT3 xmFloat3;
			XMStoreFloat3(&xmFloat3, XMVector3Length(m_xmTargetPosition - m_gameObject->GetPosition()));

			if (xmFloat3.x <= 1.0f)
				SetDirection(fTimeElapsed);

			Trasnlate(m_gameObject, m_fSpeed * fTimeElapsed, cMonsterState->GetDirection());
			//if(false == INPUT->KeyDown(YK_SPACE))
		#endif
	}

	virtual void InitializePath(int i)
	{
		m_i = i;
	}

private:
	void InitiallizeGetPath()
	{
		int nx, nz;
		XMFLOAT3 position = m_gameObject->GetPositionXMFLOAT3();
		nx = Helper::Rounding(position.x) / BLOCK_SCALE;
		nz = Helper::Rounding(position.z) / BLOCK_SCALE;

		if (nx < 0 || nz < 0) nx = 1;
		if (nx >= 251) nx = 251;
		if (nz >= 159) nz = 159;

		m_qMovePath.clear();

		while(m_qMovePath.empty())
			m_qMovePath = Helper::BFS(CONTENTMGR->GetMapArrary(), nx, nz, 252, 160);
		
		GetPathThread();
	}

	void GetPath()
	{
		int nx, nz;
		XMFLOAT3 position = m_gameObject->GetPositionXMFLOAT3();
		nx = Helper::Rounding(position.x) / BLOCK_SCALE;
		nz = Helper::Rounding(position.z) / BLOCK_SCALE;

		if (nx < 0)  nx = 0;
		if (nz < 0 ) nz = 0;
		if (nx >= 251) nx = 251;
		if (nz >= 159) nz = 159;

		m_qMovePath.clear();
		m_qMovePath = Helper::BFS(CONTENTMGR->GetMapArrary(), nx, nz, 252, 160);
	}

	void SetDirection(float fTimeElapsed)
	{
		if (m_qMovePath.empty())
		{
			#ifdef SingleThreadMoving
				GetPath();
			#else
				m_tSearchPathThread->join();
				m_qMovePath.clear();
				m_qMovePath = m_qNextMovePath;
				m_qNextMovePath.clear();
				GetPathThread();
			#endif
		}

		XMFLOAT3 xmPosition = m_gameObject->GetPositionXMFLOAT3();
		XMVECTOR xvPosition = m_gameObject->GetPosition();

		XMFLOAT2 xmfloat2 = m_qMovePath.back();
		XMFLOAT3 xmfloat3{ xmfloat2.x, xmPosition.y, xmfloat2.y };
		m_xmTargetPosition = XMLoadFloat3(&xmfloat3);
		cMonsterState->SetDirection(XMVector3Normalize(m_xmTargetPosition - xvPosition));
		//XMStoreFloat3(&xmfloat3, m_xmDir); 
		m_qMovePath.pop_back(); 

		SetRotate(fTimeElapsed);
	}

	void SetRotate(float fTimeElapsed)
	{
		XMFLOAT3 position = m_gameObject->GetPositionXMFLOAT3();
		position.x = Helper::Rounding(position.x) / BLOCK_SCALE;
		position.z = Helper::Rounding(position.z) / BLOCK_SCALE;


		XMVECTOR vPosition = XMVectorSet(position.x, position.y, position.z, 1.0f);
		XMVECTOR vTarget = m_xmTargetPosition;
		XMVECTOR vToTarget;
		vToTarget = vTarget - vPosition;

		XMFLOAT3 xmFloat3;
		XMStoreFloat3(&xmFloat3, XMVector3Length(vToTarget));
		{
			XMVECTOR vLook = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
			vLook = m_gameObject->GetLook();  XMVector3TransformNormal(vLook, m_gameObject->GetWorldMatrix());
			vToTarget = XMVector3Normalize(vToTarget);
			
			XMVECTOR vCrossProduct;
			vCrossProduct = XMVector3Cross(vLook, vToTarget);
			
			float fDotProduct;
			XMFLOAT3 look; 
			XMStoreFloat3(&look, vLook);
			XMStoreFloat3(&xmFloat3, XMVector3Dot(cMonsterState->GetDirection(), vLook));
			fDotProduct = xmFloat3.x;

			float fAngle = (fDotProduct > 0.0f) ? D3DXToDegree(acos(fDotProduct)) : 90.0f;
			XMStoreFloat3(&xmFloat3, vCrossProduct);
			fAngle *= (xmFloat3.y > 0.0f) ? 1.0f : -1.0f;
			XMMATRIX mtxRotation;
			mtxRotation = XMMatrixRotationY(fAngle);
			m_gameObject->SetWorldMatrix(mtxRotation * m_gameObject->GetWorldMatrix());

			XMFLOAT4X4 xmWorld;
			XMStoreFloat4x4(&xmWorld, m_gameObject->GetWorldMatrix());

			m_gameObject->Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), fAngle * fTimeElapsed);

			//xmWorld._41 += m_gameObject->GetWorldMatrixFLOAT()._31 * (3.0f * fTimeElapsed);
			//xmWorld._42 += m_gameObject->GetWorldMatrixFLOAT()._32 * (3.0f * fTimeElapsed);
			//xmWorld._43 += m_gameObject->GetWorldMatrixFLOAT()._33 * (3.0f * fTimeElapsed);
			//m_gameObject->SetLocalMatrix(xmWorld);
			//m_gameObject->SetWorldMatrix(xmWorld);

		}
	}


	//Path를 얻는데 Thread로 얻는다.
	void GetPathThread()
	{
		//Next Path
		XMFLOAT2 positionF2 = m_qMovePath[0];
		int nx = Helper::Rounding(positionF2.x) / static_cast<int>(BLOCK_SCALE);
		int nz = Helper::Rounding(positionF2.y) / static_cast<int>(BLOCK_SCALE);

		if (nx < 0 || nz < 0) nx = 1;
		if (nx >= 251) nx = 251;
		if (nz >= 159) nz = 159;

		m_tSearchPathThread = new std::thread([&]() {MonsterPreviousBFS(CONTENTMGR->GetMapArrary()); });

	}

	//멤버함수를 쓰레드로 돌리자
	void MonsterPreviousBFS(int **gMap, int ix, int iy, int w, int h)
	{
		std::queue<XMFLOAT2> que;
		//int visited[MAR_RANGE * 2][MAR_RANGE * 2] = { 0, 0 };
		memset(&m_visited, 0, sizeof(m_visited));

		int ax, ay;
		
		ax = ((MAR_RANGE - 1) - ix); 
		ay = ((MAR_RANGE - 1) - iy);

		if (ix < MAR_RANGE * 2) ax = 0;
		if (iy < MAR_RANGE * 2) ay = 0;

		XMFLOAT2 endPosition;
		endPosition.x = -1; endPosition.y = -1;

		XMFLOAT2 startPosition;
		startPosition.x = ix; startPosition.y = iy;
		
		int n = 5 + rand() % (MAR_RANGE - 6);
		que.push(startPosition);
		m_visited[(MAR_RANGE - 1)][(MAR_RANGE - 1)] = 1;

		while (true)
		{
			XMFLOAT2 pos = que.front(); que.pop();
			int mx, my;
			int count;
			mx = pos.x;
			my = pos.y;
			count = m_visited[my + ay][mx + ax];

			/*
				중복되지않는 4개의 숫자 뽑는 알고리즘을 사용해서 4가지 움직이는 방향을 랜덤으로 한다.
			*/
			#pragma region[4가지 방향을 랜덤으로]
				bool bRand[4] = { false };
				int nCount = 3;

				int moveNumber;

				while (1)
				{
					moveNumber = rand() % 4;

					while (bRand[moveNumber])
					{
						moveNumber = rand() % 4;
					}
					bRand[moveNumber] = true;


					if (moveNumber == 0)
					{
						if (my + 1 < h)
						{
							
							if (CONTENTMGR->GetMapArrary(my + 1, mx) == 0 && m_visited[my + 1 + ay][mx + ax] == 0)
							{
								que.push(XMFLOAT2(mx, my + 1));
								m_visited[my + 1 + ay][mx + ax] = count + 1;
							}
						}
					}

					else if (moveNumber == 1)
					{
						if (my - 1 >= 0)
						{
							if (CONTENTMGR->GetMapArrary(my - 1, mx) == 0 && m_visited[my - 1 + ay][mx + ax] == 0)
							{
								que.push(XMFLOAT2(mx, my - 1));
								m_visited[my - 1 + ay][mx + ax] = count + 1;
							}
						}
					}

					else if (moveNumber == 2)
					{
						if (mx + 1 < w)
						{
							if (CONTENTMGR->GetMapArrary(my, mx + 1) == 0 && m_visited[my + ay][mx + 1 + ax] == 0)
							{
								que.push(XMFLOAT2(mx + 1, my));
								m_visited[my + ay][mx + 1 + ax] = count + 1;
							}
						}

					}

					else
					{
						if (mx - 1 >= 0)
						{
							if (CONTENTMGR->GetMapArrary(my, mx - 1) == 0 && m_visited[my + ay][mx - 1 + ax] == 0)
							{
								que.push(XMFLOAT2(mx - 1, my));
								m_visited[my + ay][mx - 1 + ax] = count + 1;
							}
						}
					}

					bool check = bRand[0];
					for (int i = 0; i < 4; ++i)
						check = bRand[i] & check;
					if (check)break;

				}

			#pragma endregion	

			if (que.empty() || n == count)
			{
				endPosition.x = mx;
				endPosition.y = my;
				break;
			}

		}

		int ex, ey;
		ex = endPosition.x + ax; ey = endPosition.y + ay;
		int nPathLoop = 0;
		//path.reserve(visited[ey - ay][ex - ax]);
		m_qNextMovePath.push_back(XMFLOAT2(ex, ey));

		while (1)
		{
			int mx, my;
			int count;
			mx = ex; my = ey;
			count = m_visited[my][mx];

			if (count == 1 || ++nPathLoop > MAR_RANGE)
				break;

			if (m_visited[my + 1][mx] == count - 1)
			{
				ex = mx;
				ey = my + 1;
				m_qNextMovePath.push_back(XMFLOAT2(ex, ey));
			}
			else if (m_visited[my - 1][mx] == count - 1)
			{
				ex = mx;
				ey = my - 1;
				m_qNextMovePath.push_back(XMFLOAT2(ex, ey));
			}
			else if (m_visited[my][mx + 1] == count - 1)
			{
				ex = mx + 1;
				ey = my;
				m_qNextMovePath.push_back(XMFLOAT2(ex, ey));
			}
			else if (m_visited[my][mx - 1] == count - 1)
			{
				ex = mx - 1;
				ey = my;
				m_qNextMovePath.push_back(XMFLOAT2(ex, ey));
			}

		}
	};

	void MonsterPreviousBFS(int **gMap)
	{
		std::queue<XMFLOAT2> que;
		//int visited[MAR_RANGE * 2][MAR_RANGE * 2] = { 0, 0 };
		memset(&m_visited, 0, sizeof(m_visited));

		while (m_qMovePath.empty())
			return;

		XMFLOAT2 positionF2 = m_qMovePath[0];
		int ix = Helper::Rounding(positionF2.x) / static_cast<int>(BLOCK_SCALE);
		int iy = Helper::Rounding(positionF2.y) / static_cast<int>(BLOCK_SCALE);

		int w = 252;
		int h = 160;


		int ax, ay;

		ax = ((MAR_RANGE - 1) - ix);
		ay = ((MAR_RANGE - 1) - iy);

		if (ix < MAR_RANGE * 2) ax = 0;
		if (iy < MAR_RANGE * 2) ay = 0;

		XMFLOAT2 endPosition;
		endPosition.x = -1; endPosition.y = -1;

		XMFLOAT2 startPosition;
		startPosition.x = ix; startPosition.y = iy;

		int n = 5 + rand() % (MAR_RANGE - 6);
		que.push(startPosition);
		m_visited[(MAR_RANGE - 1)][(MAR_RANGE - 1)] = 1;

		while (true)
		{
			XMFLOAT2 pos = que.front(); que.pop();
			int mx, my;
			int count;
			mx = pos.x;
			my = pos.y;
			count = m_visited[my + ay][mx + ax];

			/*
			중복되지않는 4개의 숫자 뽑는 알고리즘을 사용해서 4가지 움직이는 방향을 랜덤으로 한다.
			*/
#pragma region[4가지 방향을 랜덤으로]
			bool bRand[4] = { false };
			int nCount = 3;

			int moveNumber;

			while (1)
			{
				moveNumber = rand() % 4;

				while (bRand[moveNumber])
				{
					moveNumber = rand() % 4;
				}
				bRand[moveNumber] = true;


				if (moveNumber == 0)
				{
					if (my + 1 < h)
					{

						if (CONTENTMGR->GetMapArrary(my + 1, mx) == 0 && m_visited[my + 1 + ay][mx + ax] == 0)
						{
							que.push(XMFLOAT2(mx, my + 1));
							m_visited[my + 1 + ay][mx + ax] = count + 1;
						}
					}
				}

				else if (moveNumber == 1)
				{
					if (my - 1 >= 0)
					{
						if (CONTENTMGR->GetMapArrary(my - 1, mx) == 0 && m_visited[my - 1 + ay][mx + ax] == 0)
						{
							que.push(XMFLOAT2(mx, my - 1));
							m_visited[my - 1 + ay][mx + ax] = count + 1;
						}
					}
				}

				else if (moveNumber == 2)
				{
					if (mx + 1 < w)
					{
						if (CONTENTMGR->GetMapArrary(my, mx + 1) == 0 && m_visited[my + ay][mx + 1 + ax] == 0)
						{
							que.push(XMFLOAT2(mx + 1, my));
							m_visited[my + ay][mx + 1 + ax] = count + 1;
						}
					}

				}

				else
				{
					if (mx - 1 >= 0)
					{
						if (CONTENTMGR->GetMapArrary(my, mx - 1) == 0 && m_visited[my + ay][mx - 1 + ax] == 0)
						{
							que.push(XMFLOAT2(mx - 1, my));
							m_visited[my + ay][mx - 1 + ax] = count + 1;
						}
					}
				}

				bool check = bRand[0];
				for (int i = 0; i < 4; ++i)
					check = bRand[i] & check;
				if (check)break;

			}

#pragma endregion	

			if (que.empty() || n == count)
			{
				endPosition.x = mx;
				endPosition.y = my;
				break;
			}

		}

		int ex, ey;
		ex = endPosition.x + ax; ey = endPosition.y + ay;
		int nPathLoop = 0;
		//path.reserve(visited[ey - ay][ex - ax]);
		m_qNextMovePath.push_back(XMFLOAT2(ex, ey));

		while (1)
		{
			int mx, my;
			int count;
			mx = ex; my = ey;
			count = m_visited[my][mx];

			if (count == 1 || ++nPathLoop > MAR_RANGE)
				break;

			if (m_visited[my + 1][mx] == count - 1)
			{
				ex = mx;
				ey = my + 1;
				m_qNextMovePath.push_back(XMFLOAT2(ex, ey));
			}
			else if (m_visited[my - 1][mx] == count - 1)
			{
				ex = mx;
				ey = my - 1;
				m_qNextMovePath.push_back(XMFLOAT2(ex, ey));
			}
			else if (m_visited[my][mx + 1] == count - 1)
			{
				ex = mx + 1;
				ey = my;
				m_qNextMovePath.push_back(XMFLOAT2(ex, ey));
			}
			else if (m_visited[my][mx - 1] == count - 1)
			{
				ex = mx - 1;
				ey = my;
				m_qNextMovePath.push_back(XMFLOAT2(ex, ey));
			}

		}
	};

};
