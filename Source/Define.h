#pragma once
#include "stdafx.h"

#define MAX_STATIC_LIGHTS	56 //Multiple of 4
#define MAX_DYNAMIC_LIGHTS	13

#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f
#define SHADOW_LIGHT		3.0f

#define LINEAR_FOG			1.0f
#define EXP_FOG				2.0f
//#define EXP2_FOG			3.0f


#define _USE_TAN

/*--------------------------------*/
#define _AABB_ //Awaly Use
#define _SPACEPARTITIONING //SpacePartitiong

#define _QURD_ARRARY			

//#define _MULIT_THREAD_RENDER_	  //멀티스레딩
#define _MULIT_RENDER_TARGET_     //디퍼드랜더링
#define _MULIT_THREAD_LOADING_	  //멀티 쓰레드 로딩
/*--------------------------------*/
#define MULITE_RENDER_NUMBER	4


#define Default_Mesh			CCubeMeshTexturedIlluminated


//Arrary or Tree
#ifdef _SPACEPARTITIONING
	#ifdef _QURD_ARRARY
		#define _QURD_ARRARY
	#else
		#define _QURD_TREE
	#endif
#endif

/*싱글톤 선언 */
#define NODE_DEPTH_LEVEL		  4
#define		   WORLDSIZE		252
#define		 WORLD_WIDTH		252
#define		 WORLD_DEPTH		160
#define		 TERRAINSIZE		257
#define		 BLOCK_SCALE		1.0f



/* Define */
#define    FRAMEWORK		 CGameFramework::GetInstance()
#define       TIMEMGR			 CGameTimer::GetInstance()
#define         INPUT		  CInputManager::GetInstance()
#define		 SOUNDMGR		   SoundManager::GetInstance()
//#define  TEXTUREMGR			CTextureMgr::GetInstance()
//#define RENDERSTATE			 enderState::GetInstance()

#ifdef _QURD_ARRARY
	#define		SPACE	CSpacePartitionQuad::GetInstance()
#else
	#define		SPACE		CSpacePartition::GetInstance()
#endif

//3D
#define		CONTENTMGR		CContentManager::GetInstance()
#define			PLAYER		 CPlayerManager::GetInstance()
#define		  LIGHTMGR		CLightingManger::GetInstance()
#define		 CAMERAMGR		 CCameraManager::GetInstance()
#define       WORLDMAP  CCubeTerrainManager::GetInstance()
#define		   FONTMGR         CFontManager::GetInstance()
#define		  PARTICLE	   CParticleManager::GetInstance()
#define			 UIMGR			 CUIManager::GetInstance()
#define		  MODELMGR	CWarp3DModelManager::GetInstance()
#define		 HASHTABLE			 CHashTable::GetInstance()
//#define		    TWBAR		  CTwBarManager::GetInstance()

//2D
#define FRAMEWORK_2D	 CGameFrameWork_2D::GetInstance()
#define RENDERMGR_2D	 CRenderManager_2D::GetInstance()
#define SOUNDMGR			  SoundManager::GetInstance()
#define EDITBOX_2D			   CEditBox_2D::GetInstance()

//Server
#define TCP						TCPClient::GetInstance()

//Slot Define 



//#define _WITH_MSAA4_MULTISAMPLING
#define FRAME_BUFFER_WIDTH					1280
#define FRAME_BUFFER_HEIGHT					720

#define WIDTH								FRAME_BUFFER_WIDTH
#define HEIGHT								FRAME_BUFFER_HEIGHT

#define CUBEMAP_RENDER_TARGET_WIDTH			FRAME_BUFFER_WIDTH
#define CUBEMAP_RENDER_TARGET_HEIGHT		FRAME_BUFFER_HEIGHT

#define SHADOW_RENDER_TARGET_WIDTH			1280
#define SHADOW_RENDER_TARGET_HEIGHT			720

/*ConstBuffer*/
//---------------------------------------------------------------------
              //VS
#define VS_CB_SLOT_CAMERA					0
#define VS_CB_SLOT_CAMERA_ORTHO				1
#define VS_CB_SLOT_WORLD_MATRIX				2
#define VS_CB_SLOT_TEXTURE_MATRIX			3
#define VS_CB_SLOT_TERRAIN					4
#define VS_CB_SLOT_SKYBOX					5
#define VS_CB_SLOT_SPRITE_ANIMATION			6	
#define VS_CB_SLOT_SHADOW					7
#define VS_CB_SLOT_PARTICLE					8
#define VS_CB_SLOT_UI_BUFFER				9	
#define VS_CB_SLOT_AABB						10
#define VS_CB_SLOT_BONE_TRANSFORM			11

			//GS
#define GS_CB_CAMERA_POSITION				5

			//PS
#define PS_CB_SLOT_STATIC_LIGHT				1
#define PS_CB_SLOT_DYNAMIC_LIGHT			2
#define PS_CB_SLOT_MATERIAL					3
#define PS_CB_SLOT_FOG			 			4
#define PS_CB_SLOT_BLUR_MODE	 			5
#define PS_CB_SLOT_SHADOW					7
#define PS_CB_SLOT_PARTICLE					8
#define PS_CB_SLOT_LIMLIGHT					12
#define PS_CB_SLOT_OUTLINE					13

//#define PS_CB_SLOT_TERRAIN				0x03
//#define PS_CB_SLOT_SKYBOX					0x04


//---------------------------------------------------------------------


/* 
	Texture2D 
*/
//---------------------------------------------------------------------
#define PS_SLOT_TEXTURE						0
#define PS_SLOT_NORMAL_TEXTURE				1
#define PS_SLOT_DISPALCE_TEXTURE			2
#define PS_SLOT_AMBIENT_TEXTURE				3
#define PS_SLOT_SPCULAR_TEXTURE				4
#define PS_SLOT_PARTICLE_TEXTURED_ARRARY	5
#define PS_CB_SLOT_SPRITE_ANIMATION			6	
#define PS_SLOT_FONT_TEXTURE				7

#define VS_SLOT_ANIMATION_TEXTURE			10
											
#define GS_SLOT_RANDOM_TEXTURED				4

											
											
#define PS_SLOT_TEXTURE_TERRAIN				10
#define PS_SLOT_TEXTURE_SKYBOX				13 // 13
#define PS_SLOT_TEXTURE_SHADOW				17 // 17
//---------------------------------------------------------------------


/* SampleState */
//---------------------------------------------------------------------
#define PS_SLOT_SAMPLER						0
#define PS_SLOT_SAMPLER_DETAIL				1
#define PS_SLOT_SAMPLER_TERRAIN				2
#define PS_SLOT_PARTICLE_SAMPLER			5
#define PS_SLOT_SAMPLER_SHADOW				7


#define GS_SLOT_SAMPLER						2
#define GS_SLOT_PARTICLE_SAMPLER			5

//---------------------------------------------------------------------
	/* GeoMetryShader */
//---------------------------------------------------------------------
#define GS_CB_SLOT_CAMERA					0
#define GS_CB_SLOT_PARTICLE_INFO			4
#define GS_CB_SLOT_PARTICLE					9
//---------------------------------------------------------------------

/* Domain Shader */
//---------------------------------------------------------------------
 #define DS_CB_SLOT_CAMERA					0
 #define DS_CB_SLOT_BUMP					1
 #define DS_CB_SLOT_TEXTURE_MATRIX			3
//---------------------------------------------------------------------

/* HDR COmpute Shader */
//---------------------------------------------------------------------
#define CS_HDR_CB_SLOT_DOWN_SCALE_CONTANTS		0

//CS
#define CS_HDR_FLOAT4_RW_AVERAGE_LUM				0
#define CS_HDR_FLOAT_RW_HDR_DOWN_SCALE				1

#define CS_HDR_FLOAT4_TXT_AVERAGE_VAULE				0
#define CS_HDR_FLOAT_TXT_PREV_AVE_LUM				1




//PS
#define PS_HDR_TXT_FLOAT4_HDR_DOWN_SCALE		0
#define PS_HDR_TXT_FLOAT_AVG_LUM				1
#define PS_HDR_TXT_FLOAT4_BLOOM_TEXUTRE			2
#define PS_HDR_CB_SLOT_FINAL_PASS_CONTANTS		9

#define PS_HDR_RW_BLOOM							0



//---------------------------------------------------------------------



#define PI								3.14159
#define RADAIN(X)						(PI / 180) * X
#define RANDOM_COLOR	XMFLOAT4( ((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX), 1.0f)

#define RANDOM_DIRCTION	XMFLOAT3( (rand() % 200 - 100) * 0.01f, (rand() % 200 - 100) * 0.01f, (rand() % 200 - 100) * 0.01f )
#define _XTT(X)  TEXT(X)


#define _WITH_TERRAIN_PARTITION
#define _WITH_FRUSTUM_CULLING_BY_OBJECT
//#define _WITH_FRUSTUM_CULLING_BY_SUBMESH

//#define _WITH_TERRAIN_TEXTURE_ARRAY


//KYT '16.04.30
/*
  Camera Distance Length
*/

#define CULL_CAMERA_DISTANCE 40.0f

/* GamePlay */
//---------------------------------------------------------------------
#define PLAYER_SCALE 0.004f
#define PLAYER_ANISPEED 1.0f
#define PLAYER_RUN_ANISPEED 2.0f
#define CHARACTER_SPEED 5
#define BUNNY_SCALE 0.01f
#define DESSERT_SCALE 0.01f
#define DECORATE_SCALE 0.01f
#define CLOUD_SCALE 0.04f
#define ITEM_SCALE 0.03f
//---------------------------------------------------------------------

//KYT '16.06.28
/*
	타임어택 모드의 시간
*/
#define GAME_PLAY_TIME_SECOND_LIMIT 1800

//-----------------------------------
//JJY '16'.04.16 plus
/*
  서버 접속 사용 여부
  서버 접속을 할 것이 아니라면 define을 주석처리
*/
//#define _CONNECT_SERVER_
//#define _USE_COMPUTE_SHADER

#define _FBXANIMATION_LOADING_
//#define _RUNTIME_DEBUGFILE
//-------------------------------

//KYT '16.08.02
/*
	쿨타임 & 지속시간
*/

#define     COOLTIME_HIDE	  30.0f	//쿨타임 
#define PERSIST_HIDE_TIME	  10.0f	//지속시간

#define COOLTIME_TELEPORT	  3.0f	//즉시시전
#define    COOLTIME_THROW     3.0f	//즉시시전

#define		MEI_STUN_TIME	  5.0f
#define	  LIZZY_STUN_TIME	  3.0f

//-------------------------------

/*
	투사체 위치리스트만전역에서 관리하고
	Render는 Scene에서 한다.
*/

#define CS_SLOT_TEXTURE		0x00 // t0
#define CS_SLOT_RW_TEXTURE	0x00 // u0

#define PARTICLE_POOL_COUNT 100
#define PARTICLE_SIZE		0.1f
#define PARTICLE_SPEED		1.0f
#define PARTICLE_DURINGTIME 1.1f
#define PARTICLE_EMIT_COUNT 1.0

#define TapUI_RIGHTPUSH 0 //(FRAME_BUFFER_WIDTH * 0.1f)
#define TapUI_DOWNPUSH 0 //(FRAME_BUFFER_HEIGHT * 0.1f)

//#define BOUNDING_ORIENTED_BOX

//#define DEBUG_MODE_UI
#define NON_SERVER_CLIENT_NUMBER 4

#define _SELECT_GPU_DRIVE_170904	// 21-08-22 김영태 : GPU 드라이버 세팅

#ifdef DX12_MIGRATION

#define	SWAP_CHAIN_BUFFER_COUNT				2
#define	RENTER_TARGET_BUFFER_COUNT			2

#define gDevice									CGlobalvariable::GetInstance()->GetDevice()
#define gSetDevice(X)							CGlobalvariable::GetInstance()->SetDevice(X)

#define setCBSrcDescriptorIncrementSize(X)		CGlobalvariable::GetInstance()->SetCBSrvDescriptorIncrementSize(X)
#define getCBSrcDescriptorIncrementSize			CGlobalvariable::GetInstance()->GetCBSrvDescriptorIncrementSize();

#define gCommandALlocator						CGlobalvariable::GetInstance()->GetCommandAllocator()
#define gCommandQueue							CGlobalvariable::GetInstance()->GetCommandQueue()
#define gGraphicsCommandList					CGlobalvariable::GetInstance()->GetGraphicsCommandList()

#endif //DX12_MIGRATION