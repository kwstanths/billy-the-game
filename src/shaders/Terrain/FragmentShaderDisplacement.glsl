#version 410 core

layout(location = 0) out vec3 g_position;
layout(location = 1) out vec3 g_normal;
layout(location = 2) out vec4 g_albedo_spec;
layout(location = 3) out vec3 g_position_light;

uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform sampler2D normal_map;

in TES_OUT {
    vec2 uv;
    vec3 position_viewspace;
    vec4 position_lightspace;
} tes_out;

vec3 TransformNormalFromNormalMap(vec3 normal){
    return  normal * 2.0 - 1.0;
}

void main()
{
    g_position = tes_out.position_viewspace;
    
    vec3 tn = TransformNormalFromNormalMap(texture(normal_map, tes_out.uv).rgb);
    
    /* TODO Properly transform from normal map space to object space */
    vec3 vertex_normal = vec3(tn.x, tn.z, tn.y);
    
    /* Transform object space normal to view space */
    g_normal = normalize(transpose(inverse(mat3(matrix_view * matrix_model))) * vertex_normal);
    
    g_albedo_spec.rgb = vec3(0, 0.6, 0);
    g_albedo_spec.a = 0.3;
    
    g_position_light = tes_out.position_lightspace.xyz / tes_out.position_lightspace.w;
}

