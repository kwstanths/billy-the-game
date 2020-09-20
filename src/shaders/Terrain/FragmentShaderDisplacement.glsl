#version 410 core

layout(location = 0) out vec3 g_position;
layout(location = 1) out vec3 g_normal;
layout(location = 2) out vec4 g_albedo_spec;
layout(location = 3) out vec3 g_position_light;

uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform sampler2D displacement_map;
uniform float displacement_intensity;

in TES_OUT {
    vec2 uv;
    vec3 position_viewspace;
    vec4 position_lightspace;
} tes_out;

vec3 CalculateNormalFromHeightmap(){
    vec2 ts = 1.0 / vec2(textureSize(displacement_map, 0));
    float bot = texture(displacement_map, tes_out.uv + vec2(0, ts.y)).r * displacement_intensity;
    float top = texture(displacement_map, tes_out.uv + vec2(0, -ts.y)).r * displacement_intensity;
    float left = texture(displacement_map, tes_out.uv + vec2(-ts.x, 0)).r * displacement_intensity;
    float right = texture(displacement_map, tes_out.uv + vec2(ts.x, 0)).r * displacement_intensity;
    
    return vec3((left - right) / 2, 0.7,  (top - bot) / 2);
}

void main()
{
    g_position = tes_out.position_viewspace;
    
    vec3 vertex_normal = normalize(CalculateNormalFromHeightmap());
        
    /* Transform object space normal to view space */
    g_normal = normalize(transpose(inverse(mat3(matrix_view * matrix_model))) * vertex_normal);
    
    g_albedo_spec.rgb = vec3(0.0, 0.8, 0);
    g_albedo_spec.a = 0.1;
    
    g_position_light = tes_out.position_lightspace.xyz / tes_out.position_lightspace.w;
}

