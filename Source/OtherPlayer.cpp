#include "stdafx.h"
#include "OtherPlayer.h"
#include "OtherPlayerComponent.h"
#include "OtherPlayerAnimation.h"
/*
	//����
	CPlayer		  = ��
	CGameObject  = otherPlayers

	//����
					CPlayer
	CTerrainPlayer			COtherPlayer
		��						���

		->�ٲ����� 
		->OtherPlayer�� CGameObject�� �ڲ� Object�� �߰��ؾ���
		->���� CPlayer�� Ư���� object���� �˷��ְ�
		  ������ ����� ��ӿ� �ñ��.

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