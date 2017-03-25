#pragma once
#include <fstream>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <iostream>
#include <sstream>
#include "rt3d.h"

namespace Rendering
{
	class Shader
	{
		GLuint m_program;
		int m_lightCount = 0;
	public:
		Shader(){}
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		char* loadFile(const char *fname, GLint &fSize) const;
		void use() const;
		const GLuint& getProgram() const { return this->m_program; }
		void disable() const;
		void setUniformMatrix4fv(const char* uniformName, const GLfloat* data) const;
		void setMatrices(const GLfloat* proj, const GLfloat* mv, const GLfloat* mvp) const;
		void setLightPos(const GLfloat* lightPos) const;
		void setProjection(const GLfloat* data) const;
		void setLight1(const rt3d::lightStruct& light);
		void setLight2(const rt3d::lightStruct & light);
		void setMaterial(const rt3d::materialStruct& material) const;
	};
}
