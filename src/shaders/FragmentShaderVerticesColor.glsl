#version 330 core

/* Fragment shader output */
layout(location = 0) out vec3 g_position;
layout(location = 1) out vec3 g_normal;
layout(location = 2) out vec4 g_albedo_spec;
layout(location = 3) out vec3 g_position_light;

/* Vertex color */
uniform vec3 fragment_color;

void main(){

    g_normal = vec3(0,0,1);
    g_albedo_spec = vec4(fragment_color, 1);
}