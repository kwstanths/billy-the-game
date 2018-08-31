#version 330 core

/* Define types */
struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
    float inner_radius_cosine;
    float outer_radius_cosine;
	
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

#define NR_POINT_LIGHTS 16  

/* Uniform values */
uniform vec3 camera_position_worldspace;
uniform Material object_material;

uniform PointLight point_light[NR_POINT_LIGHTS];
uniform uint number_of_point_lights;

uniform CastingLight cast_light;
uniform DirectionalLight global_illumination;

/* Function prototypes */
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, vec3 fragment_specular_intensity);
vec3 CalculatePointLight(PointLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, vec3 fragment_specular_intensity);
vec3 CalculateCastingLight(CastingLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, vec3 fragment_specular_intensity);

void main(){

	/* Sample the fragment color, and specular light intensity */
	vec3 fragment_color = texture(object_material.texture_diffuse, uv).rgb + object_material.diffuse + object_material.ambient;
	vec3 fragment_specular_intensity = texture(object_material.texture_specular, uv).rgb + object_material.specular;
	
	/* Calculate necessary light stuff */
	vec3 fragment_normal = normalize(normal);
	vec3 view_direction = normalize(camera_position_worldspace - fragment_position_worldspace);
		

	/* Calculate global illumination light */
	vec3 global_illumination_color = CalculateDirectionalLight(global_illumination, fragment_normal,view_direction, fragment_color, fragment_specular_intensity);
	
	/* Calculate the point light color */
	vec3 point_lights_color = vec3(0,0,0);
	for(uint i = 0u; i < number_of_point_lights; i++)
		point_lights_color += CalculatePointLight(point_light[i], fragment_normal,view_direction, fragment_color, fragment_specular_intensity);
	
	/* Calculate casting light color */
	vec3 cast_light_color = CalculateCastingLight(cast_light, fragment_normal, view_direction, fragment_color, fragment_specular_intensity);
	
	/* Sum total components, the minimum color is zero, the maxium color possible the actual color of the fragment */
	color = clamp(cast_light_color + point_lights_color + global_illumination_color, vec3(0,0,0), fragment_color);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, vec3 fragment_specular_intensity){
		/* Ambient component */
		vec3 light_ambient = light.ambient * fragment_color;
		
		/* Diffuse component */
		vec3 light_direction_inv = normalize(-light.direction); 
		float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
		vec3 light_diffuse = light.diffuse * light_diffuse_strength * fragment_color;
		
		/* Specular component */
		/* Find the reflected vector from the light towards the surface normal */
		vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
		float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
		vec3 light_specular = light.specular * light_specular_strength * fragment_specular_intensity;
		
		return clamp(light_ambient + light_diffuse + light_specular, 0, 1);
}

vec3 CalculatePointLight(PointLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, vec3 fragment_specular_intensity){
		/* Calculate ambient component */
		vec3 light_ambient = light.ambient * fragment_color ;
	
		/* Calculate diffuse component */
		vec3 light_direction_inv = normalize(light.position - fragment_position_worldspace); 
		float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
		vec3 light_diffuse = light.diffuse * light_diffuse_strength * fragment_color;
		
		/* Calculate specular component */
		vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
		float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
		vec3 light_specular = light.specular * light_specular_strength * fragment_specular_intensity;
	
		/* Calculate attenuation */
		float distance = length(light.position - fragment_position_worldspace);
		float attenuation = 1.0 / (light.constant + 
									light.linear * distance + 
									light.quadratic * (distance * distance)); 
				
		return clamp(attenuation * (light_ambient + light_diffuse + light_specular), 0, 1);
}

vec3 CalculateCastingLight(CastingLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, vec3 fragment_specular_intensity){
/* Calculate vector between light and fragment */
		vec3 light_direction_inv = normalize(light.position - fragment_position_worldspace); 
	
		/* Calculate angle between cast light direction and the vector between the fragment and the cast light */
		float theta = dot(light_direction_inv, normalize(-light.direction)); 
	
		/* Calculate ambient component */
		vec3 light_ambient = light.ambient * fragment_color;
	
		/* Calculate diffuse component */
		float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
		vec3 light_diffuse = light.diffuse *  light_diffuse_strength * fragment_color;  
		
		/* Calculate specular component */
		vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
		float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), object_material.shininess);
		vec3 light_specular = light.specular * light_specular_strength * fragment_specular_intensity;
		
		/* 
			Calculate edges factor
			theta angles lower than the cast light inner radius get intensity of 1
			theta angles between the cast light inner radius and outer radius get linear decreasing to zero intensity
			theta angles greater than the cast light outer radius get intensity pof zero 
		*/
		float epsilon = (light.inner_radius_cosine - light.outer_radius_cosine);
		float intensity = clamp((theta - light.outer_radius_cosine) / epsilon, 0.0, 1.0);
		
		/* Calculate attenuation */
		float distance = length(light.position - fragment_position_worldspace);
		float attenuation = 1.0 / (light.constant + 
								light.linear * distance + 
								light.quadratic * (distance * distance)); 
							
		/* Sum components */
		return clamp(attenuation * intensity *(light_ambient + light_diffuse + light_specular), 0, 1);
}