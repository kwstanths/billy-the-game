#version 330 core

layout(location = 0) out vec3 FragColor;

in vec2 uv;

uniform sampler2D u_texture;
uniform vec2 rand_pos;

bool Compare(float x, float y, float margin){
    if (abs(x-y) > margin) return false;
    return true;
}

void main() {

    vec2 texel_size = 1.0 / vec2(textureSize(u_texture, 0));

    float uij = texture(u_texture, uv).r;
    
    if (rand_pos.x > 0 && rand_pos.y > 0 && Compare(uv.x, rand_pos.x, 0.01) && Compare(uv.y, rand_pos.y, 0.01)) {
        FragColor.r = uij - 60;
        FragColor.g = 0;
        return;
    }
    
    float uimj = texture(u_texture, uv - vec2(texel_size.x, 0)).r;
    float uipj = texture(u_texture, uv + vec2(texel_size.x, 0)).r;
    float uijm = texture(u_texture, uv - vec2(0, texel_size.y)).r;
    float uijp = texture(u_texture, uv + vec2(0, texel_size.y)).r;
    float vij = texture(u_texture, uv).g;

    vij = vij + (uimj + uipj + uijm + uijp)/4 - uij;
    vij = vij * 0.99;

    FragColor.r = (uij + vij);
    FragColor.g = vij;
}