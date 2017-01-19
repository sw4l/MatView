#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
/*
	may need a redesign to incooperate some better approaches to recompiling the shaders at run time so we can do fast testing 

*/



	class Shader
	{

	private:
		const GLchar *vertexPath;
		const GLchar *fragmentPath;
		//reads the shaders and returns them to be worked on
		std::string readShaders(const GLchar* pathToShader) {
			//retrieve the source code from the file path
			std::string shaderCode;
			std::ifstream shaderFile;
			//set stream but to throw exception just in case
			shaderFile.exceptions(std::ifstream::badbit);
			try {
				//open the shader file
				shaderFile.open(pathToShader);
				//stream the shader to a string 
				std::stringstream shaderStream;
				//transfer from the file stream to the string stream
				shaderStream << shaderFile.rdbuf();
				//close the file
				shaderFile.close();
				//set the read shader code for return
				shaderCode = shaderStream.str();


			}
			catch(std::ifstream::failure e){
				//if there is some failure with the opening of the file(such as wrong location ect) throw an error
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
				return NULL;
			}

			//return the read shader file code
			return shaderCode;
		}
		//returns true or false based on if the shaders will succesfully compiled
		bool compileShaders(const GLchar* vertexShaderCode, const GLchar* fragmentShaderCode) {
			GLuint vertex, fragment;
			GLint success;
			GLchar infoLog[512];
			// Vertex Shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertexShaderCode, NULL);
			glCompileShader(vertex);
			// Print compile errors if any
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertex, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				return false;
			}
			// Fragment Shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
			glCompileShader(fragment);
			// Print compile errors if any
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragment, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				return false;
			}
			// Shader Program
			this->Program = glCreateProgram();
			glAttachShader(this->Program, vertex);
			glAttachShader(this->Program, fragment);
			glLinkProgram(this->Program);
			// Print linking errors if any
			glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				return false;
			}
			// Delete the shaders as they're linked into our program now and no longer necessery
			glDeleteShader(vertex);
			glDeleteShader(fragment);

			//if no problems return true;
			return true;
		}



	public:
		GLuint Program;
		// Constructor generates the shader on the fly
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
			//set our paths of this instance of the shaders
			this->vertexPath = vertexPath;
			this->fragmentPath = fragmentPath;
			//take in the paths, read the paths and then pass to compile shader to link with openGL clean, effective
			compileShaders(readShaders(vertexPath).c_str(), readShaders(fragmentPath).c_str());
		}

		//recompile the current shader(we only handle 1 group of shaders/programs per shader object) 
		void recompileShaders() {
			//this should recompile the shaders with changes 
			bool done = compileShaders(readShaders(this->vertexPath).c_str(), readShaders(this->fragmentPath).c_str());
			if (!done) {
				std::cout << "Compilation was not succesful" << std::endl;
			}
			else {
				std::cout << "Recompiled" << std::endl;			}
		}



		// Uses the current shader
		void Use()
		{
			glUseProgram(this->Program);
		}

		
	};

#endif