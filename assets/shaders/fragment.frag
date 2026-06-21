#version 450

// Inputs from vertex shader (must match vertex.vert locations)
layout(location = 0) in vec4 in_model_pos;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec4 in_norm;

layout(location = 0) out vec4 out_color;

// SDL_GPU SPIR-V convention: fragment-stage textures/storage -> set 2,
// fragment-stage uniform buffers -> set 3

// Combined texture+sampler (Metal's texture2d + sampler pair)
layout(set = 2, binding = 0) uniform sampler2D tex;

struct PointLight {
    vec3 position;
    vec4 color;
    float intensity;
};

// Storage buffer equivalent of `constant PointLight* p_lights`.
// NOTE: std430 pads vec3 to 16 bytes — make sure your CPU-side
// PointLight struct matches this layout (or pack position as vec4).
layout(std430, set = 2, binding = 1) readonly buffer PointLights {
    PointLight lights[];
} p_lights;

// Kept as separate uniform buffers, matching the original Metal
// buffer(1) / buffer(2) split (not merged into one struct).
layout(set = 3, binding = 0) uniform NumPLightsBuffer {
    uint num_p_lights;
} num_lights_buf;

layout(set = 3, binding = 1) uniform MousePosBuffer {
    vec2 mouse_pos;
} mouse_pos_buf;

void main()
{
    // Lighting params are still bound/available above, just unused here.
    //out_color = texture(tex, in_uv);
    out_color = vec4(1.0, 0.0, 0.0, 0.0);
}
