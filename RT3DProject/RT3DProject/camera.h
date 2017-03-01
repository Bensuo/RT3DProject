#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.inl>
#include "clock.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class camera
{
	// Default camera values
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat MAX_SPEED = 50.0f;
	const GLfloat SPEED = 0.6666f;
	const GLfloat SENSITIVTY = 0.05f;
	const GLfloat ZOOM = 45.0f;
	const GLfloat MAX_FOV = 45.1f;
	const GLfloat MIN_FOV = 45.0f;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;

	Camera_Movement currentDirection;
public:
	// camera Attributes
	glm::vec3 Position;
	GLfloat Zoom = ZOOM;

	// Constructor with vectors
	explicit camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(0), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = YAW;
		this->Pitch = PITCH;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() const
	{
		return lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	float getMovementSpeed() const
	{
		return MovementSpeed;
	}

private:
	// Calculates the front vector from the camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};