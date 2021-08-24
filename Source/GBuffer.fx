

//KYT '16.02.25 
/*
  Gbuffer.


  positionW.xyz = pos;
  psoitionW.w = Lighting할지 말지 말지결정

  normal.xyz = normal;
  normal.w = Fog 할지 말지 결정 (임시)

  normal.w = 10; : UI임 

*/

struct GBUFFER
{
	float4 diffuse   : SV_TARGET0;
    float4 positionW : SV_TARGET1;
	float4 normal    : SV_TARGET2;
    float4 shadows   : SV_TARGET3;

};

