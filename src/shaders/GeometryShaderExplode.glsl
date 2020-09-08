#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 uv;
    vec3 normal_viewspace;
    vec3 fragment_position_viewspace;
    vec4 fragment_position_lightspace;
} gs_in[];

out GS_OUT {
    vec2 uv;
    vec3 normal_viewspace;
    vec3 fragment_position_viewspace;
    vec4 fragment_position_lightspace;
} gs_out;

uniform float time;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 1.5;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
} 

void main() {
    mat4 vp = matrix_projection * matrix_view;
    gl_Position = vp * explode(gl_in[0].gl_Position, gs_in[0].normal_viewspace);
    gs_out.uv = gs_in[0].uv;
    gs_out.normal_viewspace = gs_in[0].normal_viewspace;
    gs_out.fragment_position_viewspace = gs_in[0].fragment_position_viewspace;
    gs_out.fragment_position_lightspace = gs_in[0].fragment_position_lightspace;
    EmitVertex();
    
    gl_Position = vp * explode(gl_in[1].gl_Position, gs_in[0].normal_viewspace);
    gs_out.uv = gs_in[1].uv;
    gs_out.normal_viewspace = gs_in[1].normal_viewspace;
    gs_out.fragment_position_viewspace = gs_in[1].fragment_position_viewspace;
    gs_out.fragment_position_lightspace = gs_in[1].fragment_position_lightspace;
    EmitVertex();
    
    gl_Position = vp * explode(gl_in[2].gl_Position, gs_in[0].normal_viewspace);
    gs_out.uv = gs_in[2].uv;
    gs_out.normal_viewspace = gs_in[2].normal_viewspace;
    gs_out.fragment_position_viewspace = gs_in[2].fragment_position_viewspace;
    gs_out.fragment_position_lightspace = gs_in[2].fragment_position_lightspace;
    EmitVertex();
    
    EndPrimitive();
}  