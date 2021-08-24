
#include "ParticleDefine.fx"

PS_PARTICLE_FIRE ParticleFire_VS_OUTPUT(VS_PARTICLE_FIRE input)
{
	PS_PARTICLE_FIRE output;

	output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);

	// Store the texture coordinates for the pixel shader.
	output.texCoord = input.texCoord;

	// Compute texture coordinates for first noise texture using the first scale and upward scrolling speed values.
	output.texCoords1 = (input.texCoord * scales.x);
	output.texCoords1.y = output.texCoords1.y + (frameTime * scrollSpeeds.x);

	// Compute texture coordinates for second noise texture using the second scale and upward scrolling speed values.
	output.texCoords2 = (input.texCoord * scales.y);
	output.texCoords2.y = output.texCoords2.y + (frameTime * scrollSpeeds.y);

	// Compute texture coordinates for third noise texture using the third scale and upward scrolling speed values.
	output.texCoords3 = (input.texCoord * scales.z);
	output.texCoords3.y = output.texCoords3.y + (frameTime * scrollSpeeds.z);

	return output;
}

float4 ParticleFire_PS_OUTPUT(PS_PARTICLE_FIRE input) : SV_Target
{
	float4 noise1;
	float4 noise2;
	float4 noise3;
	float4 finalNoise;
	float perturb;
	float2 noiseCoords;
	float4 fireColor;
	float4 alphaColor;


	// Sample the same noise texture using the three different texture coordinates to get three different noise scales.
	noise1 = gtxtNoise.Sample(gssPariticle, input.texCoords1);
	noise2 = gtxtNoise.Sample(gssPariticle, input.texCoords2);
	noise3 = gtxtNoise.Sample(gssPariticle, input.texCoords3);

	// Move the noise from the (0, 1) range to the (-1, +1) range.
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	// Distort the three noise x and y coordinates by the three different distortion x and y values.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	// Combine all three distorted noise results into a single noise result.
	finalNoise = noise1 + noise2 + noise3;

	// Perturb the input texture Y coordinates by the distortion scale and bias values.  
	// The perturbation gets stronger as you move up the texture which creates the flame flickering at the top effect.
	perturb = ((1.0f - input.texCoord.y) * distortionScale) + distortionBias;

	// Now create the perturbed and distorted texture sampling coordinates that will be used to sample the fire color texture.
	noiseCoords.xy = (finalNoise.xy * perturb) + input.texCoord.xy;

	// Sample the color from the fire texture using the perturbed and distorted texture sampling coordinates.
	// Use the clamping sample state instead of the wrap sample state to prevent flames wrapping around.
	fireColor = gtxtDefault.Sample(gssPariticleDetial, noiseCoords.xy);

	// Sample the alpha value from the alpha texture using the perturbed and distorted texture sampling coordinates.
	// This will be used for transparency of the fire.
	// Use the clamping sample state instead of the wrap sample state to prevent flames wrapping around.
	alphaColor = gtxtAlpha.Sample(gssPariticleDetial, noiseCoords.xy);

	// Set the alpha blending of the fire to the perturbed and distored alpha texture value.
	fireColor.a = alphaColor;

	return fireColor;
}

GBUFFER ParticleFire_PS_OUTPUT_GBUFFER(PS_PARTICLE_FIRE input) : SV_Target
{
	float4 noise1;
	float4 noise2;
	float4 noise3;
	float4 finalNoise;
	float perturb;
	float2 noiseCoords;
	float4 fireColor;
	float4 alphaColor;


	// Sample the same noise texture using the three different texture coordinates to get three different noise scales.
	noise1 = gtxtNoise.Sample(gssPariticle, input.texCoords1);
	noise2 = gtxtNoise.Sample(gssPariticle, input.texCoords2);
	noise3 = gtxtNoise.Sample(gssPariticle, input.texCoords3);

	// Move the noise from the (0, 1) range to the (-1, +1) range.
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	// Distort the three noise x and y coordinates by the three different distortion x and y values.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	// Combine all three distorted noise results into a single noise result.
	finalNoise = noise1 + noise2 + noise3;

	// Perturb the input texture Y coordinates by the distortion scale and bias values.  
	// The perturbation gets stronger as you move up the texture which creates the flame flickering at the top effect.
	perturb = ((1.0f - input.texCoord.y) * distortionScale) + distortionBias;

	// Now create the perturbed and distorted texture sampling coordinates that will be used to sample the fire color texture.
	noiseCoords.xy = (finalNoise.xy * perturb) + input.texCoord.xy;

	// Sample the color from the fire texture using the perturbed and distorted texture sampling coordinates.
	// Use the clamping sample state instead of the wrap sample state to prevent flames wrapping around.
	fireColor = gtxtDefault.Sample(gssPariticleDetial, noiseCoords.xy);

	// Sample the alpha value from the alpha texture using the perturbed and distorted texture sampling coordinates.
	// This will be used for transparency of the fire.
	// Use the clamping sample state instead of the wrap sample state to prevent flames wrapping around.
	alphaColor = gtxtAlpha.Sample(gssPariticleDetial, noiseCoords.xy);

	// Set the alpha blending of the fire to the perturbed and distored alpha texture value.
	fireColor.a = alphaColor;


	
	GBUFFER output = (GBUFFER)0;
	output.positionW = float4(input.positionW, 1.0f);
	//output.normal = float4(input.normal, 1.0f);
	output.diffuse = fireColor;

	output.ambient = gMaterial.m_cAmbient;
	output.specular = gMaterial.m_cSpecular;

	return output;

}
/*
fxc /E ParticleFire_PS_OUTPUT /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo Fire.fx
*/
