#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in mat4 matrix_model;

out VS_OUT {
    vec2 uv;
    vec3 normal_viewspace;
    vec3 fragment_position_viewspace;
} vs_out;

uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main(){

    vec4 world_position = matrix_model * vec4(vertex_position_modelspace, 1.0);

    /* Calculate the normal in viewspace */
	vs_out.normal_viewspace = mat3(transpose(inverse(matrix_view * matrix_model))) * vertex_normal;

    /* Calculate the position of the fragment in view space */
	vs_out.fragment_position_viewspace = (matrix_view * world_position).xyz;
	vs_out.uv = vertex_uv;
    
    /* Set output */
    gl_Position =  matrix_projection * matrix_view * world_position;
}