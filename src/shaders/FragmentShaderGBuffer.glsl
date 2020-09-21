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
layout(location = 3) out vec3 g_position_light;

in VS_OUT {
    vec2 uv;
    vec3 normal_viewspace;
    vec3 fragment_position_viewspace;
    vec4 fragment_position_lightspace;
} vs_in;

uniform Material object_material;

void main(){
    
	vec4 texture_color = texture(object_material.texture_diffuse, vs_in.uv);
	if (texture_color.a < 0.1) 
        discard;
	
    /* Ambient and shininess components of the material are not stored */
    
    g_position = vs_in.fragment_position_viewspace;
    
    g_normal = normalize(vs_in.normal_viewspace);
    
    g_albedo_spec.rgb = texture_color.rgb + object_material.diffuse;
    g_albedo_spec.a = texture(object_material.texture_specular, vs_in.uv).r + object_material.specular.r;
    
    g_position_light = vs_in.fragment_position_lightspace.xyz / vs_in.fragment_position_lightspace.w;
}