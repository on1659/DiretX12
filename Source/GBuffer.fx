

//KYT '16.02.25 
/*
  Gbuffer.


  positionW.xyz = pos;
  psoitionW.w = Lighting���� ���� ��������

  normal.xyz = normal;
  normal.w = Fog ���� ���� ���� (�ӽ�)

  normal.w = 10; : UI�� 

*/

struct GBUFFER
{
	float4 diffuse   : SV_TARGET0;
    float4 positionW : SV_TARGET1;
	float4 normal    : SV_TARGET2;
    float4 shadows   : SV_TARGET3;

};

