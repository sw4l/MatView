#pragma once
/*
were going to use this as a test materialLoading object
*/

//libs
#include<iostream>
#include <SOIL\SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include"../matView/Shader.h"

/*
	This object takes in a path to a texture in the constructor and loads the image into the OpenGL back end
	to render the texture we call drawTexture and pass it the Shader to use

	this method will take in the image and apply to the object loaded in 

*/


class MatLoad
{
private:
	int width, height;
	GLuint texture1;
	std::string currentPath;
	void loadImage(std::string path);
	
public:
	MatLoad(std::string path);
	void drawTexture(Shader ourShader);
	void updateImage(int time);
	//getters- will use these for the gui 
	int getHeight();
	int getWidth();
	~MatLoad();
};

