#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 100.0f;
	const GLfloat SENSITIVTY = 0.1f;
	const GLfloat SCROLL_SENSITIVTY = 10.0f;
	const GLfloat MAX_FOV = 45.1f;
	const GLfloat MIN_FOV = 45.0f;

	glm::vec2 mouseMotion;

	glm::vec3 MovementNormal;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Yaw;
	GLfloat Pitch;

	glm::mat4 view;

	float distance = 0;
	const float MAX_DISTANCE = 150.0f;
public:
	glm::vec3 Position;

	const glm::vec3& GetFront() const { return this->Front; }

	const void Update(const float& deltaTime, const glm::vec3& targetPos);

	explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : Front(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = YAW;
		this->Pitch = PITCH;
	}

	glm::mat4 GetViewMatrix() const
	{
		return view;
	}

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset)
	{
		if(isFPS())
		mouseMotion = glm::vec2(xoffset, yoffset);

		mouseMotion = glm::vec2(-xoffset, -yoffset);
	}

	void ProcessMouseScroll(GLfloat yoffset)
	{
		distance += yoffset * SCROLL_SENSITIVTY;
		distance = glm::clamp(distance, 0.0f, MAX_DISTANCE);
	}

	bool isFPS() const
	{
		return distance == 0;
	}

private:
	void updateCameraVectors(const glm::vec3& targetPos)
	{
		this->Front = normalize(-targetPos);
		this->Right = normalize(cross(this->Front, this->WorldUp)); 
		this->Up = normalize(cross(this->Right, this->Front));
	}
};
