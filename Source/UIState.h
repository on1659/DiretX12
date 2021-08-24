#pragma once

#include "stdafx.h"
#include "UIShader.h"

class UIState : public Component
{
public:
	UIState(std::string name = "")
	{
		UIState::name = "UIState";
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~UIState()
	{

	}

	//공통으로가지고 있어야 할 값들
	protected:
		CUIShader*				uiShadr;
		RECT					itemPos[4];
		float					fSpeed;
		int						nUseItem;

	private:
		bool					bMove;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);

		uiShadr = static_cast<CUIShader*>(m_gameObject->GetShader().get());
		fSpeed = 15.0f;
		bMove = false;
		nUseItem = -1;
		for (int i = 0; i < 4; ++i)
		{
			itemPos[i].left	  = 142 + (i * 44);
			itemPos[i].right  = itemPos[i].left + 30;
			itemPos[i].top	  = 532;
			itemPos[i].bottom = itemPos[i].top + 30;
		}

	}


	bool GetMoveState() const { return bMove; }

	void MoveEnd() { bMove = false; }

	void EatItem(){	bMove = true; }

	void SetUseItenNumber(int n) { nUseItem = n; }

	int GetUseItemNumber() const { return nUseItem; }

};
