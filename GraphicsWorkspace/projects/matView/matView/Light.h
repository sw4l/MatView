#pragma once
//libs
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <iostream>


//our includes
#include"AssetLoader.h"
#include"Shader.h"



//tucking this into our namespace
namespace MV {

	/*
	this object will be resposible for spawning and setting the light positions
	we will spawn the lights using AssetLoader Object
	we will use a modified CameraArcball object to rotate the light
	we will use the same shader program as we use on the main object being loaded
	this is entirely on the back end so we shouldnt really expose anything to the user/programmer
	*** we need to write a light shader to encapsulate everything into a a single one liner in the main
	*/

	class Light {
	private:

		void initLights() {
			//set light position
			GLfloat lightpos[] = { 1.0, 1.0, 1.0, 1.0 };
			//initialize the light within openGL
			glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
		}
	public:
		//default constructor
		Light() {
			this->initLights();
		}
		



		//destructor
		~Light() {

		}
		












	};

}