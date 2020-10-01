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

layout(location = 0) out vec4 FragColor;

in vec2 uv;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;
uniform sampler2D ssao_texture;
uniform sampler2D g_position_light;

uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform mat4 matrix_view_inverse;

uniform sampler2D shadow_map[3];
uniform mat4 matrix_lightspace[3];
uniform bool use_shadows;

#define NR_POINT_LIGHTS 36
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

float fragment_in_shadow;

/* Sample shadow map at position, perform PCF */
float ShadowCalculation(int shadow_map_index, vec4 fragment_position_lightspace) {
    fragment_position_lightspace.z = fragment_position_lightspace.z / fragment_position_lightspace.w;
    vec3 projCoords = fragment_position_lightspace.xyz;
    projCoords = projCoords * 0.5 + 0.5;
    
    float shadow = 0;
    if(projCoords.z > 1.0)
        return shadow;
        
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0 / textureSize(shadow_map[shadow_map_index], 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadow_map[shadow_map_index], projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - 0.002 > pcfDepth  ? 1.0f : 0.0;             
        }    
    }
    return shadow /= 9.0;
}

/* Set the depth of the fragment given it's position */
void FixDepth(){
    /* Get the fragment position in viewspace, transform it to clip space, and calculate depth */
    vec4 fragment_position_viewspace = vec4(texture(g_position, uv).xyz, 1);
    vec4 clip_space = matrix_projection * fragment_position_viewspace;
    float depth = clip_space.z / clip_space.w;
    
    /* Transform depth to OpenGL formula, and set it */
    float near = gl_DepthRange.near;
    float far = gl_DepthRange.far;
    float diff = gl_DepthRange.diff;
    gl_FragDepth = ((diff * depth) + near + far) * 0.5f;
}

void main() {
    
    vec3 fragment_position_viewspace = texture(g_position, uv).xyz;
    vec3 normal_viewspace = texture(g_normal, uv).rgb;
    
    vec3 fragment_color = texture(g_albedo_spec, uv).rgb;
    if (length(normal_viewspace) < 0.9){
        discard;
        return;
    }
    normalize(normal_viewspace);
    
    float fragment_specular_intensity = texture(g_albedo_spec, uv).a;
    
    if (use_shadows) {
        vec4 fragment_position_worldspace = matrix_view_inverse * vec4(fragment_position_viewspace, 1);
        float cascades[3];
        cascades[0] = 19.9;
        cascades[1] = 39.7;
        cascades[2] = 100;
        vec3 colors[3];
        colors[0] = vec3(1, 0.5, 0.5);
        colors[1] = vec3(0.5, 1, 0.5);
        colors[2] = vec3(0.5, 0.5, 1);
        
        for (int i = 0 ; i < 3 ; i++) {
            if (-fragment_position_viewspace.z <= cascades[i]) {
                fragment_color = colors[i];
                vec4 fragment_position_lightspace = matrix_lightspace[i] * fragment_position_worldspace;
                fragment_in_shadow = ShadowCalculation(i, fragment_position_lightspace);
                
                break;
            }
        }
    }else {
        fragment_in_shadow = 0;
    }
    
    float ambient_factor = texture(ssao_texture, uv).r;
    vec3 view_direction = normalize(-fragment_position_viewspace);


	/* Calculate directional light color contribution */
	vec3 directional_light_color = CalculateDirectionalLight(directional_light, normal_viewspace, view_direction, fragment_color, fragment_specular_intensity, ambient_factor);
	
	/* Calculate point lights color contribution */
	vec3 point_lights_color = vec3(0, 0, 0);
	for(uint i = 0u; i < NR_POINT_LIGHTS; i++){
        if (i < number_of_point_lights) 
            point_lights_color += CalculatePointLight(point_light[i], fragment_position_viewspace, normal_viewspace, view_direction, fragment_color, fragment_specular_intensity, ambient_factor);
    }
		
	/* Calculate casting light color contribution */
	vec3 cast_light_color = CalculateCastingLight(cast_light, fragment_position_viewspace, normal_viewspace, view_direction, fragment_color, fragment_specular_intensity, ambient_factor);
	
	/* Sum total components, the minimum color is zero, the maxium color possible the actual color of the fragment */
	FragColor = vec4(clamp(cast_light_color + point_lights_color + directional_light_color, vec3(0,0,0), fragment_color), 1);
    
    /* Fix the fragment's depth, since this is drawn using a quad that covers the screen */
    FixDepth();
}
   
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float ambient_factor) {
    light.direction = normalize(TransformToViewSpace(vec4(light.direction, 0)));
	vec3 light_direction_inv = -light.direction;
    vec3 half_way_direction = normalize(light_direction_inv + view_direction);
    
	/* Ambient component */
	vec3 light_ambient = ambient_factor * light.ambient * fragment_color;
	
	/* Diffuse component */
	float light_diffuse_strength = max(dot(fragment_normal, light_direction_inv), 0.0);
	vec3 light_diffuse = light.diffuse * light_diffuse_strength * fragment_color;
	
	/* Specular component */
	/* Find the reflected vector from the light towards the surface normal */
	float light_specular_strength = pow(max(dot(fragment_normal, half_way_direction), 0.0), 64.0);
	vec3 light_specular = light.specular * light_specular_strength * fragment_specular_intensity;

       //light_ambient = clamp(light_ambient, 0, 0);
       //light_diffuse = clamp(light_diffuse, 0, 0);
       //light_specular = clamp(light_specular, 0, 0);
       
	return light_ambient + (1 - fragment_in_shadow) * (light_diffuse + light_specular);
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
		
    return attenuation * (light_ambient + (light_diffuse + light_specular));
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
	return clamp(attenuation * intensity *(light_ambient * (light_diffuse + light_specular)), 0, 1);
}