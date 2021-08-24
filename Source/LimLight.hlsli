#pragma once

cbuffer LIM_LIGHT : register(b12)
{
    float3 gLim_Color;
    float gLim_RimWidth;
    float gLim_Power;
    float gLim_use;
    float2 padding;
}

float4 LimColor(float4 positionW, float3 normal, float4 cam_pos)
{
    if (0 == gLim_use) return float4(0, 0, 0, 0);

    float4 cColor;
    float3 vCameraPos = normalize(cam_pos - positionW);
    float RimLightColor = smoothstep(1.0 - gLim_RimWidth, 1.0f, 1.0f - max(0, dot(normal, vCameraPos)));
    cColor.x = RimLightColor * gLim_Power *gLim_Color.x;
    cColor.y = RimLightColor * gLim_Power *gLim_Color.y;
    cColor.z = RimLightColor * gLim_Power *gLim_Color.z;
    return cColor;
}
/*
 if (0 == gLim_use)
        return float4(0, 0, 0, 0);
    float3 vCameraPos = normalize(cam_pos - positionW);
    float RimLightColor = smoothstep(1.0 - gLim_RimWidth, 1.0f, 1.0f - max(0, dot(normal, vCameraPos)));
    return float4(RimLightColor * gLim_Power * gLim_Color.x, RimLightColor * gLim_Power * gLim_Color.y, RimLightColor * gLim_Power * gLim_Color.z, 1.0f);

*/