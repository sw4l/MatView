// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// my objects includes
#include "Shader.h"//for out shaders
#include "CameraArcball.h"//our camera object
#include"AssetLoader.h"//Asset loading object
#include"Light.h"//light object
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
// Properties
GLuint screenWidth = 1024, screenHeight = 768;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// Camera
CameraArcball camera(glm::vec3(0.0f, 0.0f, 10.0f));
// Light position :: temp
//goal, setup a light rotation object
//rotation began 5 units straight up(y up coord system)
glm::vec3 lightPos1(0.0f, 0.0f, 0.0f);
//set up a light position at the defined initial point
CameraArcball lightROT1(lightPos1);


bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
//recompile flag variable
bool recompile = false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

const int mouseRotationHotKey = GLFW_KEY_LEFT_ALT;
const int mouseRot = GLFW_MOUSE_BUTTON_RIGHT;
bool canRotateCamera = false;//if we set this to true we can rotate our view
bool canRotateLight = false;
//temp vars until we expose it to a gui
string pathToModel = "../../common/models/matTestBox.obj", pathToDiffuse, pathToNormals, pathToSpeculat;

//global scroll offset vars
glm::vec2 totalScrollOffset(0.0f, 0.0f);



// The MAIN function, from here we start our application and run the Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "MatView alpha v0.1", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetErrorCallback(error_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();
	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader shader("vertex.vert", "fragment.frag");

	// Load model
	//initialize our model
	MV::AssetLoader ourModel("../../common/models/plane.obj");
	//initialize our diffuse texture
	ourModel.initDiffuse("../../common/textures/wallDiffuse.png");
	//init normals
	ourModel.initNormal("../../common/textures/ballnormal.png");
	//init the specular into openGL
	ourModel.initSpecular("../../common/textures/ballspec.png");
	//set up our look at center
	//need to change some implementation or figure out how to defrence this 
	lightROT1.setTargetLookAtPosition(&ourModel.getCenterOfMesh());
	//debug line
	bool debug = false;


	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		//checking if the shader is requesting a recomile
		if (recompile) {
			shader.recompileShaders();
			recompile = false;
		}
		
		// Check and call events
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use our shader program so we can link it
		shader.Use();   
		//matrix transforms
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		//these uniform matrix commands link with our shaders
		//the words in quotes are the variables inside of the shader files
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// Draw the loaded model
		glm::mat4 model1;
	//	model1 = glm::translate(model1, glm::vec3(0.0f, -0.2f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model1 = glm::scale(model1, glm::vec3(0.009f, 0.009f, 0.009f));	// It's a bit too big for our scene, so scale it down
		//this is the model position we pass to the vertex shader
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model1));
		//this is the light position we pass to the vertex shader
		glUniform3fv(glGetUniformLocation(shader.Program, "lightPos"), 1, &lightROT1.Position[0]);
		//this is the eye position we pass to the vertex shader
		glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, &camera.Position[0]);
		//draw our model
		ourModel.draw(shader);
		
	
		// Swap the buffers
		glfwSwapBuffers(window);
	
	
	}

	glfwTerminate();
	return 0;
}
//this is the error call back 


void error_callback(int error, const char* description) {
	cout << "There was an error rendering :: " << description << endl;
	fprintf(stderr, "Error: %s\n", description);
}



#pragma region "User input"
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
	if (keys[GLFW_KEY_1])//full face
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (keys[GLFW_KEY_2])//wire frame
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (keys[GLFW_KEY_R])
		recompile = true;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	if (keys[mouseRotationHotKey] == 1) {
		//std::cout << "Alt has been pressed" << std::endl;
	}
	if (canRotateCamera) {
		//this rotates the camera based on mouse position input
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	if (canRotateLight) {
		lightROT1.ProcessMouseMovement(xoffset, yoffset);
		//light out put debug
		std::cout << "( " << lightROT1.Position.x << ", " << lightROT1.Position.y << ", " << lightROT1.Position.z << " )" << std::endl;
	}
	
}

//mouse button press callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	//this code block is to control mouse dragging for the Camera
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		//cout << "Pressed MB1" << endl;
		canRotateCamera = true;
	}else{
		//cout << "released MB1" << endl;
		canRotateCamera = false;
	}
	//this code rotates the light 
	//if RMB and alt is pressed rotate the light
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && keys[mouseRotationHotKey] == 1) {
		canRotateLight = true;
		std::cout << "We can rotate the light " << std::endl;
	}
	else {
		canRotateLight = false;
		std::cout << "We cant rotate the light " << std::endl;
	}


}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
	totalScrollOffset.x += xoffset;
	totalScrollOffset.y += yoffset;
	cout << "Xoffset = " << totalScrollOffset.x << " :: Yoffset = " << totalScrollOffset.y << endl;
}

#pragma endregion



