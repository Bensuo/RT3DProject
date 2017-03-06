#pragma once
#include <GL/glew.h>
#include "shader.h"
#include "camera.h"

namespace Rendering
{	
	class Skybox
	{
		const float SIZE = 1;
		GLuint vbo;

		
		bool loadCubeMapSide(GLuint& texture, GLenum side_target, const char* file_name);
	public:
		Skybox() {}
		Skybox(const char* front,
			const char* back,
			const char* top,
			const char* bottom,
			const char* left,
			const char* right,
			const char* vertexPath,
			const char* fragmentPath);
		~Skybox();
		void render(Camera& camera) const;
		GLuint vao;
		GLuint texture;
		Shader shader;
	};
}
