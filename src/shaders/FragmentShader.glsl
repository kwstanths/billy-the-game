#version 330 core

/* Define types */
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


/* Interpolated values from the vertex shader */
in vec2 uv;
in vec3 normal;
in vec3 fragment_position_worldspace;

/* Fragment shader output */
out vec3 color;

/* Uniform values */
uniform vec3 camera_position_worldspace;
uniform Light light_properties;
uniform Material object_material;

void main(){

	/* Sample the fragment color, and specular light intensity */
	vec3 fragment_color = texture(object_material.diffuse, uv).rgb;
	vec3 fragment_specular_intensity = texture(object_material.specular, uv).rgb;

	/* Calculate ambient component */
    vec3 light_ambient = light_properties.ambient * fragment_color ;

	/* Calculate diffuse component */
	vec3 normal_normalised = normalize(normal);
	vec3 light_direction_inv = normalize(light_properties.position - fragment_position_worldspace); 
	/* Calculate the cosine between these two vectors, which is the strength of the diffuse component */
	float light_diffuse_strength = max(dot(normal_normalised, light_direction_inv), 0.0);
	vec3 light_diffuse = light_properties.diffuse * light_diffuse_strength * fragment_color;
	
	/* Calculate specular component */
	vec3 view_direction = normalize(camera_position_worldspace - fragment_position_worldspace);
	/* Find the reflected vector from the light towards the surface normal */
	vec3 light_reflect_vector = reflect(-light_direction_inv, normal_normalised);
	float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
	vec3 light_specular = light_properties.specular * light_specular_strength * fragment_specular_intensity;
	
	/* Sum the two components */
	/* Specular light is currently not used */
    color = (light_ambient + light_diffuse + light_specular);

}
