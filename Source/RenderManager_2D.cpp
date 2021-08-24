
#include "stdafx.h"
#include "RenderManager_2D.h"

Image::Image()
{

}

Image::Image(TCHAR* filePath)
{
	this->LoadCImage(filePath);
}

Image::~Image()
{
	//delete m_image;
}


void Image::Init(TCHAR* filePath)
{
	this->LoadCImage(filePath);
}

void Image::LoadCImage(TCHAR* filePath)
{
	m_image = new CImage();
	HRESULT result = m_image->Load(filePath);
	if (result == S_OK)
	{
		m_Width = m_image->GetWidth();
		m_Height = m_image->GetHeight();
	}
	else
	{
		MessageBox(g_hWnd, filePath, L"��ΰ� �߸��Ǿ����ϴ�.", NULL);
		//GlobalVar::GetInstance()->ErrorMessage(filePath, "��ΰ� �߸��Ǿ����ϴ�");
	}
}

CRenderManager_2D::CRenderManager_2D()
{

}

CRenderManager_2D::~CRenderManager_2D()
{
	this->Release();
}

void CRenderManager_2D::AddImage(TCHAR* filePath, std::string name)
{
	auto pFindImg = this->FindImage(name);

	if (pFindImg != nullptr)return;

	else
	{
		std::vector<Image> vecImage;
		Image img;
		img.LoadCImage(filePath);
		vecImage.push_back(img);
		m_ImageMap.insert(std::make_pair(name, vecImage));

	}

}

std::vector<Image>* CRenderManager_2D::FindImage(std::string name)
{
	std::map<std::string, std::vector<Image>>::iterator iter = m_ImageMap.find(name);

	if (iter == m_ImageMap.end())
		return nullptr;
	else
		return &iter->second;
}

void CRenderManager_2D::LoadCImage()
{

	AddImage(L"../Assets/03_Image/2D_UI/Background/WarpLogo.png", "�ΰ�");
	AddImage(L"../Assets/03_Image/2D_UI/Background/loading_background.png", "�ε� ���ȭ��");

	AddImage(L"../Assets/03_Image/2D_UI/Background/login_background.png", "�α��� ���ȭ��");
	AddImage(L"../Assets/03_Image/2D_UI/Background/lobby_background.png", "�κ� ���ȭ��");
	AddImage(L"../Assets/03_Image/2D_UI/Background/game_room_background.png", "���ӹ� ���ȭ��");
	AddImage(L"../Assets/03_Image/2D_UI/Background/room_create_background.png", "�� ���� ���ȭ��");
	AddImage(L"../Assets/03_Image/2D_UI/Background/result_background.png", "��� ���ȭ��");
	AddImage(L"../Assets/03_Image/2D_UI/Background/match_standby_background.png", "��ġ ��� ���ȭ��");

	AddImage(L"../Assets/03_Image/2D_UI/Background/match_end_loading_background.png", "���ȭ�� �ε� ���ȭ��");

	///////////////////////////////////////////////////////////////////////////////

	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_login.png", "�α��� �����ϱ� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_create_room.png", "�� �����ϱ� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_create_room_confirm.png", "�� �����ϱ� Ȯ�� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_create_room_quit.png", "�� ���� ������ ��ư");

	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_basic_mode.png", "�⺻��� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_occupy_mode.png", "���ɸ�� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_score_mode.png", "������� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_1x1_mode.png", "1x1��� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_2x2_mode.png", "2x2��� ��ư");

	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_basic_mode_select.png", "�⺻��� ���� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_occupy_mode_select.png", "���ɸ�� ���� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_score_mode_select.png", "������� ���� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_1x1_mode_select.png", "1x1��� ���� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_2x2_mode_select.png", "2x2��� ���� ��ư");

	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_go_to_lobby.png", "�κ�� ������ ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_game_ready.png", "�غ� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_game_ready_check.png", "�غ�Ϸ� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_room_quit.png", "�� ������ ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_character1.png", "ĳ����1 ���� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_character2.png", "ĳ����2 ���� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_select_team_line.png", "�� ���� ���� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_game_quit.png", "���� â ���� ��ư");
	AddImage(L"../Assets/03_Image/2D_UI/Button/btn_game_quit2.png", "���� ���� ��ư");

	//////////////////////////////////////////////////////////////////////////////

	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_Ateam_character_1.png", "A�� ĳ����1 ǥ��");
	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_Ateam_character_2.png", "A�� ĳ����2 ǥ��");
	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_Bteam_character_1.png", "B�� ĳ����1 ǥ��");
	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_Bteam_character_2.png", "B�� ĳ����2 ǥ��");

	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_match_standby_Ateam_character_1.png", "��ġ ��� A�� ĳ����1 ǥ��");
	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_match_standby_Ateam_character_2.png", "��ġ ��� A�� ĳ����2 ǥ��");
	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_match_standby_Bteam_character_1.png", "��ġ ��� B�� ĳ����1 ǥ��");
	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_match_standby_Bteam_character_2.png", "��ġ ��� B�� ĳ����2 ǥ��");

	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_result_character_1.png", "��� ȭ�� ĳ����1 ǥ��");
	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_result_character_2.png", "��� ȭ�� ĳ����2 ǥ��");

	AddImage(L"../Assets/03_Image/2D_UI/Display/dpy_loading_anim2.png", "�ε� ǥ��");

	//////////////////////////////////////////////////////////////////////////////
	
	AddImage(L"../Assets/03_Image/2D_UI/RoomWindow/wnd_roomlist.png", "�� ����Ʈ ǥ�� â");
}


void CRenderManager_2D::DrawImageSpriteAlphaBackground(HDC hdc, std::string name, float left, float top, float width, float height, float s_left, float s_top, float s_width, float s_height, COLORREF removeColor, int alpha)
{
	HDC LayDC;
	HBITMAP Lay;
	BLENDFUNCTION bf;

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = alpha;

	Lay   = CreateCompatibleBitmap(hdc, (int)width, (int)height);
	LayDC = CreateCompatibleDC(hdc);
	SelectObject(LayDC, Lay);
	TransparentBlt(LayDC, 0, 0, (int)width, (int)height, hdc, s_left, s_top, s_width, s_left, removeColor);

	std::vector<Image>* image = FindImage(name);

	if (image)
		image->begin()->GetCimage()->TransparentBlt(hdc, left, top, width, height,
		s_left, s_top, s_width, s_height, removeColor);

	AlphaBlend(hdc, (int)left, (int)top, (int)width, (int)height, LayDC, 0, 0, (int)width, (int)height, bf);

	//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);

	DeleteDC(LayDC);
	DeleteObject(Lay);
}

void CRenderManager_2D::Render(HDC hdc, std::string name, float left, float top, float width, float height, float s_left, float s_top, float s_width, float s_height, COLORREF removeColor)
{
	std::vector<Image>* image = FindImage(name);

	if (image)
		image->begin()->GetCimage()->
		TransparentBlt(hdc, (int)left, (int)top, (int)width, (int)height,(int)s_left, (int)s_top, (int)s_width, (int)s_height, removeColor);
}

Image* CRenderManager_2D::GetImage(std::string name)
{
	return FindImage(name)->begin()->GetImage();
}

void CRenderManager_2D::Release()
{
	for (auto &map : m_ImageMap)
	{
		for (auto &image : map.second)
		{
			image.Release();
		}
	}
	m_ImageMap.clear();
}


