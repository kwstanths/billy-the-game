#version 330 core

layout(location = 0) out float FragColor;

in vec2 uv;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D noise_texture;

uniform vec3 samples[128];
uniform int samples_size;
uniform float radius;
uniform float intensity = 1.0;

uniform mat4 matrix_projection;

uniform float bias = 0.0625;
vec2 noise_scale = vec2(textureSize(g_position, 0).x / textureSize(noise_texture, 0).x, textureSize(g_position, 0).y / textureSize(noise_texture, 0).y);

void main() {
    
    /* Grab the fragment position */
    vec3 fragment_position_viewspace = texture(g_position, uv).xyz;
    /* Grab the normal */
    vec3 normal_viewspace = normalize(texture(g_normal, uv).rgb);
    
    /* Grab the noise vector for this fragment. texture coordinates are 
       scaled based on the size of the noise texture. If we multiply with 
       zero here, then no noise is applied. Pure horizontal and vertical 
       direcitons in viewspace 
    */
    float noise_angle = texture(noise_texture, uv * noise_scale).x * 3.14;

    /* Use the noise angle to rotate the axes vectors */
    vec3 noise_vector_horizontal = normalize(vec3(cos(noise_angle), sin(noise_angle), 0));
    vec3 noise_vector_vertical = normalize(vec3(sin(noise_angle), cos(noise_angle), 0));
    
    float occlusion = 0.0;
    for(int i = 0; i < samples_size; ++i) {
    
        /* Sample in the horizontal direction */
        vec3 sample_direction_h = noise_vector_horizontal * samples[i].x;
        /* Flip, if falls inside the geometry */
        if (dot(normalize(sample_direction_h), normal_viewspace) < 0) {
            sample_direction_h = -sample_direction_h;
        } 
        /* Get the sample in view space */        
        vec3 sample_h = fragment_position_viewspace + sample_direction_h * radius;
        
        /* Do the same for the vertical axis */
        vec3 sample_direction_v = noise_vector_vertical * samples[i].y;
        if (dot(normalize(sample_direction_v), normal_viewspace) < 0) {
            sample_direction_v = -sample_direction_v;
        } 
        vec3 sample_v = fragment_position_viewspace + sample_direction_v * radius;
        
        /* Transform the samples to clip space */
        vec4 offset_h = vec4(sample_h, 1.0);
        offset_h = matrix_projection * offset_h;
        offset_h.xyz /= offset_h.w;
        offset_h.xyz = offset_h.xyz * 0.5 + 0.5;
        
        vec4 offset_v = vec4(sample_v, 1.0);
        offset_v = matrix_projection * offset_v;
        offset_v.xyz /= offset_v.w;
        offset_v.xyz = offset_v.xyz * 0.5 + 0.5;
        
        /* Sample the depth at that position */
        float sample_depth_h = texture(g_position, offset_h.xy).z;
        float sample_depth_v = texture(g_position, offset_v.xy).z;
        
        /* Pass them through to occlusion function */
        float range_check_h = smoothstep(0.0, 1.0, radius / abs(fragment_position_viewspace.z - sample_depth_h));
        occlusion += (sample_depth_h >= sample_h.z + bias ? intensity : 0.0) * range_check_h / 2;
        
        float range_check_v = smoothstep(0.0, 1.0, radius / abs(fragment_position_viewspace.z - sample_depth_v));
        occlusion += (sample_depth_v >= sample_v.z + bias ? intensity : 0.0) * range_check_v / 2;
    }
    
    /* If intensity is very high, this will end up removing light */
    occlusion = (1 - occlusion / samples_size);
    
    FragColor = occlusion;
}