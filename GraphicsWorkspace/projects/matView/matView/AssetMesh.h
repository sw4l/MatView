#pragma once
#pragma once
//this object will be passed vertices, uv coords and normals
//libs
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




/*
	this object handels all of the mesh commands to push to OpenGL we need to encapsulate this to help with drawing based on our implementation of the 
	assetLoader object(i think its easier to encasulate this part for recursive calls)
*/

//defining a structured data type to encapsulate all of our vertex positions data together

struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvcoords;
	glm::vec3 tangent;
	glm::vec3 bitTangent;
};


namespace MV {

	class AssetMesh {
	public:
		//mesh data
		std::vector<vertex> vertices;
		std::vector<GLuint> indices;
		
		//constructor
		AssetMesh(std::vector<vertex> verts, std::vector<GLuint> indices) {
			//we pass the verices and indicies of the verts to this object and initialize it for setupmesh function
			this->vertices = verts;
			this->indices = indices;
			this->setupMesh();
		}

		void Draw(Shader shader) {
			glBindVertexArray(this->VAO);
			glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		//destructor
		AssetMesh() {
			//nothing to rlease really 
		}

	private:
		//Render data for OpenGL
		GLuint VAO, VBO, EBO;
		//sets up our mesh and readies it for drawing
		void setupMesh() {
			// Create buffers/arrays
			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &this->VBO);
			glGenBuffers(1, &this->EBO);

			glBindVertexArray(this->VAO);
			// Load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(vertex), &this->vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
			//vertex attrbute array pins these to the (location = n) varibles in the vertex shader
			//TODO figure out what i need to pass for specular
			// Set the vertex attribute pointers
			// Vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
			// Vertex Normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));
			// Vertex Texture Coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, uvcoords));
			//tangents we populated in assetLoader
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, tangent));
			//bitTangents we populated in AssetLoader
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, bitTangent));

			//unbind our VAO structure 
			glBindVertexArray(0);
		}

	};








}