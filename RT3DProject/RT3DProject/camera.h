#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 100.0f;
	const GLfloat SENSITIVTY = 0.05f;
	const GLfloat ZOOM = 45.0f;
	const GLfloat MAX_FOV = 45.1f;
	const GLfloat MIN_FOV = 45.0f;

	glm::vec3 MovementNormal;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Yaw;
	GLfloat Pitch;

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

	void MoveForward()
	{
		MovementNormal += this->Front;
	}

	void MoveBackward()
	{
		MovementNormal -= this->Front;
	}

	void MoveLeft()
	{
		MovementNormal -= this->Right;
	}

	void MoveRight()
	{
		MovementNormal += this->Right;
	}

	const glm::vec3& Normalise(glm::vec3& vector) const
	{
		if (vector != glm::vec3(0))
		{
			return vector = normalize(vector);
		}
		return vector;
	}

	void Update(float deltaTime)
	{
		Normalise(MovementNormal);

		auto y = Position.y;

		auto velocity = this->SPEED * deltaTime;
		this->Position += this->MovementNormal * velocity;

		Position.y = y;
		MovementNormal = glm::vec3();
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
		this->Front = normalize(front);

		this->Right = normalize(cross(this->Front, this->WorldUp)); 
		this->Up = normalize(cross(this->Right, this->Front));
	}
};
