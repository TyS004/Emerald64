Texture2D tex : register(t0, space2);
SamplerState smp : register(s0, space2);

struct PSInput
{
    float4 position : SV_Position;
    float4 color : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float4 norm : TEXCOORD2;
};

static const float Bayer8x8[64] =
{
    0, 32, 8, 40, 2, 34, 10, 42,
    48, 16, 56, 24, 50, 18, 58, 26,
    12, 44, 4, 36, 14, 46, 6, 38,
    60, 28, 52, 20, 62, 30, 54, 22,
    3, 35, 11, 43, 1, 33, 9, 41,
    51, 19, 59, 27, 49, 17, 57, 25,
    15, 47, 7, 39, 13, 45, 5, 37,
    63, 31, 55, 23, 61, 29, 53, 21
};


float GetThreshold(uint2 pixel)
{
    uint x = pixel.x & 7;
    uint y = pixel.y & 7;
    return Bayer8x8[y * 8 + x] / 64.0;
}

float4 main(PSInput input) : SV_Target
{
    float4 color = tex.Sample(smp, input.uv);
    float levels = 4.0; // 32 levels per channel
    
    float threshold = GetThreshold((uint2) input.position.xy);

    float3 c = color.rgb * (levels - 1);

    float3 base = floor(c);
    float3 fract = frac(c);

    float3 result = (base + step(threshold, fract)) / (levels - 1);

    //return float4(result, 1);
    return tex.Sample(smp, input.uv);

}