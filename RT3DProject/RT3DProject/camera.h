#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	NONE
};

class Camera
{
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat MAX_SPEED = 50.0f;
	const GLfloat SPEED = 100.0f;
	const GLfloat SENSITIVTY = 0.05f;
	const GLfloat ZOOM = 45.0f;
	const GLfloat MAX_FOV = 45.1f;
	const GLfloat MIN_FOV = 45.0f;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Yaw;
	GLfloat Pitch;

	Camera_Movement currentDirection;
public:
	glm::vec3 Position;
	float currentZoom = ZOOM;

	explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : Front(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = YAW;
		this->Pitch = PITCH;
		this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() const
	{
		return lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	void ProcessKeyboard(Camera_Movement direction)
	{
		currentDirection = direction;
	}

	void Update(float deltaTime)
	{
		auto y = Position.y;

		auto velocity = this->SPEED * deltaTime;
		if (currentDirection == FORWARD)
			this->Position += this->Front * velocity;
		if (currentDirection == BACKWARD)
			this->Position -= this->Front * velocity;
		if (currentDirection == LEFT)
			this->Position -= this->Right * velocity;
		if (currentDirection == RIGHT)
			this->Position += this->Right * velocity;

		Position.y = y;
		currentDirection = NONE;
	}

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->SENSITIVTY;
		yoffset *= this->SENSITIVTY;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		this->updateCameraVectors();
	}

	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->currentZoom >= 1.0f && this->currentZoom <= 45.0f)
			this->currentZoom -= yoffset;
		if (this->currentZoom <= 1.0f)
			this->currentZoom = 1.0f;
		if (this->currentZoom >= 45.0f)
			this->currentZoom = 45.0f;
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);

		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp)); 
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};