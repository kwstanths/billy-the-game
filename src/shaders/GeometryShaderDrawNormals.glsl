#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.3;
 
uniform mat4 matrix_model;
uniform mat4 matrix_projection;
uniform mat4 matrix_view;

mat4 mvp = matrix_projection * matrix_view * matrix_model;
mat3 normalMatrix = mat3(transpose(inverse(matrix_model)));

/* Generate a line that goes from the vertex towards a point on the normal of that triangle */
void GenerateLine(int index)
{
    gl_Position = mvp * gl_in[index].gl_Position;
    EmitVertex();

    vec4 ptOnNormal = gl_in[index].gl_Position + 
        + vec4(gs_in[index].normal, 0) * MAGNITUDE;

    gl_Position = mvp * ptOnNormal;
    
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}  