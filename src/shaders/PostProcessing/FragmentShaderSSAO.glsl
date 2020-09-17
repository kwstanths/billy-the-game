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

uniform float bias = 0.025;
vec2 noise_scale = vec2(textureSize(g_position, 0).x / textureSize(noise_texture, 0).x, textureSize(g_position, 0).y / textureSize(noise_texture, 0).y);

void main() {

    /* Grab the fragment position */
    vec3 fragment_position_viewspace = texture(g_position, uv).xyz;
    /* Grab the normal */
    vec3 normal_viewspace = normalize(texture(g_normal, uv).rgb);
    /* Grab the noise vector for this fragment. texture coordinates are scaled based on the size of the noise texture */
    vec3 noise_vector = normalize(texture(noise_texture, uv * noise_scale).xyz);
    
    float occlusion = 0.0;
    /* For all samples */
    for(int i = 0; i < samples_size; ++i) {
        
        /* Get a random direction inside a sphere around the fragment */
        vec3 random_direction = samples[i];
        /* Add some noise to the direction */
        random_direction = reflect(random_direction, noise_vector);
        /* If the random direction has different direction with the normal, then flip it to avoid self occlusion */
        if (dot(random_direction, normal_viewspace) < 0) {
           random_direction = - random_direction;
        }
        
        /* Get the random sample in view space */
        vec3 sample = fragment_position_viewspace + random_direction * radius; 
        
        /* Transform the random sample into clip-space, in order to sample the depth at that position */
        vec4 offset = vec4(sample, 1.0);
        offset = matrix_projection * offset;
        /* Do perspective division */
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        
        /* If it lies outside the screen, dont consider it for occlusion */
        if (abs(offset.x) > 1 || abs(offset.y) > 1 || abs(offset.z) > 1) continue;
        
        /* Sample the depth at the position of the random sample */
        float sample_depth = texture(g_position, offset.xy).z;
        
        /* 
            The larger the difference is between the sample depth, and the depth of that fragment, the smaller
            the contribution. If it lies above the radius limit, the contribution is zero
        */
        float range_check = smoothstep(0.0, 1.0, radius / abs(fragment_position_viewspace.z - sample_depth));
        /* 
            If the depth of the fragment stored at the position of the random sample is greater than the 
            z value of the random sample, that means the fragment at that position is in front of our sample,
            and thus it contributes to occlusion
        */
        occlusion += (sample_depth >= sample.z + bias ? intensity : 0.0) * range_check;
    }
    
    /* Invert the occlusion factor, so that we can use to "add" ambient light */
    /* If intensity is well above 1.0, this will end up removing light! */
    occlusion = 1.0 - (occlusion / samples_size);
    
    FragColor = occlusion;
}