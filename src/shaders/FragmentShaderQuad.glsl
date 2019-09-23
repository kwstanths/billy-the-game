#version 330 core
out vec3 FragColor;

in vec2 TexCoords;
uniform sampler2D sampler_texture;
uniform bool red_component = false;

float near_plane = 0.1f;
float far_plane = 16.5f;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{             
    if (red_component){
        float temp1 = texture(sampler_texture, TexCoords).r;
        FragColor = vec3(temp1, temp1, temp1);
    }else{
        FragColor = texture(sampler_texture, TexCoords).rgb;
    }
    
}