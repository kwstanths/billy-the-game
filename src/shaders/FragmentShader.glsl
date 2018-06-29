#version 330 core

/* Define types */
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct PointLight {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {
	vec3 direction;
	
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
uniform Material object_material;
uniform PointLight point_light;
uniform DirectionalLight global_illumination;

void main(){

	/* Sample the fragment color, and specular light intensity */
	vec3 fragment_color = texture(object_material.diffuse, uv).rgb;
	vec3 fragment_specular_intensity = texture(object_material.specular, uv).rgb;
	
	
	/* Calculate necessary light stuff */
	vec3 fragment_normal = normalize(normal);
	
	
	/* Calculate global illumination light */
	vec3 global_illumination_color;
	{
		/* Ambient component */
		vec3 light_ambient = global_illumination.ambient * fragment_color;
		
		/* Diffuse component */
		vec3 light_direction_inv = normalize(-global_illumination.direction); 
		/* Calculate the cosine between these two vectors, which is the strength of the diffuse component */
		float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
		vec3 light_diffuse = global_illumination.diffuse * light_diffuse_strength * fragment_color;
		
		/* Specular component */
		vec3 view_direction = normalize(camera_position_worldspace - fragment_position_worldspace);
		/* Find the reflected vector from the light towards the surface normal */
		vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
		float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
		vec3 light_specular = global_illumination.specular * light_specular_strength * fragment_specular_intensity;
		
		global_illumination_color = clamp(light_ambient + light_diffuse + light_specular, 0, 1);
	}
	
	/* Calculate the point light color */
	vec3 point_light_color;
	{
		/* Calculate ambient component */
		vec3 light_ambient = point_light.ambient * fragment_color ;
	
		/* Calculate diffuse component */
		vec3 light_direction_inv = normalize(point_light.position - fragment_position_worldspace); 
		/* Calculate the cosine between these two vectors, which is the strength of the diffuse component */
		float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
		vec3 light_diffuse = point_light.diffuse * light_diffuse_strength * fragment_color;
		
		/* Calculate specular component */
		vec3 view_direction = normalize(camera_position_worldspace - fragment_position_worldspace);
		/* Find the reflected vector from the light towards the surface normal */
		vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
		float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
		vec3 light_specular = point_light.specular * light_specular_strength * fragment_specular_intensity;
	
		/* Calculate attenuation */
		float distance = length(point_light.position - fragment_position_worldspace);
		float attenuation = 1.0 / (point_light.constant + 
									point_light.linear * distance + 
									point_light.quadratic * (distance * distance)); 
				
		point_light_color = attenuation * (light_ambient + light_diffuse + light_specular);
	}
	
	/* Sum the two components */
	color = point_light_color + global_illumination_color;
	
}
