#pragma once
//library includes
#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL\SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//our objects
#include"Shader.h" //shader compilation object
#include"AssetMesh.h" //mesh rendering object(probably needs a re-write)

//this object is a front end interface API for models and materials to go through, some methods will need to pass through a shader object

namespace MV {

	class AssetLoader {
	private:
		//paths to load material files
		std::string pathToDiffuseMap;
		std::string pathToSpecularMap;
		std::string pathToNormalMap;
		//center point of mesh
		glm::vec3 *centerOfMesh = NULL;
		//store all the meshes into this vector
		std::vector<AssetMesh> meshes;
		//our variables to send to openGL for the texture binding
		GLuint diffuse,specular,normal;
		//varibles to retrive map texture map size
		int diffuseHeight, diffuseWidth, specularHeight, specularWidth, normalHeight, normalWidth;
		//our private functions
		//load the model from the path into ASSIMPS data structures
		void loadModel(std::string path) {
			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);

			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return;
			}

			this->proccessNode(scene->mRootNode, scene);
		}
		//process all the nodes into the mesh vector
		void proccessNode(aiNode * node, const aiScene * scene) {
			// Process all the node's meshes (if any)
			for (GLuint i = 0; i < node->mNumMeshes; i++) {
				//std::cout << "We process " << i + 1 << " mesh(es)" << std::endl;
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				this->meshes.push_back(this->processMesh(mesh, scene));
			}
			// Then do the same for each of its children
			for (GLuint i = 0; i < node->mNumChildren; i++) {
				//std::cout << "We have " << i + 1 << " child(ren)" << std::endl;
				this->proccessNode(node->mChildren[i], scene);
			}

		}

		//process all the mesh loading into the meshes vector
		AssetMesh processMesh(aiMesh * mesh, const aiScene * scene) {
			// Data to fill
			std::vector<vertex> vertices;
			std::vector<GLuint> indices;
			//find the center of the mesh
			findCenterOfMesh(mesh);

			// Walk through each of the mesh's vertices
			for (GLuint i = 0; i < mesh->mNumVertices; i++) {
				vertex vertex;
				glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
								  // Positions
				vector.x = mesh->mVertices[i].x;
				//because 3ds max is Z up, the Y and Z components must be flipped(for now) 
				vector.y = mesh->mVertices[i].z;
				vector.z = mesh->mVertices[i].y;
				vertex.position = vector;
				// Normals
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].z;
				vector.z = mesh->mNormals[i].y;
				vertex.normal = vector;
				//Tangent normal vectors
				//there is an issue loading the tangents
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].z;
				vector.z = mesh->mTangents[i].y;
				vertex.tangent = vector;
				//bitTangent normal vectors
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].z;
				vector.z = mesh->mBitangents[i].y;
				vertex.bitTangent = vector;
				// Texture Coordinates
				if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
				{
					//cout << " we copied " << i + 1 << " uvs over" << endl;
					glm::vec2 vec;
					// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.uvcoords = vec;
				} else {
					//default set the uvcoords to 0
					vertex.uvcoords = glm::vec2(0.0f, 0.0f);
				}
				vertices.push_back(vertex);
			}
			// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (GLuint i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				// Retrieve all indices of the face and store them in the indices vector
				for (GLuint j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			//below here is where we initialize out materials
			//TODO figure out a way to pass through the textures we need to load when loaded
			return AssetMesh(vertices, indices);
		}

		//this checks to make sure we dont try to run a tga and crash the program
		int checkFileExt(std::string path) {
			if (path[path.size() - 3] == 't' || path[path.size() - 3] == 'T') {
				cout << "its a tga" << endl;
				return -1;
			}	
			return 0;
		}
		
		//we will run this private function to find the center of the mesh(avged)
		//nothing in, finds the center of the mesh(set it here)
		void findCenterOfMesh(aiMesh * mesh) {
			//step through all the verices
			glm::vec3 sumVector;
			for (GLuint i = 0; i < mesh->mNumVertices; i++) {
				//sum up all the position in the mesh 
				sumVector.x += mesh->mVertices[i].x;
				sumVector.y += mesh->mVertices[i].y;
				sumVector.z += mesh->mVertices[i].z;
			}
			//calculate avg position of each componet
			sumVector.x = (sumVector.x)/(mesh->mNumVertices);
			sumVector.y = (sumVector.y) / (mesh->mNumVertices);
			sumVector.z = (sumVector.z) / (mesh->mNumVertices);
			std::cout << "Avg Center of this mesh is ( " << sumVector.x << " , " << sumVector.y << " , " << sumVector.z << " )" << std::endl;
			this->centerOfMesh = &sumVector;
 		}

	public:
		//constructor
		AssetLoader(std::string pathToModel) {
			//doing this to make the constructor more clean
			this->loadModel(pathToModel);
		}
		
		//exposing load model for the lights loading
		void loadLightModel(std::string path) {
			this->loadModel(path);
		}
		
#pragma region "Mutators/Setters"
		/*These functions update the image height and width for our gui later*/
		//setters
		void setDiffuseHeight(int height) {
			this->diffuseHeight = height;
		}
		void setSpecularHeight(int height) {
			this->specularHeight = height;
		}
		void setNormalHeight(int height) {
			this->normalHeight = height;
		}
		void setDiffuseWidth(int width) {
			this->diffuseWidth = width;
		}
		void setSpecularWidth(int width) {
			this->specularWidth = width;
		}
		void setNormalWidth(int width) {
			this->normalWidth = width;
		}
		//get each image height and width 
		int getDiffuseHeight() {
			return this->diffuseHeight;
		}
		int getDiffuseWidth() {
			return this->diffuseWidth;
		}
		int getSpecularHeight() {
			return this->specularHeight;
		}
		int getSpecularWidth() {
			return this->specularWidth;
		}
		int getNormalHeight() {
			return this->normalHeight;
		}
		int getNormalWidth() {
			return this->normalHeight;
		}
		//get the center of the mesh
		//if the center of the mesh has been calculated return it else return center of world space
		glm::vec3 getCenterOfMesh() {
			if (this->centerOfMesh != NULL) {
				return *this->centerOfMesh;
			}else{
				std::cout << "Error, CenterOfMesh has no value" << std::endl;
				return glm::vec3(0,0,0);
			}
		}

#pragma endregion
		//load our textures
		//we will use these as public to initialize the textures into openGL

		void initDiffuse(std::string path) {
		//	this->debugDiffuseInitTest(path);
			if (checkFileExt(path)) {
				cout << "cannot load this file type" << endl;
				return;
			}else {
				this->initMaterialMap(path, this->diffuse);
			}
		}

		void initSpecular(std::string path) {
			if (checkFileExt(path)) {
				cout << "cannot load this file type" << endl;
				return;
			}
			else {
				this->initMaterialMap(path, this->specular);
			}
			
		}

		void initNormal(std::string path) {
			if (checkFileExt(path)) {
				cout << "cannot load this file type" << endl;
				return;
			}
			else {
				this->initMaterialMap(path, this->normal);
			}

		}

		//we will can load material and call initMap then pass the type through
		// must pass the type by address to add it to the buffer correctly
		void initMaterialMap(std::string path, GLuint &type) {
			if (&type == &this->diffuse) {
				std::cout << "We are loading a diffuse map" << endl;
			}else if (&type == &this->specular) {
				std::cout << "We are loading a specular map" << endl;
			}else if (&type == &this->normal) {
				std::cout << "We are loading a Normal map" << endl;
			}else {
				std::cout << "We have some kind of type mismatch::Not a Diffuse,specular, or normal map" << endl;
			}

			glGenTextures(1, &type);
			glBindTexture(GL_TEXTURE_2D, type); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
														 // Set our texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Set texture filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Load, create texture and generate mipmaps
			int width, height;
			cout << path.c_str() << endl;
			unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			//check to see if the image importing was successful	
			if (image == NULL) {
				std::cout << "An error occurred while loading the image ";
				std::cout << SOIL_last_result() << std::endl;
				return;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

		}

		//draw our model onto the screen
		void draw(Shader shader) {
			//draw our materials
			this->drawMaterials(shader);
			//Draw our mesh
			for (GLuint i = 0; i < this->meshes.size(); i++) {
				this->meshes[i].Draw(shader);
			}
		}

		//draw all of our materials and pass to the shaders
		void drawMaterials(Shader shader) {
			
			//these will get sent and verified to the fragment shader for sampler2d types to be used inside the shader
			//the f_ tag is used to denote that it will be inside the .frag file
			//set up diffuse
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->diffuse);
			glUniform1i(glGetUniformLocation(shader.Program,"f_diffuse"),0);
			//normals
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->normal);
			glUniform1i(glGetUniformLocation(shader.Program, "f_normal"), 1);
			//specular
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, this->specular);
			glUniform1i(glGetUniformLocation(shader.Program, "f_specular"), 2);
		}




		//destuctor
		~AssetLoader() {

		}
	};

}

