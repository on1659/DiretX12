#include "stdafx.h"
#include "OtherPlayer.h"
#include "OtherPlayerComponent.h"
#include "OtherPlayerAnimation.h"
/*
	//기존
	CPlayer		  = 나
	CGameObject  = otherPlayers

	//개편
					CPlayer
	CTerrainPlayer			COtherPlayer
		나						상대

		->바꾼이유 
		->OtherPlayer가 CGameObject라서 자꾸 Object뭘 추가해야함
		->따라서 CPlayer로 특수한 object란걸 알려주고
		  세세한 기능은 상속에 맡긴다.

*/

COtherPlayer::COtherPlayer(int nObjects) : CPlayer(nObjects)
{
	SetCompoent(std::make_unique<OtherPlayerComponent>());
	SetCompoent(std::make_unique<OtherPlayerAnimation>());

}

COtherPlayer::~COtherPlayer()
{
}

void COtherPlayer::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{

}