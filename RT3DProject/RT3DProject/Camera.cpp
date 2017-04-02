#include "camera.h"
#include <string>

void camera::update(const float & deltaTime, const glm::vec3 & targetPos)
{
	if (length(mouseMotion * SENSITIVTY * deltaTime) >= deadZone && mouseMotion != glm::vec2(0,0))
	{
		yaw += -mouseMotion.x * SENSITIVTY * deltaTime;

		pitch += mouseMotion.y * SENSITIVTY * deltaTime;

		if (!isFPS()) {
			pitch = glm::clamp(pitch, -0.3f, 1.5f);
		}
		else {
			pitch = glm::clamp(pitch, -1.5f, 1.5f);
		}
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

	position += (interpolation * deltaTime) * (glm::vec3(camX, camY, camZ) - position);

	view = lookAt(glm::vec3(position),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));

	view = translate(view, -targetPos);

	mouseMotion = glm::vec3(0);

	updateCameraVectors(position);
}

const float& camera::getYaw() const
{
	return yaw;
}

const glm::vec3& camera::getFront() const
{
	return this->front;
}

void camera::updateCameraVectors(const glm::vec3& targetPos)
{
	this->front = normalize(-targetPos);
	this->right = normalize(cross(this->front, this->worldUp));
	this->up = normalize(cross(this->right, this->front));
}

camera::camera(glm::vec3 position, glm::vec3 up) : front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = position;
	this->worldUp = up;
	this->yaw = 0;
	this->pitch = 0;
}

glm::mat4 camera::getViewMatrix() const
{
	return view;
}

void camera::processMouseMovement(const glm::vec2& offset)
{
	if (isFPS())
		mouseMotion = offset;

	mouseMotion = -offset;
}

void camera::processMouseScroll(float yoffset)
{
	distance += yoffset * SCROLL_SENSITIVTY;
	distance = glm::clamp(distance, 0.0f, MAX_DISTANCE);
}

void camera::snapDistance(const float& distance)
{
	if (!isFPS())
		this->distance = distance;}

bool camera::isFPS() const
{
	return distance == 0;
}

void camera::snapToMaxDistance()
{
	if(distance < MAX_DISTANCE && distance > 0)
	{
		distance = MAX_DISTANCE;
	}
	else
	{
		if (distance == 0)
		{
			distance = MAX_DISTANCE;
		}
		else if (distance == MAX_DISTANCE)
		{
			distance = 0;
		}
	}
}
