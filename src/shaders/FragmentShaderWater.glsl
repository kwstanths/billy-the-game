#version 410 core

out vec4 FragColor;

in TES_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} tes_out;

struct DirectionalLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirectionalLight directional_light;
uniform vec3 camera_world_position;
uniform sampler2D normal_map;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor);

void main()
{
    vec3 normal_worldspace = texture(normal_map, tes_out.uv).rgb;
    
	vec3 directional_light_color = CalculateDirectionalLight(directional_light, normal_worldspace, normalize(camera_world_position - tes_out.position_worldspace), vec3(0, 0.85, 0), 1, 0.3);
	
    FragColor = vec4(directional_light_color, 1);
}


vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor) {
    /* Ambient component */
	vec3 light_ambient = ambient_factor * light.ambient * fragment_color;
	
	/* Diffuse component */
	vec3 light_direction_inv = normalize(-light.direction); 
	float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
	vec3 light_diffuse = light.diffuse * light_diffuse_strength * fragment_color;
	
	/* Specular component */
	/* Find the reflected vector from the light towards the surface normal */
	vec3 light_reflect_vector = reflect(-light_direction_inv, fragment_normal);
	float light_specular_strength = pow(max(dot(view_direction, light_reflect_vector), 0.0), 2.0);
	vec3 light_specular = light.specular * light_specular_strength * fragment_specular_intensity;
       
	return light_ambient + (light_diffuse + light_specular);
}
