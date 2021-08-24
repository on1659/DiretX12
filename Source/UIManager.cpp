#include "stdafx.h"
#include "UIManager.h"
#include "UIShader.h"
#include "UIMesh.h"

#include "UIEatItem.h"

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::ReleseInstance()
{
	for (auto &ui : m_mUIShader) ui.second->ReleseInstance();
	for (auto &ui : m_mVisibleShader) ui.second->ReleseInstance();
	for (auto &ui : m_mBlendingShader) ui.second->ReleseInstance();
	m_pd3dDevice = nullptr;
}

void CUIManager::Load(ID3D11Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	int mTapUI_RightPush = 0;

	m_mUIShader.clear();
	m_mVisibleShader.clear();
	m_mBlendingShader.clear();



	AddShader(true, UIShaderStruct("3d_dpy_target_point", "../Assets/03_Image/3D_UI/3d_dpy_target_point.png", FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 - 20, 60, 60));
	AddShader(true, UIShaderStruct("3d_dpy_Bteam_occupy", "../Assets/03_Image/3D_UI/3d_dpy_Bteam_occupy.png", FRAME_BUFFER_WIDTH / 2 + 350, FRAME_BUFFER_HEIGHT / 2 + 50, 45, 45));
	AddShader(true, UIShaderStruct("3d_dpy_map", "../Assets/03_Image/3D_UI/3d_dpy_map.png", FRAME_BUFFER_WIDTH / 2 + 450, FRAME_BUFFER_HEIGHT / 2 + 165, 252, 160
		, true), "UI_Translucence");
	AddShader(true, UIShaderStruct("3d_dpy_map_view", "../Assets/03_Image/3D_UI/3d_dpy_map_view.png", FRAME_BUFFER_WIDTH / 2 + 450, FRAME_BUFFER_HEIGHT / 2 + 150, 281, 216
		, true), "UI_Translucence");

	//JJY '16.08.19
	/*
		경기 시간 및 리스폰 시간 UI
	*/
	AddShader(true, UIShaderStruct("3d_dpy_match_time_view", "../Assets/03_Image/3D_UI/3d_dpy_match_time_view.png", FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 - 300, 300, 100));

	//AddShader(true, UIShaderStruct("3d_dpy_respawn_time_view", "../Assets/03_Image/3D_UI/3d_dpy_respawn_time_view.png", FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2, 300, 100));


	//KYT '15.05.28
	/*
		UI Shader 옮겨옴 확인 필요
	*/
#ifdef _CONNECT_SERVER_

	{
		std::string path;

		for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
		{
			UINT character_type = TCP->GetRoomPlayerInfo(idx).iSelectedCharacter;
			UINT selected_team = TCP->GetRoomPlayerInfo(idx).iSelectedTeam;

			if (character_type == CHARACTER_MEI)
			{
				if (selected_team == A_TEAM_0 || selected_team == A_TEAM_1)
				{
					path = "../Assets/03_Image/3D_UI/3d_dpy_Ateam_character_1.png";
				}
				else if (selected_team == B_TEAM_0 || selected_team == B_TEAM_1)
				{
					path = "../Assets/03_Image/3D_UI/3d_dpy_Bteam_character_1.png";
				}
			}

			else if (character_type == CHARACTER_LIZZY)
			{
				if (selected_team == A_TEAM_0 || selected_team == A_TEAM_1)
				{
					path = "../Assets/03_Image/3D_UI/3d_dpy_Ateam_character_2.png";
				}
				else if (selected_team == B_TEAM_0 || selected_team == B_TEAM_1)
				{
					path = "../Assets/03_Image/3D_UI/3d_dpy_Bteam_character_2.png";
				}
			}

			// -------------------------- icon ------------------------------------------/
			if (selected_team == A_TEAM_0 || selected_team == A_TEAM_1)
			{
				AddShader(true, UIShaderStruct("RedPlayer", path, 100 + (100 * idx), FRAME_BUFFER_HEIGHT / 2 - 300, 80, 80));

				AddShader
				(
					false,
					UIShaderStruct
					(
						"0BulletinPlayer_TabIcon" + to_string(selected_team)
						, path
						, 300 + TapUI_RIGHTPUSH//초기 x값 250
						, 142 + (selected_team * 95) + TapUI_DOWNPUSH
						, 55
						, 55
						, false
					)
				);

			}
			else if (selected_team == B_TEAM_0 || selected_team == B_TEAM_1)
			{
				AddShader(true, UIShaderStruct("BluePlayer", path, 900 + (100 * idx), FRAME_BUFFER_HEIGHT / 2 - 300, 80, 80));

				AddShader
				(
					false,
					UIShaderStruct
					(
						"0BulletinPlayer_TabIcon" + to_string(selected_team)
						, path
						, 300 + TapUI_RIGHTPUSH //초기 x값 250
						, 342 + ((selected_team - 2) * 95) + TapUI_DOWNPUSH
						, 55
						, 55
						, false
					)
				);	//AddShader

			}	// Team A

		}//for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
	}

	//JJY '16.05.01
	/*
	내 모습에 따라 UI를 만듭니다.
	*/

	{
		std::string my_character_ui_path;
		int height;
		my_character_ui_path = "../Assets/03_Image/3D_UI/3d_dpy_my_character_A1.png";

		UINT my_character_type = TCP->my_info.iSelectedCharacter;
		UINT my_selected_team = TCP->my_info.iSelectedTeam;

		if (my_character_type == CHARACTER_MEI)
		{
			if (my_selected_team == A_TEAM_0 || my_selected_team == A_TEAM_1)
				my_character_ui_path = "../Assets/03_Image/3D_UI/3d_dpy_my_character_A1.png";
			else if (my_selected_team == B_TEAM_0 || my_selected_team == B_TEAM_1)
				my_character_ui_path = "../Assets/03_Image/3D_UI/3d_dpy_my_character_B1.png";
		}
		else if (my_character_type == CHARACTER_LIZZY)
		{
			if (my_selected_team == A_TEAM_0 || my_selected_team == A_TEAM_1)
				my_character_ui_path = "../Assets/03_Image/3D_UI/3d_dpy_my_character_A2.png";
			else if (my_selected_team == B_TEAM_0 || my_selected_team == B_TEAM_1)
				my_character_ui_path = "../Assets/03_Image/3D_UI/3d_dpy_my_character_B2.png";
		}

		AddShader(true, UIShaderStruct("PlayerUI", my_character_ui_path, FRAME_BUFFER_WIDTH / 2 - 450, FRAME_BUFFER_HEIGHT / 2 + 180, 313, 126));

		if (my_selected_team == A_TEAM_0 || my_selected_team == A_TEAM_1)
		{
			height = 165;
		}
		else
		{
			height = 370;
		}

		AddShader
		(
			false,
			UIShaderStruct
			(
				"0BulletinPlayerIcon" + to_string(my_selected_team)
				, my_character_ui_path
				, 250
				, height + (my_selected_team * 110) + 5
				, 75
				, 75
				, false
			)
		);	//AddShader


		//auto pUIShdaer = CONTENTMGR->MakeUIShaderFactoty(pd3dDevice);
		//auto pMesh = std::make_unique<CUIMesh>(pd3dDevice, 313, 126);
		//pUIShdaer->SetMesh(move(pMesh));
		//pUIShdaer->CreateBlendState(pd3dDevice, d3dBlendStateDesc);
		//pUIShdaer->SetMaterial(pd3dDevice, my_character_ui_path);
		//pUIShdaer->SetPosition(FRAME_BUFFER_WIDTH / 2 - 450, FRAME_BUFFER_HEIGHT / 2 + 180, 313, 126);
	};

#else

	//Add Shader
	AddShader(true, UIShaderStruct("3d_dpy_my_character_A1", "../Assets/03_Image/3D_UI/3d_dpy_my_character_A1.png", FRAME_BUFFER_WIDTH / 2 - 450, FRAME_BUFFER_HEIGHT / 2 + 180, 313, 126));


	AddShader(false, UIShaderStruct("Alpha", "../Assets/03_Image/Alpha/alpha.png", FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 + 100, 200, 100,
		true), "UI_Translucence");


#endif






	//Iterm
	/*
	Item은 Render 우선순위를 하기 위해서 앞에 0을 붙인다.
	*/

	int itemSize = 30;
	for (int i = 0; i < 4; ++i)
	{
		AddItemShader
		(
			false
			, UIShaderStruct("Apple", "../Assets/03_Image/3D_UI/3d_dpy_item_Apple.2d.png", FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.5f, itemSize, itemSize)
			, i
			, std::make_unique<UIEatItem>()
		);

		AddItemShader
		(
			false
			, UIShaderStruct("Bugger", "../Assets/03_Image/3D_UI/3d_dpy_item_BugerJelly.2d.png", FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.5f, itemSize, itemSize)
			, i
			, std::make_unique<UIEatItem>()
		);

		AddItemShader
		(
			false
			, UIShaderStruct("Doughunt", "../Assets/03_Image/3D_UI/3d_dpy_item_Doughnut.2d.png", FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.5f, itemSize, itemSize)
			, i
			, std::make_unique<UIEatItem>()
		);

		AddItemShader
		(
			false
			, UIShaderStruct("Cherry", "../Assets/03_Image/3D_UI/3d_dpy_item_Cherry.2d.png", FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.5f, itemSize, itemSize)
			, i
			, std::make_unique<UIEatItem>()
		);

	}

	for (auto &pShader : m_mUIShader) pShader.second->Start();
	for (auto &pShader : m_mVisibleShader) pShader.second->Start();
}

void CUIManager::FinalLoad()
{
	auto v = HASHTABLE->GetPlayerPaths();
	int i = 0;
	int height = 0;

	//-------------------- Player Icon Loading --------------------

	//for (auto path : v)
	//{
	//	if (path.size() == 0) continue;
	//
	//	std::string name = to_string(i);
	//	name.append(path);
	//
	//	if (path.find(".png") > path.size())
	//		path.append(".png");
	//
	//	if (i == 2) height = 5;
	//
	//	AddShader
	//	(
	//		false,
	//		UIShaderStruct
	//		(
	//			"0BulletinPlayerIcon" + to_string(i)
	//			, "../Assets/03_Image/3D_UI/" + path
	//			, 250
	//			, 165 + (i * 110) + height
	//			, 75
	//			, 75
	//			, false
	//		)
	//	);
	//	i++;
	//}
	//-------------------- Player Icon Loading --------------------

	//--------------------------------------------------

	AddShader(
		false,
		UIShaderStruct
		(
			"0BulletinView_TabPanel"
			, "../Assets/03_Image/3D_UI/panel.png"
			, FRAME_BUFFER_WIDTH * 0.5f 
			, FRAME_BUFFER_HEIGHT * 0.4f 
			, 1000
			, 530
			, true
		)
	);

	AddShader(
		false,
		UIShaderStruct
		(
			"0BulletinView_TabPanel1"
			, "../Assets/03_Image/3D_UI/3d_dpy_bulletin_board.png"//"../Assets/03_Image/3D_UI/panel-02.png"
			, FRAME_BUFFER_WIDTH * 0.5f
			, FRAME_BUFFER_HEIGHT * 0.4f
			, 1000
			, 530
			, false
		)
	);

}

void CUIManager::AddShader(bool active, UIShaderStruct ui, std::string strBlendDesc)
{
	auto pUIShader = std::make_shared<CUIShader>();
	pUIShader->CreateShader(m_pd3dDevice);
	auto pMesh = std::make_unique<CUIMesh>(m_pd3dDevice, ui.w, ui.h);
	pUIShader->SetMesh(move(pMesh));
	pUIShader->SetUIMaterial(m_pd3dDevice, ui.path);

	pUIShader->SetPosition(ui.left, ui.top);
	pUIShader->SetUIShader(pUIShader);

	pUIShader->BuildObjects(m_pd3dDevice);
	pUIShader->name = std::to_string(ui.blend);
	pUIShader->name.append(ui.name);

	pUIShader->SetBlendState(strBlendDesc);

	if (active)
	{
		if (ui.blend)
			m_mBlendingShader.insert(make_pair(ui.name, pUIShader));

		else
			m_mVisibleShader.insert(make_pair(ui.name, pUIShader));
	}
	else
		m_mUIShader.insert(make_pair(ui.name, pUIShader));
}

bool CUIManager::SetComponet(std::string name, std::unique_ptr<Component> componet)
{
	auto shader = m_mUIShader.find(name);
	if (shader == m_mUIShader.end()) return false;
	shader->second->SetComponet(move(componet));
	return true;
}

void CUIManager::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	//KYT '16.10.01
	/*
		영상용
	*/
	CUIManager::RenderTransparent(pd3dDeviceContext);

	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, "UINoBlend");
	for (auto &pShader : m_mVisibleShader) pShader.second->Render(pd3dDeviceContext);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void CUIManager::RenderTransparent(ID3D11DeviceContext * pd3dDeviceContext)
{
	//KYT '16.07.29
	/*
		수정을위함
	*/

	for (auto &pShader : m_mBlendingShader)
	{
		pShader.second->RenderTransparent(pd3dDeviceContext);
	}
}

void CUIManager::Animate(float fTimeElapsed)
{
	for (auto &pShader : m_mVisibleShader)
		pShader.second->Animate(fTimeElapsed);
}

CUIShader* CUIManager::UseShader(std::string uiName)
{
	std::string str = uiName;

	auto iter = m_mUIShader.find(str);

	if (iter == m_mUIShader.end())
		return nullptr;

	if (iter->second->name[0] == '1')
		m_mBlendingShader.insert(std::make_pair(iter->first, iter->second));

	if (iter->second->name[0] == '0')
		m_mVisibleShader.insert(std::make_pair(iter->first, iter->second));

	CUIShader* re = iter->second.get();
	m_mUIShader.erase(iter);

	return re;
}

void CUIManager::EndUseShader(std::string uiName)
{
	bool isBlend = false;

	std::string str = uiName;

	std::multimap<std::string, std::shared_ptr<CUIShader>>::iterator iter = m_mVisibleShader.find(str);

	if (iter == m_mVisibleShader.end())
	{
		iter = m_mBlendingShader.find(str);
		if (iter == m_mBlendingShader.end())
			return;
		isBlend = true;
	}

	m_mUIShader.insert(std::make_pair(iter->first, iter->second));


	if (isBlend)
		m_mBlendingShader.erase(str);
	else
		m_mVisibleShader.erase(iter);

}

CUIShader* CUIManager::UseItemUiShader(int slot, std::string uiName)
{
	std::string str = "0";
	str += (slot + 48);
	str.append(uiName);

	auto iter = m_mUIShader.find(str);

	if (iter == m_mUIShader.end())
		return nullptr;

	if (iter->second->name[0] == '1')
		m_mBlendingShader.insert(std::make_pair(iter->first, iter->second));

	if (iter->second->name[0] == '0')
		m_mVisibleShader.insert(std::make_pair(iter->first, iter->second));

	CUIShader* re = iter->second.get();
	m_mUIShader.erase(iter);

	return re;
}

void CUIManager::EndUseItemUiShader(int slot, std::string uiName)
{
	bool isBlend = false;

	std::string str = "0";
	str += (slot + 48);
	str.append(uiName);

	std::multimap<std::string, std::shared_ptr<CUIShader>>::iterator iter = m_mVisibleShader.find(str);

	if (iter == m_mVisibleShader.end())
	{
		iter = m_mBlendingShader.find(str);

		if (iter == m_mBlendingShader.end())
			return;
	}
	iter->second->SetPosition(FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.5f, 30.0f, 30.0f);

	m_mUIShader.insert(std::make_pair(iter->first, iter->second));

	m_mBlendingShader.erase(str);
	m_mVisibleShader.erase(iter);

}