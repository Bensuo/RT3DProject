#pragma once
#include <GL/glew.h>
#include "shader.h"
#include "camera.h"

namespace rendering
{	
	class skybox
	{
		const float SIZE = 1;
		GLuint vbo;
		GLuint vao;
		GLuint texture;
		shader shader;
		bool loadCubeMapSide(GLuint& texture, GLenum side_target, const char* file_name);
	public:
		skybox() {}
		skybox(const char* front,
			const char* back,
			const char* top,
			const char* bottom,
			const char* left,
			const char* right,
			const char* vertexPath,
			const char* fragmentPath);
		~skybox();
		void render(camera& camera) const;
	};
}
