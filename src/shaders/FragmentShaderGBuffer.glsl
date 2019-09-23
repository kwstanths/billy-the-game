#version 330 core

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
    float shininess;
};

layout(location = 0) out vec3 g_position;
layout(location = 1) out vec3 g_normal;
layout(location = 2) out vec4 g_albedo_spec;
layout(location = 3) out vec4 g_ambient;

in vec2 uv;
in vec3 normal_viewspace;
in vec3 fragment_position_viewspace;
in vec4 fragment_position_lightspace;

uniform Material object_material;
uniform sampler2D shadow_map;

float ShadowCalculation() {
    vec3 projCoords = fragment_position_lightspace.xyz / fragment_position_lightspace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadow_map, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth - 0.005 > closestDepth  ? 1.0f : 0.0; 
    return shadow;
}

void main(){
    
	vec4 texture_color = texture(object_material.texture_diffuse, uv);
	if (texture_color.rgb == vec3(1,1,1) || texture_color.a < 0.1) 
        discard;
	
    /* Ambient and shininess components of the material are not stored */
    
    float in_shadow = ShadowCalculation();
    
    g_position = fragment_position_viewspace;
    
    g_normal = normalize(normal_viewspace);
    
    g_albedo_spec.rgb = texture_color.rgb + object_material.diffuse;
    g_albedo_spec.a = texture(object_material.texture_specular, uv).r + object_material.specular.r;
    
    g_ambient.rgb = object_material.ambient;
    g_ambient.a = (1.0f - in_shadow);
}