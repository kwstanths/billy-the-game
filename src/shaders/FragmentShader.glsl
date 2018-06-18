#version 330 core

/* Interpolated values from the vertex shader */
in vec2 uv;
in vec3 normal;
in vec3 fragment_position_globalspace;

/* Fragment shader output */
out vec3 color;

/* Texture */
uniform sampler2D sampler_texture;
uniform vec3 light_color;
uniform vec3 light_position;

void main(){

	/* Calculate ambient component */
    float light_ambient_strength = 0.1;
    vec3 light_ambient = light_ambient_strength * light_color;

	/* Calculate diffuse component */
	vec3 normal_normalised = normalize(normal);
	vec3 light_diffuse_light_direction_inv = normalize(light_position - fragment_position_globalspace); 
	/* Calculate the cosine between these two vectors, which is the strength of the diffuse component */
	float light_diffuse_strength = max(dot(normal_normalised, light_diffuse_light_direction_inv), 0.0);
	vec3 light_diffuse = light_diffuse_strength * light_color;
	
	/* Sum the two components */
    color = (light_ambient + light_diffuse) * texture(sampler_texture, uv).rgb;

}
