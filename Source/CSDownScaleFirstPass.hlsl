#include "PostProcessingDefine.hlsli"

float DownScale4X4(uint2 cur_pixel, uint grupThread)
{
    float avgLum = 0.0f;

    if(cur_pixel.y < Res.y)
    {
        int3 i3FullResPos = int3(cur_pixel * 4, 0);
        float4 f4DownScale = float4(0.0f, 0.0f, 0.0f, 0.0f);
        [uroll]
        for (int y = 0; y < 4; ++y)
        {
            for (int x = 0; x < 4; ++x)
            {
                [unroll]
                f4DownScale += HDRTex.Load(i3FullResPos, int2(x, y));
            }

        }
        f4DownScale /= 16;
        HDRDownScale[cur_pixel.xy] = f4DownScale; //1/4이미지 해상도 저장

        avgLum = dot(f4DownScale, LUM_FACTOR);
        SharedPositions[grupThread] = avgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // 동기화, 여기까지 다른 쓰레드 종료를 기다림

    return avgLum;
}

[numthreads(1024, 1, 1)]
void main(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID, uint3 dispatchThreadId : SV_DispatchThreadID)
{
    uint2 CurPixel = uint2(dispatchThreadId.x % Res.x, dispatchThreadId.x / Res.x);

	// Skip out of bound pixels
    float avgLum = DownScale4X4(CurPixel, groupThreadId.x);
    // Down scale from 1024 to 256
    if (groupThreadId.x % 4 == 0)
    {
		// Calculate the luminance sum for this step
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadId.x + 1 < Domain ? SharedPositions[groupThreadId.x + 1] : avgLum;
        stepAvgLum += dispatchThreadId.x + 2 < Domain ? SharedPositions[groupThreadId.x + 2] : avgLum;
        stepAvgLum += dispatchThreadId.x + 3 < Domain ? SharedPositions[groupThreadId.x + 3] : avgLum;
		
		// Store the results
        avgLum = stepAvgLum;
        SharedPositions[groupThreadId.x] = stepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 256 to 64
    if (groupThreadId.x % 16 == 0)
    {
		// Calculate the luminance sum for this step
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadId.x + 4 < Domain ? SharedPositions[groupThreadId.x + 4] : avgLum;
        stepAvgLum += dispatchThreadId.x + 8 < Domain ? SharedPositions[groupThreadId.x + 8] : avgLum;
        stepAvgLum += dispatchThreadId.x + 12 < Domain ? SharedPositions[groupThreadId.x + 12] : avgLum;

		// Store the results
        avgLum = stepAvgLum;
        SharedPositions[groupThreadId.x] = stepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 64 to 16
    if (groupThreadId.x % 64 == 0)
    {
		// Calculate the luminance sum for this step
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadId.x + 16 < Domain ? SharedPositions[groupThreadId.x + 16] : avgLum;
        stepAvgLum += dispatchThreadId.x + 32 < Domain ? SharedPositions[groupThreadId.x + 32] : avgLum;
        stepAvgLum += dispatchThreadId.x + 48 < Domain ? SharedPositions[groupThreadId.x + 48] : avgLum;

		// Store the results
        avgLum = stepAvgLum;
        SharedPositions[groupThreadId.x] = stepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 16 to 4
    if (groupThreadId.x % 256 == 0)
    {
		// Calculate the luminance sum for this step
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadId.x + 64 < Domain ? SharedPositions[groupThreadId.x + 64] : avgLum;
        stepAvgLum += dispatchThreadId.x + 128 < Domain ? SharedPositions[groupThreadId.x + 128] : avgLum;
        stepAvgLum += dispatchThreadId.x + 192 < Domain ? SharedPositions[groupThreadId.x + 192] : avgLum;

		// Store the results
        avgLum = stepAvgLum;
        SharedPositions[groupThreadId.x] = stepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 4 to 1
    if (groupThreadId.x == 0)
    {
		// Calculate the average lumenance for this thread group
        float fFinalAvgLum = avgLum;
        fFinalAvgLum += dispatchThreadId.x + 256 < Domain ? SharedPositions[groupThreadId.x + 256] : avgLum;
        fFinalAvgLum += dispatchThreadId.x + 512 < Domain ? SharedPositions[groupThreadId.x + 512] : avgLum;
        fFinalAvgLum += dispatchThreadId.x + 768 < Domain ? SharedPositions[groupThreadId.x + 768] : avgLum;
        fFinalAvgLum /= 1024.0;

        AverageLum[groupId.x] = fFinalAvgLum; // Write the final value into the 1D UAV which will be used on the next step
    }
}