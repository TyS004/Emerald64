#include <metal_stdlib>
#include <metal_types.h>

using namespace metal;

struct Matrix{
    float4x4 matrix;
};

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
    VertexIn in[[stage_in]],
    constant Matrix& model      [[buffer(0)]], 
    constant Matrix& view       [[buffer(1)]],
    constant Matrix& proj       [[buffer(2)]],
    constant Matrix& normal_mat [[buffer(3)]]) 
{
    VertexOut out;

    out.position = proj.matrix * view.matrix * model.matrix * float4(in.position, 1.0f);
    out.color = in.color;
    out.uv = in.uv;
    out.norm =      normal_mat.matrix * float4(in.norm, 1.0f);
    out.model_pos = model.matrix * float4(in.position, 1.0f);
    
    return out;
}
