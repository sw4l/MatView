#pragma once

/*
	This object is close to being done issues left
	1. Zoom is broken(fix it )

*/


// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//we will pass in our constructor a initial camera position
//such as CameraArcball camera(glm::vec3(0.0f, 0.0f, 3.0f));


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	RISE,
	FALL

	//rise and fall will be used to change target up and down
};


// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.1f;
const GLfloat kSENSITIVTY = 1.0f;
const GLfloat ZOOM = 1.0f;
const glm::vec3 TARGETSENSITIVITY(0.0f,0.01f,0.0f);



class CameraArcball {
private:
	//private function to update values as input is done
	void updateCameraVectors() {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		glm::vec3 position;
		//update our position based on change in yaw/pitch
		position.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		position.y = sin(glm::radians(this->Pitch));
		position.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Position = glm::normalize(position);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));

	}

	float lastYoffset = 0.0f;

public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::vec3 targetPositon;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	//constructor
	CameraArcball(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		//static target position at the center of the viewport
		this->targetPositon = glm::vec3(0.0f, 0.0f, 0.0f);
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor with scalar values
	CameraArcball(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	//set the targetLookAtPosition
	void setTargetLookAtPosition(glm::vec3* target) {
		this->targetPositon = *target;
	}
	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	//in order to keep looking at the center we keep the second argument at the center
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->targetPositon, this->Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;
		this->Yaw += xoffset;
		this->Pitch -= yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset)
	{
		float zoomSens = 0.1f;

		//make sure we dont go past our limits
		if (this->Zoom <= 0.0f)
			this->Zoom = 0.0f;
		if (this->Zoom >= 3.0f)
			this->Zoom = 3.0f;

		//zoom in
		if (yoffset > lastYoffset) {
			this->Zoom -= zoomSens;
		}
		//zoom out
		if (yoffset < lastYoffset) {
			this->Zoom += zoomSens;
		}

		
	}

















};





/*- this is the old input system designed for fps like movement
GLfloat velocity = this->MovementSpeed * deltaTime;
if (direction == FORWARD)
this->Position += this->Front * velocity;
if (direction == BACKWARD)
this->Position -= this->Front * velocity;
if (direction == LEFT)
this->Position -= this->Right * velocity;
if (direction == RIGHT)
this->Position += this->Right * velocity;*/