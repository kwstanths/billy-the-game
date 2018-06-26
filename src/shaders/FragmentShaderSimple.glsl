#version 330 core

/* Interpolated values from the vertex shader */
in vec2 uv;

/* Fragment shader output */
out vec3 color;

/* Texture */
uniform sampler2D sampler_texture;

void main(){

    color = texture(sampler_texture, uv).rgb;

}