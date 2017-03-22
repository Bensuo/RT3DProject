#pragma once
#include <glm/glm.hpp>

class CameraThirdPerson
{
	glm::vec3 cameraPos;

	//data vectors temp
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = normalize(cameraPos - cameraTarget);
	glm::vec3 upNormal = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = normalize(cross(upNormal, cameraDirection));
	glm::vec3 cameraUp = cross(cameraDirection, cameraRight);

	glm::mat4 view;
	glm::vec2 mouseMotion;
	float radius = 150.0f;
	const float MOUSE_SENSITIVITY = 0.25f;
	const float SCROLL_SENSITIVITY = 10.0f;
	const float MAX_Y = 1500.0f;
	const float MAX_RADIUS = 150.0f;
	float yaw;
	float pitch;
public:
	float currentZoom = 45.0f;
	CameraThirdPerson();
	void Update(float deltaTime, glm::vec3 targetPos);
	void UpdateRotation(glm::vec2 newPos);
	void UpdateRadius(float radius);
	glm::mat4 GetViewMatrix() const;
	~CameraThirdPerson();
};

