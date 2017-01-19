#include "MatLoad.h"
//constructor
MatLoad::MatLoad(std::string path){

	this->currentPath = path;
	this->loadImage(path);
}

void MatLoad::drawTexture(Shader ourShader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
}

void MatLoad::loadImage(std::string path) {
	glGenTextures(1, &this->texture1);
	glBindTexture(GL_TEXTURE_2D, this->texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.




}
//use this to re-init an image after so much time
void MatLoad::updateImage(int time) {
	if (time % 2 == 0) {
		this->loadImage(this->currentPath);
	}
}

//getters
int MatLoad::getHeight(){
	return this->height;
}

int MatLoad::getWidth(){
	return this->width;
}


//destuctor
MatLoad::~MatLoad() {

}



/*Legacy function
//take in a path and load the image file from the path
unsigned char* image = SOIL_load_image(path.c_str(), &this->width, &this->height, 0, SOIL_LOAD_RGB);
//creating a texture ID
GLuint textureID;
glGenTextures(1, &textureID);
/*
The glGenTextures function first takes as input how many textures we want to generate and stores them in a GLuint array given as its
second argument (in our case just a single GLuint).
Just like other objects we need to bind it so any subsequent texture commands will configure the currently bound texture:
*/
//glBindTexture(GL_TEXTURE_2D, textureID);
/*
Now that the texture is bound, we can start generating a texture using the previously loaded image data. Textures are generated with glTexImage2D
GltextImage2D
argument explaination
1.The first argument specifies the texture target; setting this to GL_TEXTURE_2D means this operation will generate a texture on the currently bound texture object at the same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).
2.The second argument specifies the mipmap level for which we want to create a texture for if you want to set each mipmap level manually, but we'll leave it at the base level which is 0.
3.The third argument tells OpenGL in what kind of format we want to store the texture. Our image has only RGB values so we'll store the texture with RGB values as well.
4.The 4th and 5th argument sets the width and height of the resulting texture. We stored those earlier when loading the image so we'll use the corresponding variables.
5.The next argument should always be 0 (some legacy stuff).
6.The 7th and 8th argument specify the format and datatype of the source image. We loaded the image with RGB values and stored them as chars (bytes) so we'll pass in the corresponding values.
7.The last argument is the actual image data.
*/
//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//generate mip maps on our texture buffer object
//glGenerateMipmap(GL_TEXTURE_2D);
//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//glEnableVertexAttribArray(2);
