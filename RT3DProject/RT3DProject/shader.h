#pragma once
#include <fstream>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <iostream>
#include <sstream>
#include "rt3d.h"

namespace rendering
{
	class shader
	{
		GLuint Program;
	public:
		shader(){}
		shader(const std::string& vertexPath, const std::string& fragmentPath);
		char* loadFile(const char *fname, GLint &fSize);
		void use() const;
		const GLuint& getProgram() const { return this->Program; }
		void disable() const;
		void setUniformMatrix4fv(const char* uniformName, const GLfloat* data) const;
		void setMatrices(const GLfloat* proj, const GLfloat* mv, const GLfloat* mvp) const;
		void setLightPos(const GLfloat* lightPos) const;
		void setProjection(const GLfloat* data) const;
		void setLight(const rt3d::lightStruct light) const;
		void setMaterial(const rt3d::materialStruct material) const;
	};
}
