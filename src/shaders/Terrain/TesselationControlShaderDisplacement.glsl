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

float GetTessLevel(float Distance0, float Distance1)                                            
{         
    float AvgDistance = (Distance0 + Distance1) / 2.0;                                          
    
    if (AvgDistance <= 40.0) {
        return 64.0;
    }
    else if (AvgDistance <= 80.0) {
        return 32.0;
    }
    else if (AvgDistance <= 100.0){
        return 16.0;
    }else if (AvgDistance <= 130) {
        return 8.0;
    }else {
        return 4.0;
    }
}

void main()                                                                                     
{                                                                                               
    // Set the control points of the output patch                                               
    tcs_out[gl_InvocationID].uv = tcs_in[gl_InvocationID].uv;
    tcs_out[gl_InvocationID].normal_worldspace = tcs_in[gl_InvocationID].normal_worldspace;
    tcs_out[gl_InvocationID].position_worldspace = tcs_in[gl_InvocationID].position_worldspace;
    
    float EyeToVertexDistance0 = distance(camera_world_position, tcs_in[0].position_worldspace);
    float EyeToVertexDistance1 = distance(camera_world_position, tcs_in[1].position_worldspace);
    float EyeToVertexDistance2 = distance(camera_world_position, tcs_in[2].position_worldspace);
                                                                                                
    // Calculate the tessellation levels                                                        
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);            
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);            
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);            
    gl_TessLevelInner[0] = gl_TessLevelOuter[0];
}                                                                                               