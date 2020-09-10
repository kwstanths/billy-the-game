#version 410 core                                                                               
                                                                                                
// define the number of CPs in the output patch                                                 
layout (vertices = 3) out;                                                                      
                                                                                                
uniform vec3 camera_world_position;                                                                  

in VS_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} tcs_in[]; 

out TCS_OUT {
    vec2 uv;
    vec3 normal_worldspace;
    vec3 position_worldspace;
} tcs_out[];

float MAXIMUM_TESSELATION = 32.0;
float MINIMUM_TESSELATION = 2.0;
float MAXIMUM_DISTANCE = 80.0;
float MINIMUM_DISTANCE = 1.0;



float GetTessLevel(float Distance0)                                            
{         
    if (Distance0 <= 30.0) {
        return 32.0;
    }
    else if (Distance0 <= 40.0) {
        return 16.0;
    }
    else if (Distance0 <= 50.0){
        return 4.0;
    }else {
        return 2.0;
    }
}

void main()                                                                                     
{                                                                                               
    // Set the control points of the output patch                                               
    tcs_out[gl_InvocationID].uv = tcs_in[gl_InvocationID].uv;
    tcs_out[gl_InvocationID].normal_worldspace = tcs_in[gl_InvocationID].normal_worldspace;
    tcs_out[gl_InvocationID].position_worldspace = tcs_in[gl_InvocationID].position_worldspace;
    
    vec3 edge0midpoint = (tcs_out[1].position_worldspace + tcs_out[2].position_worldspace)/2;
    vec3 edge1midpoint = (tcs_out[0].position_worldspace + tcs_out[2].position_worldspace)/2;
    vec3 edge2midpoint = (tcs_out[0].position_worldspace + tcs_out[1].position_worldspace)/2;
    
    float EyeToVertexDistance0 = distance(camera_world_position, edge0midpoint);
    float EyeToVertexDistance1 = distance(camera_world_position, edge1midpoint);
    float EyeToVertexDistance2 = distance(camera_world_position, edge2midpoint);
                                                                                                
    // Calculate the tessellation levels                                                     
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance0);            
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance1);            
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance2);            
    gl_TessLevelInner[0] = gl_TessLevelOuter[0];
}                                                                                               