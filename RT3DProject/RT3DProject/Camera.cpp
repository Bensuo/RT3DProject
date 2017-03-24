#include "Camera.h"

void Camera::Update(const float & deltaTime, const glm::vec3 & targetPos)
{
	yaw += -mouseMotion.x * SENSITIVTY * deltaTime;

	pitch += mouseMotion.y * SENSITIVTY * deltaTime;

	if (!isFPS()) {
		pitch = glm::clamp(pitch, -0.1f, 1.5f);
	} else {
		pitch = glm::clamp(pitch, -1.5f, 1.5f);
	}

	float camX;
	float camY;
	float camZ;

	if (distance == 0)
	{
		camX = cos(yaw) * cos(pitch);
		camY = sin(pitch);
		camZ = sin(yaw) * cos(pitch);
	}
	else
	{
		camX = distance * cos(yaw) * cos(pitch);
		camY = distance * sin(pitch);
		camZ = distance * sin(yaw) * cos(pitch);
	}

	position = glm::vec3(camX, camY, camZ);

	view = lookAt(glm::vec3(position),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));

	view = translate(view, -targetPos);

	mouseMotion = glm::vec3(0);

	updateCameraVectors(position);
}

const glm::vec3& Camera::GetFront() const
{
	return this->front;
}

void Camera::updateCameraVectors(const glm::vec3& targetPos)
{
	this->front = normalize(-targetPos);
	this->right = normalize(cross(this->front, this->worldUp));
	this->up = normalize(cross(this->right, this->front));
}

Camera::Camera(glm::vec3 position, glm::vec3 up) : front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = position;
	this->worldUp = up;
	this->yaw = 0;
	this->pitch = 0;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return view;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	if (isFPS())
		mouseMotion = glm::vec2(xoffset, yoffset);

	mouseMotion = glm::vec2(-xoffset, -yoffset);
}

void Camera::ProcessMouseScroll(float yoffset)
{
	distance += yoffset * SCROLL_SENSITIVTY;
	distance = glm::clamp(distance, 0.0f, MAX_DISTANCE);
}

void Camera::SnapDistance(const float& distance)
{
	if (!isFPS())
		this->distance = distance;
}

bool Camera::isFPS() const
{
	return distance == 0;
}
