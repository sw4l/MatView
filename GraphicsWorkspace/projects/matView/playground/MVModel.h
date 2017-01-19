#pragma once
//libs
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL\SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MVMesh.h"


class MVModel {
public:
	//constructor
	MVModel(GLchar* path);
	void draw(Shader shader);
	//destructor
	~MVModel();
private:
	//model data
	std::vector<MVMesh> meshes;
	std::string currentPath;
	//functions
	void loadModel(std::string path);
	void proccessNode(aiNode* node, const aiScene* scene);
	MVMesh processMesh(aiMesh* mesh, const aiScene* scene);
	

};

