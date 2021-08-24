

//셋팅

/*
 
  //------------------------------------------
						VS 
	  ※Const Buffer
		0x00 = 00  ViewProjection Camera Buffer 
		0x07 = 01  Ortho  <- Change 02
		0x01 = 02  WorldMatrix
		0x02 = 03  Texture VS
		0x03 = 04  Terrain
		0x04 = 05  Skybox
		0x05 = 06  Projecttion  //Why..?  No Use
		0x06 = 07  Shadow 
		0x08 = 08  Particle
		0x09 = 
  //------------------------------------------

  //------------------------------------------
						PS 
  ※Const Buffer
  0x00 = 00	 cbLights
  0x01 = 01  cbMaterial
  0x02 = 02  cbFog
  0x03 = 03  Terrain // No Use Texture Arrary
  0x04 = 04  SkyPs   // No Use SkyBox Arrary

  ※ Texture2D
  0x00 = 00 Default Texture - gtxtDefault
  0x01 = 01 Detail Texture or Normal Texture 
  0x02 = 02 BumpTexture;

  0x0A = 10 Terrain Texture
  0x0B = 11 Terrain Normal Texture
  0x0C = 12 Terrain Detail Texture
  0x0D = 13 SkyBox Teture
  0x0E = 14 CubeMapped        // No Use
  0x10 = 16 Projection		  // No Use
  0x11 = 17 Shadow

  ※ SampleState     
  0x00 = 00 Default SampleState
  0x01 = 01 extra Default SampleState
  0x02 = 02 Detail Terrain  -  WARP~
  0x07 = 07 SkySample		-
  //------------------------------------------

  //------------------------------------------
					  GS
  0x00 = 00  ViewProjection Camera Buffer
  0x09 = 09  Particle

 //------------------------------------------

  TS

  //------------------------------------------
						DS
	0x00 = 00  ViewProjection Camera Buffer
	0x01 = 01  Bump Texture
	0x02 = 02  Texture_Matrix(Move)
  //------------------------------------------


  HS


  CS





*/

/*
  Update 변경사항
    
	1. playermanager
	2. terrainManager;
	3. CCubeMeshTexturedIlluminated;
	4. TerrainCubeMesh
	5. GS_TerrainMeshCube.fx
	6. stdaxf.

*/


/*
	************************************************** 추후 작업 예정 **************************************************

	  ★ GameSystem에서 Collision 처리 만들기
	    
		 -> Collision() 함수 이용하여서
		 -> Test는 메인 플레이어가 아이템을 먹고 이를 OtherPlayer에게 던지는 형식이거나
		     i,j,k,l을 이용하여 OtherPlayer를 움직이게 할 예정
	 
	  ★ 리비전 133과 비교하여 캐릭터 움직임 체크

	  1. ContentManager 있는 GameObject를 shared_ptr<CGameObject> -> CGameObject* 바꿔도 되나 가늠해보기

	  2. SpriteObject_2D 와 같은 기능을하는 SpriteObject_3D를 만들지 각 재기

	  3. UI Blending Test

	  4. 게임 결과창 만z들기

	  5. 2D->3D 전환시 자유자재로 가능하게

	  ************************************************** 추후 작업 예정 **************************************************


*/

//KYT '16.06.29
/*

    ************************* 우 선 사 항 *****************************

      ㆍ100초가 지나면 3D->2D로 바뀌기 때문에 #define 바꿔야함
      ㆍOtherPlayerComponent 중 헷갈리는데 Anmate를 return으로 막음
      ㆍTerrainPlayer::OnPlayerUpdate() 이부분을
        #ifndef _CONNECT_SERVER_로 클라테스트일떄는 실행되게 해둠

    ************************* 우 선 사 항 *****************************


   1. GameSystem 추가
       - 여기서 모든 플레이어의 대한 GameObejetc*를 가지고 있는다.
       - Load에서 몇명의 플레이어를 로드할지 결정한다.
       - 항상 정렬해서 가지고 있는다.
       - PlayerState* Vector이 있어서 플레이어 상태의 바로 접근한다.

   2. BasicSystem 추가
     - 시간제한이 있따.

   3. PlayerManager에서 OtherPlayer를 Connect되지 않아도 생성되게 만들었따.
    - #define GAME_SEVER_TEST 를 OnOff하므로써 이부분만 수정하면 되게 만들었따.
    - 그대로 컨복할떄 idx가 for(int i = 1부터임이믈 명시해라
    - m_pPlayer에 ID를 0번으로 고정값 줬다.
      이 부분ㅇ은 TCP에서 받아서 만들어야 한다.

   4. PlayerState* (Player에 게임 데이터가 있는 m_vComponent[0] <- 항상0번임)
       - 여기에 ID(Read1,Read2,Blue1,Blue2 이의미임) / Kill / Dead 넣어둠
       - Hp는 임시로 1로 넣어둠. 확장성을 위하여 bool아닌 int로 하였음
       - 1,2,3,4 각각의 ID에 해당하는 플레이어의 Hp +1
       - 5,6,7,8 각각 해당하는 ID 플레이어의 HP - 1

   5. 곧 PlayerManager OnCollision의 있는 기능을 System Collision으로 옮길예정
       - 여기서
       for(auto &player : m_vPlayers)
       {
         PlayerManager에 OnCollision하던 처리를 할것임
       }
       cuz. 충돌체크는 모든 플레이어[들]을 기준으로 검사해야하므로
            이래야지 충돌할 개체가 기하급수적으로 줄어듬

   6. BasicSystem Render에서 시간을 Render하고있음

   7. Loading 순서의 중요성
     - GameSystem->Load(pd3dDevice)를 하고
            ->PlayerLoad(플레이어 들 넣는다, 가변인자이므로 몇개 넣든 노상관)
     - GameSystem->
()를 꼭 해줘야한다.
       그래야지 Player들의 데이터를 정렬 및 PlayerState* 데이터를 넣어줌
*/
