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

struct Wave {
    vec3 direction;
    float wavelength;
    float amplitude;
};

in VS_OUT {
    vec2 uv;
    vec3 normal_viewspace;
    vec3 fragment_position_worldspace;
} fs_in;

uniform Material object_material;
uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform DirectionalLight directional_light;
uniform float time;
uniform sampler2D bump_texture;
uniform sampler2D depth_texture;

#define NR_WAVES 4
uniform Wave waves[NR_WAVES];
uniform uint number_of_waves;

vec3 TransformToViewSpace(vec4 vector){
	return (matrix_view * vector).xyz;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 fragment_normal, vec3 view_direction, vec3 fragment_color, float fragment_specular_intensity, float shininess, float ambient_factor);

vec3 CalculateNormal();
vec3 CalculateBumpNormal();
vec3 CalculateColor(vec3 view_direction, vec3 normal_viewspace);
float CalculateTransparency();

void main(){

	vec4 texture_color = texture(object_material.texture_diffuse, fs_in.uv);
	if (texture_color.a < 0.1) 
        discard;
	
    vec3 fragment_position_viewspace = (matrix_view * vec4(fs_in.fragment_position_worldspace, 1)).xyz;
    vec3 normal = CalculateBumpNormal();
    vec3 normal_viewspace = normalize(transpose(inverse(mat3(matrix_view * matrix_model))) * normal);
    
    float fragment_specular_intensity = texture(object_material.texture_specular, fs_in.uv).r + + object_material.specular.r;

    vec3 view_direction = normalize(-fragment_position_viewspace);
    vec3 fragment_color = CalculateColor(view_direction, normal_viewspace);
    
	/* Calculate directional light color contribution */
	vec3 directional_light_color = CalculateDirectionalLight(directional_light, normal_viewspace, view_direction, fragment_color, fragment_specular_intensity, object_material.shininess, 1);

    float alpha = CalculateTransparency();
    FragColor = vec4(directional_light_color, alpha);
}

float LinearizeDepth(float depth) 
{    
    float near = 0.1;
    float far = 600;
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

float CalculateTransparency(){
    float depth = texture(depth_texture, gl_FragCoord.xy / vec2(1920, 1080)).r;
    depth = LinearizeDepth(depth);
    
    float curent_depth = LinearizeDepth(gl_FragCoord.z);
    
    return max(abs(depth - curent_depth) / 3, 0.1);
}

vec3 CalculateColor(vec3 view_direction, vec3 normal_viewspace){
    float cos_angle = max(dot(view_direction, normal_viewspace), 0);

    vec3 shallow = vec3(0.16, 0.12, 0.7);
    vec3 deep = object_material.diffuse;
    vec3 water_color = deep * cos_angle + shallow * (1 - cos_angle);
    
    return water_color;
}

vec3 CalculateBumpNormal(){
    vec2 uv = fs_in.uv;
    
    vec4 t0 = 2 * texture(bump_texture, uv * 100 * 0.02 + 2 * 0.07 * vec2(1, 0) * (time / 10) * vec2(-1, -1)) -1;
    vec4 t1 = 2 * texture(bump_texture, uv * 100 * 0.03 + 2 * 0.057 * vec2(0.86, 0.5) * (time / 10) * vec2(1, -1)) -1;
    vec4 t2 = 2* texture(bump_texture, uv * 100 * 0.05 + 2 * 0.047 * vec2(0.86, -0.5) * (time / 10) * vec2(-1, 1)) -1;
    vec4 t3 = 2 *texture(bump_texture, uv * 100 * 0.07 + 2 * 0.037 * vec2(0.7, 0.7) * (time / 10)) -1;
    
    vec3 N = (t0 + t1 + t2 + t3).xzy;
    N.xz *= 4;
    N = normalize(N);
    return N;
}

/* Calculate the normal by using the waves themselves, not used */
vec3 CalculateNormal(){
    vec3 du = vec3(1, 0, 0);
    vec3 dv = vec3(0, 0, 1);
    
    for(uint i = 0u; i < NR_WAVES; i++){
        if (i < number_of_waves) {
            vec3 direction = waves[i].direction;
            float wavelength = waves[i].wavelength;
            float amp = waves[i].amplitude;
            
            float pdotk = dot(fs_in.fragment_position_worldspace, direction);
            float vel = sqrt(1.5621f * wavelength);
            float phase = (6.28f / wavelength) * (pdotk + vel * time);
            
            vec2 offset;
            offset.x = sin(phase);
            offset.y = cos(phase);
            offset.x *= - amp;
            offset.y *= - amp;
            
            vec3 da = vec3(direction.x * offset.y, offset.x, direction.z * offset.y);
            da *= 6.28 / wavelength;
            du += da * direction.x;
            dv += da * direction.z;
        }
    }

    return normalize(transpose(inverse(mat3(matrix_view * matrix_model))) * cross(dv, du));
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