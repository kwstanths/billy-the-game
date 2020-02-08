#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;

out vec2 uv;
out vec3 normal_viewspace;
out vec3 fragment_position_viewspace;
out vec4 fragment_position_lightspace;

uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform mat4 matrix_lightspace;
uniform sampler2D displacement_texture;
uniform float displacement_mult;

vec4 CalcNormal(){

    vec2 texel_size = 1.0 / vec2(textureSize(displacement_texture, 0));
    
    float scale = 0.5;
    vec4 corner00 = vec4(-5, 0, -5, 1);
    vec4 corner01 = vec4(5, 0, -5, 1);
    vec4 corner10 = vec4(-5, 0, 5, 1);
    vec4 corner11 = vec4(5, 0, 5, 1);
    
    vec3 xm, xp, ym, yp;
    vec2 tc = vertex_uv + vec2(-texel_size.x * scale, 0);
    
    vec4 meh = mix(mix(corner00, corner01, tc.x), mix(corner10, corner11,tc.x), tc.y);
    xm = meh.xyz/meh.w;
    tc = vertex_uv + vec2(+texel_size.x * scale, 0);
    meh = mix(mix(corner00, corner01, tc.x), mix(corner10, corner11, tc.x), tc.y);
    xp = meh.xyz/meh.w;
    tc = vertex_uv + vec2(0, -texel_size.y * scale);
    meh = mix(mix(corner00, corner01, tc.x), mix(corner10, corner11, tc.x), tc.y);
    ym = meh.xyz/meh.w;
    tc = vertex_uv + vec2(0, texel_size.y * scale);
    meh = mix(mix(corner00, corner01, tc.x), mix(corner10, corner11, tc.x), tc.y);
    yp = meh.xyz/meh.w;
    
    xm.y = displacement_mult * texture(displacement_texture, vertex_uv + vec2(texel_size.x, 0)).r;
    xp.y = displacement_mult * texture(displacement_texture, vertex_uv - vec2(texel_size.x, 0)).r;
    ym.y = displacement_mult * texture(displacement_texture, vertex_uv + vec2(0, texel_size.y)).r;
    yp.y = displacement_mult * texture(displacement_texture, vertex_uv - vec2(0, texel_size.y)).r;
    
    vec3 normal = normalize(-cross(xp - xm, yp - ym));
    return vec4(0.5 + 0.5 * normal, 1);
}

void main(){

    vec4 world_position = matrix_model * vec4(vertex_position_modelspace, 1.0);
    world_position.y = world_position.y + displacement_mult * texture(displacement_texture, vertex_uv).r;

    /* Calculate the normal in viewspace */
    if (displacement_mult > 0){
        normal_viewspace = mat3(transpose(inverse(matrix_view))) * CalcNormal().xyz;
    } else {
        normal_viewspace = mat3(transpose(inverse(matrix_view * matrix_model))) * vertex_normal;
    }

    /* Calculate the position of the fragment in view space */
	fragment_position_viewspace = (matrix_view * world_position).xyz;
	uv = vertex_uv;
    
    /* Calculate light space position */
    fragment_position_lightspace = matrix_lightspace * world_position;
    
    /* Set output */
    gl_Position =  matrix_projection * matrix_view * world_position;
}