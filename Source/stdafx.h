	// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

//KYT '16.01.20 plus
/* 코드스타일  규칙*/
/*
 1. 멤버변수는 m을 붙인다.
 2. m_ 오는 첫 글자는 변수의 타입이다.
 3. #define 은 무조건 대문자이다.
 4. #ifdef 는 앞에 _ + 대문자 이다.
 5. 부등호는 왠만하면 왼쪽이 크다. 하지만 범위 부등호일때는 오른쪽으로 한다.
   ex) a > b;   ( a < i && i < b ) 목적은 가독성이다.
 
*/

#pragma once



#include "targetver.h"

//JJY 16'.04.16 WIN32_LEAN_AND_MEAN을 #define함
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:

//JJY 16'.04.16 winsock2과 겹쳐서 windows.h 주석
//#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <queue>



#define DX12_MIGRATION
#ifdef DX12_MIGRATION
// D3D12
#include <d3d12.h>
#include <dxgi1_4.h>
//#include <Client.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "../Assets/import/DirectX/DX12/d3dx12.h"
#include "../Assets/import/DirectX/DX12/DDSTextureLoader12.h"
#endif //DX12_MIGRATION
#include <d3d11.h>
#include <d3dx11.h>
#include <D3DX10Math.h>
#include <d3dcompiler.h>

//#include <D3D9Types.h>

#include <Mmsystem.h>

#include "../Assets/import/AntTweakBar/include/AntTweakBar.h"

//#ifdef _DEBUG
#ifdef UNICODE
	#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else 
	#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

//전역 HWND
extern HWND g_hWnd;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

//#include <xnamath.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

#include <vector>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <stack>
#include <algorithm>
#include <memory>
#include <process.h>
#include <ctime>
#include <random>
#include <chrono>
#include <atlimage.h>
#include <unordered_map>

/* User include */
#include "Define.h"
#include "MyStruct.h"
#include "MyEnum.h"


//RSH '16.05.01
/*
	XBOX 컨트롤러 조작 추가를 위한 라이브러리 정의
*/
#include <XInput.h>
#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "dxgi.lib")

//JJY 16'.04.16 plus
/*
	네트워크 include 추가
*/
//NetWork
#pragma comment(lib, "ws2_32")
#include <winsock2.h>

/*FBXSDK*/
#include <fbxsdk.h>


#include "Object.h"
#include "SingleTon.h"

#ifdef _QURD_ARRARY
	#include "SpacePartitionQuad.h"
#else
	#include "SpacePartition.h"
#endif

//3D
#include "SceneState.h"
#include "ShaderFactory.h"

#include "InputManager.h"
#include "ContentManager.h"
#include "PlayerManager.h"
#include "Helper.h"
#include "LightingManger.h"
#include "CameraManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "ParticleManager.h"
#include "TwBarManager.h"
#include "Globalvariable.h"
//#include "../Assets/import/AntTweakBar/include/AntTweakBar.h"

//#include "GraphicObject.h"
//#include "SpriteObject.h"
#include "UIManager.h"
#include "HashTable.h"
#include "ThreadProcessor.h"

//2D
#include "GameState_2D.h"
#include "Draw_2D.h"
#include "GraphicObject_2D.h"
#include "SpriteObject_2D.h"
#include "RenderManager_2D.h"
#include "MyFunction.h"
#include "GameObject_2D.h"
#include "Lusian_Scrool.h"
#include "EditBox_2D.h"
#include "MathCalu.h"


//JJY 16'.04.16
//Network
#include "TCP_Client.h"


//KYT '16.04.08

/*
 gInstance, hWnd 선언
*/
static HINSTANCE ghInstance;

#include <thread>
extern std::thread* loadingThread;

//오류메세지 출력
extern int GMessageBox(std::string title, std::string message = " ");

extern int GMessageBox(TCHAR* title, TCHAR* message = L" ");

extern int RandomRange(int start, int end);

extern void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);

extern TCHAR* StringToTCHAR(std::string& s);

extern TCHAR* tChar(LPCSTR pszShaderName);

extern void gotoxy(int x, int y);

inline void gotoxy(int x, int y, const char *msg, ...)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	va_list arg;
	va_start(arg, msg);
	vprintf(msg, arg);
	va_end(arg);
	printf("\n");
}

extern bool gbGameMode;


//Inline Function
inline D3DXVECTOR3 XMLoadD3DXVECTOR3(const XMVECTOR& xmVec)
{
	XMFLOAT3 xmf3;
	XMStoreFloat3(&xmf3, xmVec);
	return std::move(D3DXVECTOR3(xmf3.x, xmf3.y, xmf3.z));
}

inline XMFLOAT3 XMVFLOAT3Converter(const XMVECTOR& xmVec)
{
	XMFLOAT3 xmf3;
	XMStoreFloat3(&xmf3, xmVec);
	return xmf3;
}

inline XMVECTOR XMVECTORConverter(const XMFLOAT3& d3dxv, float wParam = 0.f)
{
	return (XMVectorSet(d3dxv.x, d3dxv.y, d3dxv.z, 0.0f));
}

inline XMVECTOR D3DXVec3ToXMVec(const D3DXVECTOR3& d3dxv, float wParam = 0.f)
{
	return (XMVectorSet(d3dxv.x, d3dxv.y, d3dxv.z, 0.0f));
}

inline D3DXVECTOR3 D3DXLoadFLOAT3(const XMFLOAT3 xmf)
{
	return D3DXVECTOR3(xmf.x, xmf.y, xmf.z);
}
	
inline XMFLOAT3 D3DXStoreFLOAT3(const D3DXVECTOR3 xmf)
{
	return XMFLOAT3(xmf.x, xmf.y, xmf.z);
}

inline BoundingBox GetBoudingBox(const D3DXVECTOR3 &fMin, const D3DXVECTOR3 &fMax)
{
	BoundingBox boundingbox;
	boundingbox.Center = { (fMin.x + fMax.x) * 0.5f, (fMin.y + fMax.y) * 0.5f, (fMin.z + fMax.z) * 0.5f };
	boundingbox.Extents = { (fMin.x - fMax.x) * 0.5f, (fMin.y - fMax.y) * 0.5f, (fMin.z - fMax.z) * 0.5f };
	return boundingbox;
}

//RSH '16.08.17
/*
충돌을 정밀하게 만들기 위해 두 캐릭터 벡터의 사이각을 구하는 함수 추가
*/
inline float GetBetweenAngle(const XMVECTOR& a, const XMVECTOR& b)
{
	XMVECTOR angleVec = XMVector3AngleBetweenVectors(a, b);
	float angleRadians = XMVectorGetX(angleVec);
	float angleDegree = XMConvertToDegrees(angleRadians);

	return angleDegree;
}

static std::chrono::high_resolution_clock::time_point cStart;

extern void GTimeRecord(bool start = true, std::string str = "");

extern void ConsoleShow(XMFLOAT3 x3, std::string msg ="");

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#define V_RETURN(X) X

//KYT '16.04.28
/*
	Blur Mode
	x = Mode의 변화가 있을 시 감지
	y = Run Bulr
	z = 미정
	w = 미정

	ex) Run 상태이면 (1, 5, ? ? )
	    -> Map Umap하는 부분에서 x를 다시 0으로 변경함
	
*/
extern XMFLOAT4 gBulrMode;
extern void GBulrChange(YT_BlurMode state);

static int icode = 0;
static float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

//KYT '16.07.26
/*
	GameObject Global ID;
*/
static int gInstanceID = 0;
static int gFakeItemInstancedID = 40000;
static int gTrapItemInstancedID = 41000;

static bool gOnlyOneMoveCheck = true;
