#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 texture_coordinates;

uniform mat4 matrix_projection;

void main()
{
    gl_Position = matrix_projection * vec4(vertex.xy, 0.0, 1.0);
    texture_coordinates = vertex.zw;
}  