

Texture2D gtxTexture : register(t0);
SamplerState gssSamplerState : register(s0);

struct VS_TEXTURED_OUTPUT
{
    float4 position     : SV_POSITION;
    float2 texCoord     : TEXCOORD0;
    float3 cam_lightPos : POSITION;
    float3 cam_Pos      : POSITION1;
};

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target
{
   float3 pos = gtxTexture.Sample(gssSamplerState, input.texCoord);
    float dis_light = distance(pos.z, input.cam_lightPos.z);
    float dis_cam = distance(pos.z, input.cam_Pos.z);

  //  return float4(dis_light / input.cam_lightPos.z, 0, 0, 1.0f);
   // return float4(0.0f, 1.0f, 0.0f, 1.0f);

    if(dis_light > dis_cam)
         return float4(0.0f, 0.0f, 0.0f, 1.0f);

    return float4(dis_light * 0.1f, dis_light * 0.1f, dis_light * 0.1f, 0);

     return float4(0, input.cam_lightPos.y, 0, 1);
}