#version 330 core
out vec3 FragColor;

in vec2 TexCoords;
uniform sampler2D sampler_texture;
uniform bool red_component = false;

void main()
{             
    if (red_component){
        float temp1 = texture(sampler_texture, TexCoords).r;
        FragColor = vec3(temp1, temp1, temp1);
    }else{
        FragColor = texture(sampler_texture, TexCoords).rgb;
    }
    
}