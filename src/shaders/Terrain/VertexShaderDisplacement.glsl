#version 410 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;

out VS_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} vs_out;

uniform mat4 matrix_model;

void main()                                                                                     
{
    vs_out.position_worldspace = (matrix_model * vec4(vertex_position_modelspace, 1.0)).xyz;                                  
    vs_out.uv = vertex_uv;                                                            
    vs_out.normal_worldspace   = mat3(transpose(inverse(matrix_model))) * vertex_normal;                                   
}
