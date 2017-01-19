#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvcoords;	
//we neet tangent and bit tangent to calculated the TBN matrix 
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitTangent;
 
out vec2 TexCoords;
//out vec3 Normals;
//out mat3 TBN;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    mat3 TBN;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//position the light in refrence to the vertex
uniform vec3 lightPos;
//position of the camera currently we pass it by using glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, &camera.Position[0]) in the main loop
uniform vec3 viewPos;



void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(position, 1.0));   
    vs_out.TexCoords = uvcoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.Normal = normalize(normalMatrix * normal);
    
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitTangent);
    vec3 N = normalize(normalMatrix * normal);
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TBN = TBN;
    
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
    
    vs_out.Tangent = tangent;
    vs_out.Bitangent = bitTangent;
}


// T = tangent, B = bitTangent, N = Normal

