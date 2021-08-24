
#include "stdafx.h"
#include "ThreadProcessor.h"
#include "Scene.h"
#include "ContentManager.h"
#include "ParticleManager.h"
#include "Warp3DModelManager.h"
#include "CubeTerrainManager.h"
#include "SpacePartitionQuad.h"
#include "GameFramework.h"
#include "GameFramework_2D.h"
#include "ScreenShader.h"

#include "SceneLoading_2D.h"
#include "SceneGameResultLoading_2D.h"


std::shared_ptr<CScreenShader> gpTextureToScreenShader;

UINT WINAPI LoadingThreadLoad(LPVOID lpParam)
{
	std::cout << " Thread Loading" << std::endl;

	ID3D11Device* pd3dDevice = (ID3D11Device*)lpParam;

	//KYT '16.01.20 plus
	/*디폴트 오브젝트 생성*/
	CONTENTMGR->Load(pd3dDevice);
	std::cout << " ContentManager Load" << std::endl;

	//Load
	MODELMGR->Load();
	WORLDMAP->Load(pd3dDevice);
	PARTICLE->Load(pd3dDevice);
	FRAMEWORK->LoadScreenShader(pd3dDevice);
	//BULLETMGR->Load(pd3dDevice);


	//m_pScene = CScene::GetInstance();
	auto scene = CScene::GetInstance();
	if (scene)
	{
		scene->BuildObjects(pd3dDevice);
		scene->LoadStaticFBX(pd3dDevice);
		//scene->LoadDynamicFBX(pd3dDevice);
	}

	//KYT '16.04.29
	/*
	Fog 파라메터 세팅을 HLSL로 넘김
	*/

	CONTENTMGR->FinalLoad(pd3dDevice);
	SPACE->FinalLoad();
	std::cout << " 쓰레드 로딩완료" << std::endl;
	::GTimeRecord(false, "화면 띄어지고나서부터의 시간 : ");


	return 0;
}

void WINAPI PrepareManagerLoading(LPVOID lpParam)
{
	std::cout << " Join " << std::endl;

	if (nullptr != loadingThread) loadingThread->join();
	loadingThread = nullptr;
	::GTimeRecord();

	/// 수정시작
	UIMGR->Load(FRAMEWORK->GetDevice());
	CONTENTMGR->UILoad(FRAMEWORK->GetDevice());

	//Player
	PLAYER->Load(FRAMEWORK->GetDevice());
	PARTICLE->SetCamera(PLAYER->GetCamera());
	PARTICLE->CreateConstBuffer(FRAMEWORK->GetDevice());


	CScene::GetInstance()->LoadDynamicFBX(FRAMEWORK->GetDevice());
	CScene::GetInstance()->SetCamera(PLAYER->GetCamera());
	CScene::GetInstance()->LoadSystem();

	/// 수정 끝
#ifdef _MULIT_THREAD_RENDER_
	FRAMEWORK->InitializeWorkerThreads();
#endif	


}

void WINAPI FinalManagerLoading(LPVOID lpParam)
{
	CONTENTMGR->FinalLoad(FRAMEWORK->GetDevice());
	SPACE->FinalLoad();
	UIMGR->FinalLoad();

	::GTimeRecord(false, "Deffered Loading : ");
	::DimensionsChange(GAME_MODE_INGAME);

	EDITBOX_2D->Clear();

	SOUNDMGR->Stop();
	//SOUNDMGR->Play("배경소리");
	
	SOUNDMGR->Play("스타트");

	//RSH '16.08.22
	/*
		게임플레이 BGM 추가
	*/
	SOUNDMGR->Play("게임플레이");
}

void WINAPI ReleaseManager(LPVOID lpParam)
{
	FRAMEWORK->ReleaseObjects();
}


void DimensionsChange(YT_GameMode mode, float delay)
{
	if (gbGameMode == mode)return; //모드가 같으면 무시함

	gbGameMode = mode;

	//이전 모드가 2D였음
	if (gbGameMode == YT_GameMode::GAME_MODE_INGAME)
	{
		FRAMEWORK_2D->EndDraw();
	}

	else
	{

		//KYT '16.07.31
		/*
		삭제~
		*/

		::ReleaseManager(nullptr);
		ShowCursor(true);
		FRAMEWORK_2D->change_state(new CSceneGameResultLoading_2D("SceneGameResultLoading_2D"));
	}

}

DWORD WINAPI ThreadOnUpdate(LPVOID lpParam)
{
	FRAMEWORK->AnimateObjects();
	return 0;
}

DWORD WINAPI ThreadOnCollision(LPVOID lpParam)
{
	FRAMEWORK->OnCollision();
	return 0;
}
