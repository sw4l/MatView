#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include"Shader.h"

//this object takes in the vertices,normals,and UVcords of the initialized object 


namespace MV {

	class InitAsset {



	public:
		//constructor
		InitAsset() {

		}




	private:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;

	};



}