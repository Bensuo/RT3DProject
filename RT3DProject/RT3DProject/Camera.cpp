#include "camera.h"
#include <string>

//calculate target camera position based on euler angles
glm::vec3 Camera::calculatePosition() const
{
	glm::vec3 calculatePos;
	if (distance == 0)
	{
		calculatePos.x = cos(yaw) * cos(pitch);
		calculatePos.y = sin(pitch);
		calculatePos.z = sin(yaw) * cos(pitch);
	}
	else
	{
		calculatePos.x = distance * cos(yaw) * cos(pitch);
		calculatePos.y = distance * sin(pitch);
		calculatePos.z = distance * sin(yaw) * cos(pitch);
	}
	return calculatePos;
}

//update the camera so that it faces the current target position
void Camera::update(const float & deltaTime, const glm::vec3 & targetPos)
{
	//rotate camera to current mouse position
	if (length(mouseMotion * SENSITIVTY * deltaTime) >= deadZone && mouseMotion != glm::vec2(0,0))
	{
		yaw += -mouseMotion.x * SENSITIVTY * deltaTime;
		pitch += mouseMotion.y * SENSITIVTY * deltaTime;
		if (!isFPS()) 
		{
			pitch = glm::clamp(pitch, -0.3f, 1.5f);
		}
		else 
		{
			pitch = glm::clamp(pitch, -1.5f, 1.5f);
		}
	}

	//calculate target camera position based on euler angles
	auto camPos = calculatePosition();

	//interpolate current position to target position
	position += (interpolation * deltaTime) * (camPos - position);

	//calculate view matrix around world origin
	view = lookAt(glm::vec3(position),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));

	//translate view matrix to target position
	view = translate(view, -targetPos);

	mouseMotion = glm::vec3(0);

	updateCameraVectors(position);
}

//return camera yaw
const float& Camera::getYaw() const
{
	return yaw;
}

//return camera front normal
const glm::vec3& Camera::getFront() const
{
	return this->front;
}

//update the camera vectors relative to the new target position
void Camera::updateCameraVectors(const glm::vec3& targetPos)
{
	this->front = normalize(-targetPos);
	this->right = normalize(cross(this->front, this->worldUp));
	this->up = normalize(cross(this->right, this->front));
}

//construct a new camera
Camera::Camera(const glm::vec3& position, const glm::vec3& up) : front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = position;
	this->worldUp = up;
	this->yaw = 0;
	this->pitch = 0;
}

//return the current view matrix
const glm::mat4& Camera::getViewMatrix() const
{
	return view;
}

//take the relative mouse motion to adjust camera position in update
void Camera::processMouseMovement(const glm::vec2& offset)
{
	if (isFPS())
		mouseMotion = offset;

	mouseMotion = -offset;
}

//adjust the distance from the target position using the mouse wheel
void Camera::processMouseScroll(const float& yoffset)
{
	distance += yoffset * SCROLL_SENSITIVTY;
	distance = glm::clamp(distance, 0.0f, MAX_DISTANCE);
}

//snap camera position to the specified distance from the target position
void Camera::snapDistance(const float& distance)
{
	if (!isFPS())
		this->distance = distance;}

//return if the viewport is first-person or third person
bool Camera::isFPS() const
{
	return distance == 0;
}

//snap to the max distance from the target
void Camera::snapToMaxDistance()
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
