#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.inl>

namespace Rendering
{
	class Camera
	{
	private:
		const GLfloat ZOOM = 45.0f;
		const GLfloat MAX_FOV = 45.1f;
		const GLfloat MIN_FOV = 45.0f;

		glm::mat4 view;
		glm::vec3 position;
		GLfloat zoom = ZOOM;
	public:

		const GLfloat& getZoom() const
		{
			return zoom;
		}

		const glm::vec3& getPosition() const
		{
			return position;
		}

		void setPosition(const glm::vec3& position)
		{
			this->position = position;
		}

		glm::mat4 GetViewMatrix() const
		{
			return view;
		}

		void update(float currentTime)
		{
			auto radius = 120.0f;
			position.x = sin(currentTime / 24.0) * radius;
			position.z = cos(currentTime / 24.0) * radius;
			view = glm::lookAt(glm::vec3(position.x, 0.0, position.z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		}
	};
}