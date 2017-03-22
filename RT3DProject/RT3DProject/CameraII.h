#pragma once
#include <glm/glm.hpp>
#include "CameraThirdPerson.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.hpp>

class CameraII
{
	glm::vec3 position;
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 front = normalize(position - target);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = normalize(cross(up, front));
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view;

	glm::vec2 mouseMotion;

	float distance = 0.0f;

	const float MOUSE_SENSITIVITY = 0.25f;
	const float SCROLL_SENSITIVITY = 10.0f;

	const float MAX_DISTANCE = 150.0f;

	float yaw;
	float pitch;
public:
	float currentZoom = 45.0f;

	void UpdateThirdPerson(float deltaTime, glm::vec3 targetPos)
	{
		distance = glm::clamp(distance, 0.0f, MAX_DISTANCE);

		yaw += -mouseMotion.x * MOUSE_SENSITIVITY * deltaTime;

		pitch += mouseMotion.y * MOUSE_SENSITIVITY * deltaTime;
		pitch = glm::clamp(pitch, -0.3f, 1.5f);

		auto camX = distance * cos(yaw) * cos(pitch);
		auto camY = distance * sin(pitch);
		auto camZ = distance * sin(yaw) * cos(pitch);
		position = glm::vec3(camX, camY, camZ);

		view = lookAt(glm::vec3(position),
		              glm::vec3(0.0, 0.0, 0.0),
		              glm::vec3(0.0, 1.0, 0.0));

		view = translate(view, -targetPos);

		mouseMotion = glm::vec3(0);
	}

	void UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = normalize(front);

		this->right = normalize(cross(this->front, this->worldUp));
		this->up = normalize(cross(this->right, this->front));
	}

	void UpdateFirstPerson(const glm::vec3& targetPos)
	{
		position = targetPos;
		this->yaw += mouseMotion.x;
		this->pitch += mouseMotion.y;
		pitch = glm::clamp(pitch, -89.0f, 89.0f);
		UpdateCameraVectors();
		view = lookAt(this->position, this->position + this->front, this->up);
	}

	void Update(const float& deltaTime, const glm::vec3& targetPos)
	{
		//third person logic
		if (distance != 0)
		{
			UpdateThirdPerson(deltaTime, targetPos);
		} else {
			UpdateFirstPerson(targetPos);
		}
	}

	CameraII()
	{
		yaw = glm::pi<float>();	//horizontal angle
		pitch = 0;				//vertical angle
	}

	const glm::vec3& GetFrontNormal() const
	{
		return front;
	}

	void UpdateRotation(const glm::vec2& mousePos)
	{
		this->mouseMotion = mousePos;
	}

	void UpdateDistance(const float& distance)
	{
		this->distance += distance * SCROLL_SENSITIVITY;
	}

	const glm::mat4& GetViewMatrix() const
	{
		return view;
	}

	void ProcessMouseMovement(const glm::vec2& mousePos)
	{
		this->mouseMotion = mousePos;
	}

	void ProcessMouseScroll(float yoffset)
	{
		this->distance += yoffset * SCROLL_SENSITIVITY;
	}

	bool FirstPerson() const
	{
		return distance == 0;
	}

	~CameraII()
	{
	}
};