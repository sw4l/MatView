#version 330 core

in vec2 TexCoords;
in vec2 normals
out vec4 color;

uniform sampler2D f_diffuse;

void main()
{    
    color = texture(f_diffuse, TexCoords);
}