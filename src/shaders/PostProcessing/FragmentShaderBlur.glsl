#version 330 core

layout(location = 0) out float FragColor;

in vec2 uv;

uniform sampler2D sampler_texture;
uniform int blur_kernel_size = 2;

void main() {

    vec2 texel_size = 1.0 / vec2(textureSize(sampler_texture, 0));
    float result = 0.0;
    vec2 hlim = vec2(float(-blur_kernel_size) * 0.5 + 0.5);
    for (int x = 0; x < blur_kernel_size; ++x)  {
        for (int y = 0; y < blur_kernel_size; ++y)  {
            vec2 offset = (hlim + vec2(float(x), float(y))) * texel_size;
            result += texture(sampler_texture, uv + offset).r;
        }
    }
    FragColor = result / (blur_kernel_size * blur_kernel_size);
}