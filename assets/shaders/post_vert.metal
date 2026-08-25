#include <metal_stdlib>
#include <metal_types.h>

using namespace metal;

struct VertexIn{
    float3 position [[attribute(0)]];
    float4 color    [[attribute(1)]];
    float2 uv       [[attribute(2)]];
    float3 norm     [[attribute(3)]];
};

struct VertexOut {
    float4 position [[position]];
    float4 model_pos;
    float4 color;
    float2 uv;
    float4 norm;
};

vertex VertexOut vertex_main(
    VertexIn in[[stage_in]]) 
{
    VertexOut out;

    out.position = float4(in.position, 1.0f);
    out.color = in.color;
    out.uv = in.uv;
    out.norm = float4(in.norm, 1.0f);
    out.model_pos = float4(in.position, 1.0f);
    
    return out;
}
