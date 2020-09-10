#version 410 core                                                                               

layout(triangles, equal_spacing, ccw) in;

uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform sampler2D displacement_map;  

in TCS_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} tes_in[];

out TES_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} tes_out;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)                                                   
{                                                                                               
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;   
}                                                                                               

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)                                                   
{                                                                                               
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}     

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}                                                                                          

void main()                                                                                     
{                                                                                               
    // Interpolate the attributes of the output vertex using the barycentric coordinates        
    tes_out.uv = interpolate2D(tes_in[0].uv, tes_in[1].uv, tes_in[2].uv);
    tes_out.normal_worldspace = interpolate3D(tes_in[0].normal_worldspace, tes_in[1].normal_worldspace, tes_in[2].normal_worldspace);
    tes_out.normal_worldspace = normalize(tes_out.normal_worldspace);
    tes_out.position_worldspace = interpolate3D(tes_in[0].position_worldspace, tes_in[1].position_worldspace, tes_in[2].position_worldspace);
                                                                                                
    // Displace the vertex along the normal                                                
    float Displacement = texture(displacement_map, tes_out.uv).r * 10;
    tes_out.position_worldspace += Displacement * tes_out.normal_worldspace;
    
    gl_Position = matrix_projection * matrix_view * vec4(tes_out.position_worldspace, 1.0);                                              
}                                                                                               
