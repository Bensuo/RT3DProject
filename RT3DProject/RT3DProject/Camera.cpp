#include "Camera.h"

const void Camera::Update(const float & deltaTime, const glm::vec3 & targetPos)
{
	Yaw += -mouseMotion.x * SENSITIVTY * deltaTime;

	Pitch += mouseMotion.y * SENSITIVTY * deltaTime;

	if (!isFPS()) {
		Pitch = glm::clamp(Pitch, -0.1f, 1.5f);
	} else {
		Pitch = glm::clamp(Pitch, -1.5f, 1.5f);
	}

	float camX;
	float camY;
	float camZ;

	if (distance == 0)
	{
		camX = cos(Yaw) * cos(Pitch);
		camY = sin(Pitch);
		camZ = sin(Yaw) * cos(Pitch);
	}
	else
	{
		camX = distance * cos(Yaw) * cos(Pitch);
		camY = distance * sin(Pitch);
		camZ = distance * sin(Yaw) * cos(Pitch);
	}

	Position = glm::vec3(camX, camY, camZ);

	view = lookAt(glm::vec3(Position),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));

	view = translate(view, -targetPos);

	mouseMotion = glm::vec3(0);

	updateCameraVectors(Position);
}