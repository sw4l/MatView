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
#include"../matView/Shader.h"
//our definition of a vertex object
struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvcoords;
};

struct texture {
	GLuint id;
	std::string type;

};

class MVMesh {
public:
	//mesh data
	std::vector<vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<texture> textures;
	//our contructor
	MVMesh(std::vector<vertex> verts,std::vector<GLuint> indices,std::vector<texture> textures);
	//used in render loop for drawing
	void Draw(Shader shader);
	//destructor
	~MVMesh();
private:
	//Render data for OpenGL
	GLuint VAO, VBO, EBO;
	void setupMesh();

};

