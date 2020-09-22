#version 410 core                                                                               

layout(triangles, equal_spacing, ccw) in;

uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform float time;

in TCS_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} tes_in[];

out VS_OUT {
    vec2 uv;
    vec3 normal_viewspace;
    vec3 fragment_position_worldspace;
    vec3 fragment_position_worldspace_undisplaced;
} tes_out;

#define PI 3.14

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)                                                   
{                                                                                               
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;   
}                                                                                               

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)                                                   
{                                                                                               
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}



void main()                                                                                     
{                                                                                               
    tes_out.uv = interpolate2D(tes_in[0].uv, tes_in[1].uv, tes_in[2].uv);
    
    /* World space normal will be used as displacement direction */
    vec3 normal_worldspace = interpolate3D(tes_in[0].normal_worldspace, tes_in[1].normal_worldspace, tes_in[2].normal_worldspace);
    normal_worldspace = normalize(normal_worldspace);
    
    vec3 position_worldspace = interpolate3D(tes_in[0].position_worldspace, tes_in[1].position_worldspace, tes_in[2].position_worldspace);
    tes_out.fragment_position_worldspace_undisplaced = position_worldspace;
    
    vec3 displacement = vec3(0, 0, 0);
    
    /* add waves */
    vec3 direction = vec3(-1, 0, 0.01);
    float wavelength = 15.5;
    float amp = 0.15;
    
    float pdotk = dot(position_worldspace, direction);
    float vel = sqrt(1.5621f * wavelength);
    float phase = (6.28f / wavelength) * (pdotk + vel * time);
    
    vec2 offset;
    offset.x = sin(phase);
    offset.y = cos(phase);
    offset.x *= - amp;
    offset.y *= - amp;
    
    displacement += vec3(direction.x * offset.x, - offset.y, direction.z * offset.x);

    position_worldspace += displacement;

    tes_out.fragment_position_worldspace = (vec4(position_worldspace, 1)).xyz;
    
    gl_Position = matrix_projection * matrix_view * vec4(position_worldspace, 1);                                              
}                                                                                               
