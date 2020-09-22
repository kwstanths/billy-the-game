#version 410 core                                                                               

layout(triangles, equal_spacing, ccw) in;

uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform mat4 matrix_lightspace;
uniform sampler2D displacement_map;
uniform float displacement_intensity;

in TCS_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} tes_in[];

out TES_OUT {
    vec2 uv;
    vec3 position_viewspace;
    vec4 position_lightspace;
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
                                                  
    float Displacement = texture(displacement_map, tes_out.uv).r * displacement_intensity;
    position_worldspace += Displacement * normal_worldspace;
    
    tes_out.position_viewspace = (matrix_view * vec4(position_worldspace, 1)).xyz;
    tes_out.position_lightspace = matrix_lightspace * vec4(position_worldspace, 1);
    
    gl_Position = matrix_projection * matrix_view * vec4(position_worldspace, 1);                                              
}                                                                                               
