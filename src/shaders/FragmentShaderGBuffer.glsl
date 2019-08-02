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

in vec2 uv;
in vec3 normal_viewspace;
in vec3 fragment_position_viewspace;

uniform Material object_material;

void main(){
    
	vec4 texture_color = texture(object_material.texture_diffuse, uv);
	if (texture_color.rgb == vec3(1,1,1)) 
		discard; 
	
    /* Ambient and shininess components of the material are not stored */
    
    g_position = fragment_position_viewspace;
    
    g_normal = normalize(normal_viewspace);
    
    g_albedo_spec.rgb = texture_color.rgb + object_material.diffuse;
    
    g_albedo_spec.a = texture(object_material.texture_specular, uv).r + object_material.specular.r;
}