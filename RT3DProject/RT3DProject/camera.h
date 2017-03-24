#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	const float SPEED = 100.0f;
	const float SENSITIVTY = 0.1f;
	const float SCROLL_SENSITIVTY = 10.0f;
	const float MAX_FOV = 45.1f;
	const float MIN_FOV = 45.0f;

	glm::vec2 mouseMotion;

	glm::vec3 position;
	glm::vec3 movementNormal;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	glm::mat4 view;

	float distance = 0;
	const float MAX_DISTANCE = 150.0f;
public:
	const glm::vec3& GetFront() const;
	void Update(const float& deltaTime, const glm::vec3& targetPos);
	explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 GetViewMatrix() const;
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseScroll(float yoffset);
	void SnapDistance(const float& distance);
	bool isFPS() const;
private:
	void updateCameraVectors(const glm::vec3& targetPos);
};
