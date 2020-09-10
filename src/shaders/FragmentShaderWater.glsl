#version 410 core

out vec4 FragColor;

in TES_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} tes_out;

void main()
{
    FragColor = vec4(tes_out.position_worldspace / 4, 1);   
}