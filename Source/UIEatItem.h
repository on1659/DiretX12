#pragma once

#include "stdafx.h"
#include "UIState.h"

class UIEatItem : public UIState
{
public:
	UIEatItem(std::string name = "")
	{
		UIEatItem::name = "UIGetItem";
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~UIEatItem() {}

	//변수를 선언하시오
private:
	UIState*		cUIState;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		UIState::Start(gameObject);
		cUIState = uiShadr->GetUIStateComponet();
	}

	virtual void Animate(float fTimeElapsed)
	{
		//printf("[useItem : %d]\n", cUIState->GetUseItemNumber());

		if (cUIState->GetMoveState())
		{
			UIEatItem::Move();
		}
	}

	void Move()
	{
		RECT uiPos = uiShadr->GetPosition();

		XMFLOAT2 f2Pos{ (float)uiPos.left ,		(float)uiPos.top };
		XMFLOAT2 itPos{ (float)itemPos[cUIState->GetUseItemNumber()].left, (float)itemPos[cUIState->GetUseItemNumber()].top };
		XMVECTOR distance = XMVectorSubtract(XMLoadFloat2(&itPos), XMLoadFloat2(&f2Pos));
		XMVECTOR xmLength = XMVector2Length(distance);

		XMFLOAT2 f2Length;
		XMStoreFloat2(&f2Length, xmLength);

		if (f2Length.x < fSpeed * 0.5)
		{
			uiShadr->SetPosition(itemPos[cUIState->GetUseItemNumber()]);
			cUIState->MoveEnd();
			return;
		}

		MathCalu::ADD_RECTnPOINT(&uiPos, XMVector2Normalize(distance), fSpeed);
		uiShadr->SetPosition(uiPos);
	}


};


	