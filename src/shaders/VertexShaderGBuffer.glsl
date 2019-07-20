#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;

out vec2 uv;
out vec3 normal_viewspace;
out vec3 fragment_position_viewspace;

uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main(){

    vec4 world_position = matrix_model * vec4(vertex_position_modelspace, 1.0);

    /* Calculate the normal in viewspace */
	normal_viewspace = mat3(transpose(inverse(matrix_view * matrix_model))) * vertex_normal;

    /* Calculate the position of the fragment in view space */
	fragment_position_viewspace = (matrix_view * world_position).xyz;
	uv = vertex_uv;
    
    /* Set output */
    gl_Position =  matrix_projection * matrix_view * world_position;
}