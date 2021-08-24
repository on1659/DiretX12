

//����

/*
 
  //------------------------------------------
						VS 
	  ��Const Buffer
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
  ��Const Buffer
  0x00 = 00	 cbLights
  0x01 = 01  cbMaterial
  0x02 = 02  cbFog
  0x03 = 03  Terrain // No Use Texture Arrary
  0x04 = 04  SkyPs   // No Use SkyBox Arrary

  �� Texture2D
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

  �� SampleState     
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
  Update �������
    
	1. playermanager
	2. terrainManager;
	3. CCubeMeshTexturedIlluminated;
	4. TerrainCubeMesh
	5. GS_TerrainMeshCube.fx
	6. stdaxf.

*/


/*
	************************************************** ���� �۾� ���� **************************************************

	  �� GameSystem���� Collision ó�� �����
	    
		 -> Collision() �Լ� �̿��Ͽ���
		 -> Test�� ���� �÷��̾ �������� �԰� �̸� OtherPlayer���� ������ �����̰ų�
		     i,j,k,l�� �̿��Ͽ� OtherPlayer�� �����̰� �� ����
	 
	  �� ������ 133�� ���Ͽ� ĳ���� ������ üũ

	  1. ContentManager �ִ� GameObject�� shared_ptr<CGameObject> -> CGameObject* �ٲ㵵 �ǳ� �����غ���

	  2. SpriteObject_2D �� ���� ������ϴ� SpriteObject_3D�� ������ �� ���

	  3. UI Blending Test

	  4. ���� ���â ��z���

	  5. 2D->3D ��ȯ�� ��������� �����ϰ�

	  ************************************************** ���� �۾� ���� **************************************************


*/

//KYT '16.06.29
/*

    ************************* �� �� �� �� *****************************

      ��100�ʰ� ������ 3D->2D�� �ٲ�� ������ #define �ٲ����
      ��OtherPlayerComponent �� �򰥸��µ� Anmate�� return���� ����
      ��TerrainPlayer::OnPlayerUpdate() �̺κ���
        #ifndef _CONNECT_SERVER_�� Ŭ���׽�Ʈ�ϋ��� ����ǰ� �ص�

    ************************* �� �� �� �� *****************************


   1. GameSystem �߰�
       - ���⼭ ��� �÷��̾��� ���� GameObejetc*�� ������ �ִ´�.
       - Load���� ����� �÷��̾ �ε����� �����Ѵ�.
       - �׻� �����ؼ� ������ �ִ´�.
       - PlayerState* Vector�� �־ �÷��̾� ������ �ٷ� �����Ѵ�.

   2. BasicSystem �߰�
     - �ð������� �ֵ�.

   3. PlayerManager���� OtherPlayer�� Connect���� �ʾƵ� �����ǰ� �������.
    - #define GAME_SEVER_TEST �� OnOff�ϹǷν� �̺κи� �����ϸ� �ǰ� �������.
    - �״�� �����ҋ� idx�� for(int i = 1�������̹� ����ض�
    - m_pPlayer�� ID�� 0������ ������ ���.
      �� �κФ��� TCP���� �޾Ƽ� ������ �Ѵ�.

   4. PlayerState* (Player�� ���� �����Ͱ� �ִ� m_vComponent[0] <- �׻�0����)
       - ���⿡ ID(Read1,Read2,Blue1,Blue2 ���ǹ���) / Kill / Dead �־��
       - Hp�� �ӽ÷� 1�� �־��. Ȯ�强�� ���Ͽ� bool�ƴ� int�� �Ͽ���
       - 1,2,3,4 ������ ID�� �ش��ϴ� �÷��̾��� Hp +1
       - 5,6,7,8 ���� �ش��ϴ� ID �÷��̾��� HP - 1

   5. �� PlayerManager OnCollision�� �ִ� ����� System Collision���� �ű濹��
       - ���⼭
       for(auto &player : m_vPlayers)
       {
         PlayerManager�� OnCollision�ϴ� ó���� �Ұ���
       }
       cuz. �浹üũ�� ��� �÷��̾�[��]�� �������� �˻��ؾ��ϹǷ�
            �̷����� �浹�� ��ü�� ���ϱ޼������� �پ��

   6. BasicSystem Render���� �ð��� Render�ϰ�����

   7. Loading ������ �߿伺
     - GameSystem->Load(pd3dDevice)�� �ϰ�
            ->PlayerLoad(�÷��̾� �� �ִ´�, ���������̹Ƿ� � �ֵ� ����)
     - GameSystem->
()�� �� ������Ѵ�.
       �׷����� Player���� �����͸� ���� �� PlayerState* �����͸� �־���
*/
