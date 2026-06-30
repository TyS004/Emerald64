cbuffer mouse_pos : register(b0, space3)
{
    float2 mouse_pos;
}

struct PSInput
{
    float4 position : SV_Position;
    float4 model_pos : TEXCOORD0;
    float4 screen_pos : TEXCOORD1;
    float4 color : TEXCOORD2;
    float2 uv : TEXCOORD3;
    float4 norm : TEXCOORD4;
};

static float2 mouse_pos_now;

float4 main(PSInput input) : SV_Target
{
    return float4(0.0, 1.0, 1.0, 1.0);
}