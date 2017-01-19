#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"



class ModelOperations
{
private:




public:
	//vars
	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Texture> textures_loaded;	
	vector<Mesh> meshes;
	string directory;
	string pathToNormal;
	string pathToDiffuse;
	string pathToSpecular;
	bool gammaCorrection;
	//private functions
	//inits model copying over values from assimp
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	//this loads our texture from the file
	GLint TextureFromFile(const char* path);
	//public functions
	//constructor
	ModelOperations(string const & path);
	void Draw(Shader shader);
	//destructor
	~ModelOperations();
};

