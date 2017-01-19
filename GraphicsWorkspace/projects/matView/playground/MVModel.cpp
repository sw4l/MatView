#include "MVModel.h"


MVModel::MVModel(GLchar * path) {
	this->loadModel(path);
}

void MVModel::draw(Shader shader) {
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shader);
}



void MVModel::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	
	this->proccessNode(scene->mRootNode, scene);

}

void MVModel::proccessNode(aiNode * node, const aiScene * scene) {

	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		std::cout << "We process " << i + 1 << " mesh(es)" << std::endl;
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		std::cout << "We have " << i + 1 << " child(ren)" << std::endl;
		this->proccessNode(node->mChildren[i], scene);
	}

}

MVMesh MVModel::processMesh(aiMesh * mesh, const aiScene * scene) {
	
	// Data to fill
	std::vector<vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<texture> textures;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		vertex vertex;
		glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		std::cout << "Vertex " << i + 1 << " = " << vector.x << "," << vector.z << "," << vector.y << "," << std::endl;
		std::cout << sizeof(vector) << std::endl;
 		vertex.position = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uvcoords = vec;
		} else {
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







	return MVMesh(vertices, indices,textures);

}


//destructor
MVModel::~MVModel() {
}