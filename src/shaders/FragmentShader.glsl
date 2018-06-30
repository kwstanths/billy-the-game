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

struct CastingLight {
    vec3 position;  
    vec3 direction;
    float radius_cosine;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
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
uniform CastingLight cast_light;
uniform DirectionalLight global_illumination;

void main(){

	/* Sample the fragment color, and specular light intensity */
	vec3 fragment_color = texture(object_material.diffuse, uv).rgb;
	vec3 fragment_specular_intensity = texture(object_material.specular, uv).rgb;
	
	
	/* Calculate necessary light stuff */
	vec3 fragment_normal = normalize(normal);
	vec3 view_direction = normalize(camera_position_worldspace - fragment_position_worldspace);
		
	
	/* Calculate global illumination light */
	vec3 global_illumination_color;
	{
		/* Ambient component */
		vec3 light_ambient = global_illumination.ambient * fragment_color;
		
		/* Diffuse component */
		vec3 light_direction_inv = normalize(-global_illumination.direction); 
		float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
		vec3 light_diffuse = global_illumination.diffuse * light_diffuse_strength * fragment_color;
		
		/* Specular component */
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
		float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
		vec3 light_diffuse = point_light.diffuse * light_diffuse_strength * fragment_color;
		
		/* Calculate specular component */
		vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
		float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
		vec3 light_specular = point_light.specular * light_specular_strength * fragment_specular_intensity;
	
		/* Calculate attenuation */
		float distance = length(point_light.position - fragment_position_worldspace);
		float attenuation = 1.0 / (point_light.constant + 
									point_light.linear * distance + 
									point_light.quadratic * (distance * distance)); 
				
		point_light_color = clamp(attenuation * (light_ambient + light_diffuse + light_specular), 0, 1);
	}
	
	/* Calculate casting light color */
	vec3 cast_light_color;
	{
		/* Calculate vector between light and fragment */
		vec3 light_direction_inv = normalize(cast_light.position - fragment_position_worldspace); 
	
		/* check if lighting is inside the spotlight cone */
		float theta = dot(light_direction_inv, normalize(-cast_light.direction)); 
	
		if (theta > cast_light.radius_cosine){
			/* Diffuse component */
			float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
			vec3 light_diffuse = cast_light.diffuse *  fragment_color;  
			
			/* Specular component */
			vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
			float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
			vec3 light_specular = cast_light.specular * light_specular_strength * fragment_specular_intensity;
			
			/* Calculate attenuation */
			float distance = length(cast_light.position - fragment_position_worldspace);
			float attenuation = 1.0 / (cast_light.constant + 
									cast_light.linear * distance + 
									cast_light.quadratic * (distance * distance)); 
									
			cast_light_color = clamp(attenuation * (light_diffuse + light_specular), 0, 1);
		}
	
		
	}
	
	/* Sum the two components, the minimum color is zero, the maxium color possible the actual color of the fragment */
	color = clamp(cast_light_color + point_light_color + global_illumination_color, vec3(0,0,0), fragment_color);
}
