#version 330 core

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

layout(location = 0) out vec3 FragColor;

in vec2 uv;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;
uniform sampler2D ssao_texture;
uniform mat4 matrix_view;

#define NR_POINT_LIGHTS 24
uniform PointLight point_light[NR_POINT_LIGHTS];
uniform uint number_of_point_lights;

uniform DirectionalLight directional_light;
uniform CastingLight cast_light;


vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor);
vec3 CalculatePointLight(PointLight light, vec3 fragment_position, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor);
vec3 CalculateCastingLight(CastingLight light, vec3 fragment_position, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor);

vec3 TransformToViewSpace(vec4 vector){
	return (matrix_view * vector).xyz;
}

void main() {
    
    vec3 fragment_position_viewspace = texture(g_position, uv).xyz;
    vec3 normal_viewspace = normalize(texture(g_normal, uv).rgb);
    vec3 fragment_color = texture(g_albedo_spec, uv).rgb;
    float fragment_specular_intensity = texture(g_albedo_spec, uv).a;
    float ambient_factor = texture(ssao_texture, uv).r;
    vec3 view_direction = normalize(-fragment_position_viewspace);
    
	/* Calculate directional light color contribution */
	vec3 directional_light_color = CalculateDirectionalLight(directional_light, normal_viewspace, view_direction, fragment_color, fragment_specular_intensity, ambient_factor);
	
	/* Calculate point lights color contribution */
	vec3 point_lights_color = vec3(0, 0, 0);
	for(uint i = 0u; i < number_of_point_lights; i++)
		point_lights_color += CalculatePointLight(point_light[i], fragment_position_viewspace, normal_viewspace, view_direction, fragment_color, fragment_specular_intensity, ambient_factor);
		
	/* Calculate casting light color contribution */
	vec3 cast_light_color = CalculateCastingLight(cast_light, fragment_position_viewspace, normal_viewspace, view_direction, fragment_color, fragment_specular_intensity, ambient_factor);
	
	/* Sum total components, the minimum color is zero, the maxium color possible the actual color of the fragment */
	FragColor = clamp(cast_light_color + point_lights_color + directional_light_color, vec3(0,0,0), fragment_color);
}
   
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor) {
    light.direction = normalize(TransformToViewSpace(vec4(light.direction, 0)));
	
	/* Ambient component */
	vec3 light_ambient = ambient_factor * light.ambient * fragment_color;
	
	/* Diffuse component */
	vec3 light_direction_inv = normalize(-light.direction); 
	float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
	vec3 light_diffuse = light.diffuse * light_diffuse_strength * fragment_color;
	
	/* Specular component */
	/* Find the reflected vector from the light towards the surface normal */
	vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
	float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), 16.0);
	vec3 light_specular = light.specular * light_specular_strength * fragment_specular_intensity;
	
       //light_ambient = clamp(light_ambient, 0, 0);
       //light_diffuse = clamp(light_diffuse, 0, 0);
       //light_specular = clamp(light_specular, 0, 0);
       
	return light_ambient + light_diffuse + light_specular;
}

vec3 CalculatePointLight(PointLight light, vec3 fragment_position, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor){

	light.position = TransformToViewSpace(vec4(light.position, 1));

    /* Calculate ambient component */
    vec3 light_ambient = ambient_factor * light.ambient * fragment_color;
    
    /* Calculate diffuse component */
    vec3 light_direction_inv = normalize(light.position - fragment_position); 
    float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
    vec3 light_diffuse = light.diffuse * light_diffuse_strength * fragment_color;
    
    /* Calculate specular component */
    vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
    float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), 16.0);
    vec3 light_specular = light.specular * light_specular_strength * fragment_specular_intensity;
    
    /* Calculate attenuation */
    float distance = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + 
                                light.linear * distance + 
                                light.quadratic * (distance * distance)); 
    
        //light_ambient = clamp(light_ambient, 0, 0);
        //light_diffuse = clamp(light_diffuse, 0, 0);
        //light_specular = clamp(light_specular, 0, 0);
		
    return attenuation * (light_ambient + light_diffuse + light_specular);
}

vec3 CalculateCastingLight(CastingLight light, vec3 fragment_position, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor){
	
	light.position = TransformToViewSpace(vec4(light.position, 1));
	light.direction = normalize(TransformToViewSpace(vec4(light.direction, 0)));
	
	/* Calculate vector between light and fragment */
	vec3 light_direction_inv = normalize(light.position - fragment_position); 
	
	/* Calculate angle between cast light direction and the vector between the fragment and the cast light */
	float theta = dot(light_direction_inv, normalize(-light.direction)); 
	
	/* Calculate ambient component */
	vec3 light_ambient = ambient_factor * light.ambient * fragment_color;
	
	/* Calculate diffuse component */
	float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
	vec3 light_diffuse = light.diffuse *  light_diffuse_strength * fragment_color;  
		
	/* Calculate specular component */
	vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
	float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), 16.0);
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
	float distance = length(light.position - fragment_position);
	float attenuation = 1.0 / (light.constant + 
							light.linear * distance + 
							light.quadratic * (distance * distance)); 
							
	/* Sum components */
	return clamp(attenuation * intensity *(light_ambient + light_diffuse + light_specular), 0, 1);
}