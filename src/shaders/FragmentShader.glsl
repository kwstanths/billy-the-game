#version 330 core

/* Define types */
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

/* Interpolated values from the vertex shader */
in vec2 uv;
in vec3 normal;
in vec3 fragment_position_worldspace;

/* Fragment shader output */
out vec3 color;

/* Uniform values */
uniform sampler2D sampler_texture;
uniform vec3 camera_position_worldspace;
uniform vec3 light_color;
uniform vec3 light_position;
uniform Material object_material;

void main(){

	/* Calculate ambient component */
    vec3 light_ambient = light_color * object_material.ambient;

	/* Calculate diffuse component */
	vec3 normal_normalised = normalize(normal);
	vec3 light_direction_inv = normalize(light_position - fragment_position_worldspace); 
	/* Calculate the cosine between these two vectors, which is the strength of the diffuse component */
	float light_diffuse_strength = max(dot(normal_normalised, light_direction_inv), 0.0);
	vec3 light_diffuse = light_color * light_diffuse_strength * object_material.diffuse;
	
	/* Calculate specular component */
	vec3 view_direction = normalize(camera_position_worldspace - fragment_position_worldspace);
	/* Find the reflected vector from the light towards the surface normal */
	vec3 light_reflect_vector = reflect(-light_direction_inv, normal_normalised);
	float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
	vec3 light_specular = light_color * light_specular_strength * object_material.specular;
	
	
	/* Sum the two components */
	/* Specular light is currently not used */
    color = (light_ambient + light_diffuse) * texture(sampler_texture, uv).rgb;

}
