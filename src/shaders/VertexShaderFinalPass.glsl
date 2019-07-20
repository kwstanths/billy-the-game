#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;

out vec2 uv;

void main(){
    uv = vertex_uv;
    gl_Position = vec4(vertex_position_modelspace, 1.0);
}
