#version 410 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in TES_OUT {
    vec2 uv;
    vec3 position_worldspace;
} tes_out[3];

const float MAGNITUDE = 0.2;
 
uniform mat4 matrix_model;
uniform mat4 matrix_projection;
uniform mat4 matrix_view;
uniform sampler2D displacement_map;
uniform float displacement_intensity;

vec3 CalculateNormalFromHeightmap(int index){
    vec2 ts = 1.0 / vec2(textureSize(displacement_map, 0));
    float bot = texture(displacement_map, tes_out[index].uv + vec2(0, ts.y)).r * displacement_intensity;
    float top = texture(displacement_map, tes_out[index].uv + vec2(0, -ts.y)).r * displacement_intensity;
    float left = texture(displacement_map, tes_out[index].uv + vec2(-ts.x, 0)).r * displacement_intensity;
    float right = texture(displacement_map, tes_out[index].uv + vec2(ts.x, 0)).r * displacement_intensity;
    
    return vec3((left - right) / 2, 0.7, (top - bot) / 2);
}

mat4 vp = matrix_projection * matrix_view;
mat3 normalMatrix = mat3(transpose(inverse(matrix_model)));

/* Generate a line that goes from the vertex towards a point on the normal of that triangle */
void GenerateLine(int index)
{
    vec3 vertex_normal = normalize(CalculateNormalFromHeightmap(index));
    vertex_normal = normalize(normalMatrix * vertex_normal);

    gl_Position = vp * gl_in[index].gl_Position;
    EmitVertex();
    
    vec4 ptOnNormal = gl_in[index].gl_Position + 
        + vec4(vertex_normal, 0) * MAGNITUDE;

    gl_Position = vp * ptOnNormal;
    
    EmitVertex();
    EndPrimitive();
}

void main()
{    
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}  