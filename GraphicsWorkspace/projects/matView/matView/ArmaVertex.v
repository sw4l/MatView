#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvcoords;	

out vec2 TexCoords;
out vec2 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	//flipping the y direction so textures arent upside down
    TexCoords = vec2(uvcoords.x, 1.0 - uvcoords.y );
	//pass the normals to the fragment shader
	normals = normal;
	//probably need to do some lighting calulations inside the vertex shader here
}


//if we let SOIL flip y we use the function below
	//TexCoords = uvcoords;