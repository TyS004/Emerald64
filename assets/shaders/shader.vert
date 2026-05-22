#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform Uniform {
    mat4 mvp;
};

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    outColor = color;
}