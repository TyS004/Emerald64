#include <metal_stdlib>
using namespace metal;

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

vertex VertexOut vertex_main(uint vid [[vertex_id]]) {
    // Define vertices directly (or load from buffer)
    const float4 positions[3] = {
        float4( 0.0,  0.5, 0.0, 1.0),
        float4(-0.5, -0.5, 0.0, 1.0),
        float4( 0.5, -0.5, 0.0, 1.0)
    };
    
    const float4 colors[3] = {
        float4(1.0, 0.0, 0.0, 1.0), // Red
        float4(0.0, 1.0, 0.0, 1.0), // Green
        float4(0.0, 0.0, 1.0, 1.0)  // Blue
    };

    VertexOut out;
    out.position = positions[vid];
    out.color = colors[vid];
    return out;
}
