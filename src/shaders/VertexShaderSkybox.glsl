#version 330 core
layout(location = 0) in vec3 vertex_position_worldspace;

out vec3 uv;

uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main()
{
    uv = vertex_position_worldspace;
    vec4 pos =  matrix_projection * matrix_view * vec4(vertex_position_worldspace,1);
    gl_Position = pos.xyww;
}  