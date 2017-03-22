#include "CameraThirdPerson.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.hpp>

CameraThirdPerson::CameraThirdPerson()
{
	yaw = glm::pi<float>();	//horizontal angle
	pitch = 0;				//vertical angle
}

void CameraThirdPerson::Update(float deltaTime, glm::vec3 targetPos)
{
	if (radius >= MAX_RADIUS)
		radius = MAX_RADIUS;

	if (radius <= 0.1)
		radius = 0.1;

	yaw += -mouseMotion.x * MOUSE_SENSITIVITY * deltaTime;

	pitch += mouseMotion.y * MOUSE_SENSITIVITY * deltaTime;
	pitch = glm::clamp(pitch, -0.3f, 1.5f);

	auto camX = radius * cos(yaw) * cos(pitch);
	auto camY = radius * sin(pitch);
	auto camZ = radius * sin(yaw) * cos(pitch);
	cameraPos = glm::vec3(camX, camY, camZ);

	view = lookAt(glm::vec3(cameraPos),
				  glm::vec3(0.0, 0.0, 0.0),											
				  glm::vec3(0.0, 1.0, 0.0));

	view = translate(view, -targetPos);

	mouseMotion = glm::vec3(0);
}

void CameraThirdPerson::UpdateRotation(glm::vec2 mousePos)
{
	this->mouseMotion = mousePos;
	
}

void CameraThirdPerson::UpdateRadius(float radius)
{
	this->radius += radius * SCROLL_SENSITIVITY;
}

glm::mat4 CameraThirdPerson::GetViewMatrix() const
{
	return view;
}

CameraThirdPerson::~CameraThirdPerson()
{
}
