#include <metal_stdlib>
#include <metal_types.h>
using namespace metal;

struct Uniform{
    float4x4 mvp;
};

struct VertexIn{
    float3 position [[attribute(0)]];
    float4 color [[attribute(1)]];
};

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

vertex VertexOut vertex_main(VertexIn in[[stage_in]], constant Uniform& u [[buffer(0)]]) {
    VertexOut out;

    out.position = u.mvp * float4(in.position, 1.0f);
    out.color = in.color;
    
    return out;
}
