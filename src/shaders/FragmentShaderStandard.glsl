#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
    float shininess;
};

struct DirectionalLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT {
    vec2 uv;
    vec3 normal_viewspace;
    vec3 fragment_position_viewspace;
} fs_in;

uniform Material object_material;
uniform mat4 matrix_view;
uniform DirectionalLight directional_light;

vec3 TransformToViewSpace(vec4 vector){
	return (matrix_view * vector).xyz;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float shininess, float ambient_factor);

void main(){

	vec4 texture_color = texture(object_material.texture_diffuse, fs_in.uv);
    vec3 fragment_position_viewspace = fs_in.fragment_position_viewspace;
    vec3 normal_viewspace = normalize(fs_in.normal_viewspace);
    vec3 fragment_color = texture_color.rgb + object_material.diffuse;
    float fragment_specular_intensity = texture(object_material.texture_specular, fs_in.uv).r + + object_material.specular.r;

    vec3 view_direction = normalize(-fragment_position_viewspace);

	/* Calculate directional light color contribution */
	vec3 directional_light_color = CalculateDirectionalLight(directional_light, normal_viewspace, view_direction, fragment_color, fragment_specular_intensity, object_material.shininess, 1);

	/* Sum total components, the minimum color is zero, the maxium color possible the actual color of the fragment */
	FragColor = vec4(clamp(directional_light_color, vec3(0,0,0), fragment_color), 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float shininess, float ambient_factor) {
    light.direction = normalize(TransformToViewSpace(vec4(light.direction, 0)));
	vec3 light_direction_inv = -light.direction;
    vec3 half_way_direction = normalize(light_direction_inv + view_direction);
    
	/* Ambient component */
	vec3 light_ambient = ambient_factor * object_material.ambient * light.ambient;
	
	/* Diffuse component */
	float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
	vec3 light_diffuse = light.diffuse * light_diffuse_strength * fragment_color;
	
	/* Specular component */
	/* Find the reflected vector from the light towards the surface normal */
	float light_specular_strength = pow(max(dot(fragment_normal, half_way_direction), 0.0), shininess);
	vec3 light_specular = light.specular * light_specular_strength * fragment_specular_intensity;

       //light_ambient = clamp(light_ambient, 0, 0);
       //light_diffuse = clamp(light_diffuse, 0, 0);
       //light_specular = clamp(light_specular, 0, 0);
       
	return light_ambient + light_diffuse + light_specular;
}