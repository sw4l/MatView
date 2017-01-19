#version 330 core


//output of the color
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    mat3 TBN;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;


//diffuse map passed in
uniform sampler2D f_diffuse;
//normal map passed in
uniform sampler2D f_normal;
//specular map passed in
uniform sampler2D f_specular;

void main()
{    
	//normal cal
	vec3 normal = fs_in.Normal;
    mat3 tbn;
    // Obtain normal from normal map in range [0,1]
    normal = texture(f_normal, fs_in.TexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);   
    // Then transform normal in tangent space to world-space via TBN matrix
     tbn = mat3(fs_in.Tangent, fs_in.Bitangent, fs_in.Normal); // TBN calculated in fragment shader
    // normal = normalize(tbn * normal); // This works!
    normal = normalize(fs_in.TBN * normal); // This gives incorrect results
    // Get diffuse color
    vec3 color = texture(f_diffuse, fs_in.TexCoords).rgb;
    // Ambient
    vec3 ambient = 0.1 * color;
    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(diffuse, 1.0f);
}

