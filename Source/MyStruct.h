
#pragma once

#include "stdafx.h"

class CSpaceNodeQuad;

struct LIGHT
{
	XMFLOAT4				m_d3dxcAmbient;
	XMFLOAT4				m_d3dxcDiffuse;
	XMFLOAT4				m_d3dxcSpecular;
	XMFLOAT3				m_d3dxvPosition;
	float					m_fRange;
	XMFLOAT3				m_d3dxvDirection;
	float					m_nType;
	XMFLOAT3				m_d3dxvAttenuation;
	float 					m_fFalloff;
	float 					m_fTheta; //cos(m_fTheta)
	float					m_fPhi; //cos(m_fPhi)
	float					m_bEnable;
	float					padding;
};

struct LIGHTS
{
	LIGHT					m_pStaticLights[MAX_STATIC_LIGHTS];
	XMFLOAT4				m_d3dxcGlobalAmbient;
	//XMFLOAT4				m_d3dxvCameraPosition;
};

struct LIGHTS_Dynamic
{
	LIGHT		m_pDynamicLights[MAX_DYNAMIC_LIGHTS];
	XMVECTOR	m_d3dxvCameraPosition;
};

struct FOG
{
	XMFLOAT4	gcFogColor;
	XMFLOAT4	gvFogParameter;
};

class WorldMatrix
{
public:
	XMMATRIX		matrix;
	CSpaceNodeQuad* space;
	bool			life;

	WorldMatrix() { life = true; }

	~WorldMatrix() { matrix = XMMatrixIdentity(); space = nullptr; }

	void SetSpaceNodeQuad(CSpaceNodeQuad* _space) { space = _space; }

	CSpaceNodeQuad*	GetSpaceNodeQuad() { return space; }

	XMFLOAT3 GetPositionXMFLOAT3()
	{
		XMFLOAT4X4 m;
		XMStoreFloat4x4(&m, matrix);
		return XMFLOAT3(m._41, m._42, m._43);
	}

	void Release()
	{
		if (space) delete space;
		space = nullptr;
	}
};

//LSH '16.04.20 
/*
	스키닝 애니메이션에 뼈대 변환행렬(상수버퍼)
	인스턴싱용 애니메이션에 사용하는 것이다.
*/
struct GlobalMatrixBoneTransforms
{
	XMFLOAT4X4 gmtxBoneTransforms[96];
};

enum AnimState
{
	ANI_IDLE,
	ANI_RUN,
	ANI_FALLDOWN,
	ANI_LEFT_STRAFE,
	ANI_RIGHT_STRAFE,
	ANI_LEFT_FOWARD_DIAGONAL,
	ANI_RIGHT_FOWARD_DIAGONAL,
	ANI_RUN_BACKWARD,
	ANI_SMASH,
	ANI_STARTFALL
};

//LSH '16.04.11
/*
	20160411 애니메이션 관리 오브젝트용
*/
struct AnimationClip
{
	//std::string m_name;
	AnimState m_state;
	int m_startKeyframe;
	int m_endKeyframe;
	bool m_isLoop;
};

//KYT '16.07.15
/*
	추가해씀
*/
enum ObjectType
{
	DynamicObject,
	StaticObject,
	WorldMatrixObject,
	PlayerSmashBoundingBox,
	PlayerBackBoundingBox,
	BulletType,
};

enum YT_KEY
{
	YK_LEFT = 0,
	YK_RIGHT,
	YK_UP,
	YK_DOWN,
	YK_RBUTTON,
	YK_LBUTTON,
	YK_LSHIFT,
	YK_SPACE,
	YK_F1,
	YK_F2,
	YK_F3,
	YK_F4,
	YK_F5,
	YK_W,
	YK_A,
	YK_S,
	YK_D,
	YK_Z,
	YK_X,
	YK_C,
	YK_I,
	YK_J,
	YK_K,
	YK_L,
	YK_U,
	YK_O,
	YK_P,
	YK_T,
	YK_G,
	YK_0,
	YK_1,
	YK_2,
	YK_3,
	YK_4,
	YK_5,
	YK_6,
	YK_7,
	YK_8,
	YK_9,
	YK_M,
	YK_H,
	YK_TAB,
	YK_F6,	//서버와의 혼선을 주지 않게 하려고
	YK_F7,
	YK_F8,
	YK_Y,
	YK_F11,
	YK_CTRL,
	YK_END
};

enum YT_MouseWheel
{
	WHEEL_UP = 1,
	WHEEL_DOWN = -1,
	WHEEL_NON = 0,
};

enum VK_KEY
{
	VK_0 = 48,
	VK_1 = 49,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,
	VK_A = 0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z
};

enum YT_SAMPLER
{
	SAMPLESTATE_DEFAULT,
	SAMPLESTATE_PARTICLE_BASE,
	SAMPLESTATE_PARTICLE_DETAIL,
};

enum YT_GameMode
{
	GAME_MODE_DIALOG = 0,
	GAME_MODE_INGAME = 1
};

enum YT_ColorNoUse
{
	RED = 0xFF0000FF,			//0
	BLUE = 0xFF00FF00,			//1
	GREEN = 0xFFFF0000,			//2	
	BLACK = 0xFFFFFFFF,			//3
	ORANGE = 0xF0287DFF,			//4
	CYAN = 0xFFFFFF00,			//5
	MAGENTA = 0xFFFF00FF,			//6
	YELLOW = 0xFF00FFFF,			//7


	BRIGHT_RED_1 = 0xFF2D14FF,	//238,20,45
	BRIGHT_RED_2 = 0xFFA534E5,	//229 52 165
	BRIGHT_RED_3 = 0xFF320FE7,	//231 15 62
	BRIGHT_RED_4 = 0xFFDA7AF6,	//246 122 218
	BRIGHT_RED_5 = 0xFF2105EE,	//238 5 33
	BRIGHT_RED_6 = 0xFF3C3AF3,	//243 58 60

	BRIGHT_YELLOW_1 = 0xFF51F9EB,	// 235 249,81
	BRIGHT_YELLOW_2 = 0xFF6DE5F4,	//244,229,109
	BRIGHT_YELLOW_3 = 0xFF1CD5F4,	//244 213 28
	BRIGHT_YELLOW_4 = 0xFF4CDAFD,	//253 218 76
	BRIGHT_YELLOW_5 = 0xFF5CEAEE,	//228,234,92
	BRIGHT_YELLOW_6 = 0xFF6AE4EA,	//234,228,106

	BRIGHT_ORANGE_1 = 0xFF1284F1,		//241 132 18
	BRIGHT_ORANGE_2 = 0xFF5381F4,		//244 129 83

	BRIGHT_GREEN_1 = 0xFF79E073,		//115 225 121
	BRIGHT_GREEN_2 = 0xFF52FA66,		// 102 250 82
	BRIGHT_GREEN_3 = 0xFF67F4B1,		//177,244,83
	BRIGHT_GREEN_4 = 0xFF99E275,		//117 226 153
	BRIGHT_GREEN_5 = 0xFF81E207,		//7, 226, 129
	BRIGHT_GREEN_6 = 0xFF78E26F,		//111,226,120
	BRIGHT_GREEN_7 = 0xFF6CFE5A,		//90,254,108
	BRIGHT_GREEN_8 = 0xFF76FFA3,		//163 255 118

	BRIGHT_BLUE_1 = 0xFFD3C921,	//33 201 216
	BRIGHT_BLUE_2 = 0xFFD15E4D,	//77,94,209
	BRIGHT_BLUE_4 = 0xFFB1D854,		//84 216 177
	BRIGHT_BLUE_3 = 0xFFC2A81E,	//30,168,194

	CORAL = 0xFF8080FF,

	//보라색이문제가있네0x00000000

	WHITE = 0xFFFFFF,			//10
	BRUSH,			//8
	BRUSH2,			//9
	BRIGHT_BLACK,
	CHANGE,
	RANDOM,			//11
	RANDOBJECT_COLOR,

};

enum SH_FBXTYPE
{
	FBX_MODEL = 0,
	FBX_VOXEL = 1,
};

enum TAG_TYPE
{
	TAG_NOTHING,
	TAG_ITEM,
	TAG_FAKE_ITEM,
	TAG_EAT_FAKE_ITEM,
	TAG_MonsterPatrol,
	TAG_RotatingScript,
	TAG_PlayerComponet,
	TAG_SKILL,
	TAG_ITEM_USE,
	TAG_BULLET,
	TAG_BULLET_FLASH,
	TAG_BUNNY_MONSTER,
	TAG_TRAP,
	TAG_END
};

enum YT_BlurMode
{
	BLUR_NOTHING = 0,
	BLUR_CHANGE  = 1,
	BLUR_RUN	 = 5,
	BLUR_FLASH	 = 6,
	BLUR_RESPAWN = 7,
};

//KYT '16.05.12
/*
	ItemType;
*/
enum ITEM_TYPE
{
	ITEM_TYPE_NOTHING = 0,
	ITEM_TYPE_SPEED_UP = 1,
	ITEM_TYPE_HIDE,
	ITEM_TYPE_THROW,
	ITEM_TYPE_TRAP,
	ITEM_TYPE_FAKE,
	ITEM_TYPE_END,
};

//KYT '16.06.26
enum GAME_PLAY_STATE
{
	GAME_PLAY_STOP,
	GAME_PLAY_PLAYING,
	GAME_PLAY_PAUSE,
};


//KYT '16.07.23
struct Bulletin_INFO
{
	std::string		strTexutreName;
	int				nId;
	int				nKill;
	int				nDead;
};

//KYT '16.08.07
/*
몬스터 Materail 변경
*/
enum MonsterTeam
{
	  MonsterTeam_A_Team = 0
	, MonsterTeam_B_Team
};

enum TRAP_ITEM_STATE
{
	TRAP_NON,
	TRAP_Setup,
	TRAP_Boom
};

//KYT '17.04.07
/*
	HDR Struct
*/
struct YT_FirstPassCB
{
	UINT	 nWidth;			
	UINT	 nHeight;			// res
	UINT	 nTotalPixels;		// domain
	UINT	 nGroupSize;		// groupsize
								   
	float	 fAdaptation;		// 적응 값
	float	 fBloomThreshold;	// bloom 임계값
	XMFLOAT2 padding;
};


struct TwStruct
{
	int width;
	int height;
	int color[3];

};

enum class MeshTag
{
	eNone,

	// Skinned
	eTerrorist,

	// --------------------------------------------------------- // 
	MaxSkinnedMesh = 100,
	// --------------------------------------------------------- // 

	ePolice,
	// Character
	eDarkFighter, eRifle,

	// Environment

	// Building
	eBuilding21, eBuilding50,

	// Road
	eRoad,


	// Etc
	eTest, eTest2,
};
