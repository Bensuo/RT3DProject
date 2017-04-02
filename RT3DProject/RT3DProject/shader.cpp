#include "Shader.h"
#include "md2model.h"
#include <string>
#include <fstream>

namespace Rendering
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		GLuint f, v;

		char *vs, *fs;

		v = glCreateShader(GL_VERTEX_SHADER);
		f = glCreateShader(GL_FRAGMENT_SHADER);

		// load shaders & get length of each
		GLint vlen;
		GLint flen;
		vs = loadFile(vertexPath.c_str(), vlen);
		fs = loadFile(fragmentPath.c_str(), flen);

		const char * vv = vs;
		const char * ff = fs;

		glShaderSource(v, 1, &vv, &vlen);
		glShaderSource(f, 1, &ff, &flen);

		GLint compiled;

		glCompileShader(v);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			std::cout << "Vertex Shader not compiled." << std::endl;
			rt3d::printShaderError(v);
		}

		glCompileShader(f);
		glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			std::cout << "Fragment Shader not compiled." << std::endl;
			rt3d::printShaderError(f);
		}

		program = glCreateProgram();

		glAttachShader(program, v);
		glAttachShader(program, f);

		glBindAttribLocation(program, RT3D_VERTEX, "position");
		glBindAttribLocation(program, RT3D_COLOUR, "color");
		glBindAttribLocation(program, RT3D_NORMAL, "normal");
		glBindAttribLocation(program, RT3D_TEXCOORD, "texCoords");
		glBindAttribLocation(program, RT3D_VERTEX_NEXT, "position_next");
		glLinkProgram(program);
		glUseProgram(program);

		delete[] vs; // dont forget to free allocated memory
		delete[] fs; // we allocated this in the loadFile function...
	}

	char* Shader::loadFile(const char* fname, GLint& fSize) const
	{
		int size;
		char * memblock;

		// file read based on example in cplusplus.com tutorial
		// ios::ate opens file at the end
		std::ifstream file(fname, std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open()) {
			size = static_cast<int>(file.tellg()); // get location of file pointer i.e. file size
			fSize = static_cast<GLint>(size);
			memblock = new char[size];
			file.seekg(0, std::ios::beg);
			file.read(memblock, size);
			file.close();
			std::cout << "file " << fname << " loaded" << std::endl;
		}
		else {
			std::cout << "Unable to open file " << fname << std::endl;
			fSize = 0;
			// should ideally set a flag or use exception handling
			// so that calling function can decide what to do now
			return nullptr;
		}
		return memblock;
	}

	// Uses the current Shader
	void Shader::use() const
	{
		glUseProgram(program);
	}

	void Shader::disable() const
	{
		glUseProgram(0);
	}

	void Shader::setUniformMatrix4fv(const char* uniformName, const GLfloat *data) const
	{
		int uniformIndex = glGetUniformLocation(program, uniformName);
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data);
	}

	void Shader::setUniform1f(const char * uniformName, GLfloat data) const
	{
		int uniformIndex = glGetUniformLocation(program, uniformName);
		glUniform1f(uniformIndex, data);
	}

	void Shader::setUniform1i(const char * uniformName, GLuint data) const
	{
		int uniformIndex = glGetUniformLocation(program, uniformName);
		glUniform1i(uniformIndex, data);
	}

	// set matrices untested... likely to change - not totally happy with this for now.
	void Shader::setMatrices(const GLfloat *proj, const GLfloat *mv, const GLfloat *mvp) const
	{
		int uniformIndex = glGetUniformLocation(program, "modelview");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, mv);
		uniformIndex = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, proj);
		uniformIndex = glGetUniformLocation(program, "MVP");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, mvp);
		uniformIndex = glGetUniformLocation(program, "normalmatrix");
	}

	void Shader::setLightPos(const GLfloat *lightPos) const
	{
		int uniformIndex = glGetUniformLocation(program, "lightPosition");
		glUniform4fv(uniformIndex, 1, lightPos);
	}

	void Shader::setProjection(const GLfloat *data) const
	{
		int uniformIndex = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data);
	}

	void Shader::addLight(const rt3d::lightStruct& light, const int& index) const
	{
		auto number = std::to_string(index);
		auto uniformIndex = glGetUniformLocation(program, ("lights[" + number + "].ambient").c_str());
		glUniform4fv(uniformIndex, 1, light.ambient);
		uniformIndex = glGetUniformLocation(program, ("lights[" + number + "].diffuse").c_str());
		glUniform4fv(uniformIndex, 1, light.diffuse);
		uniformIndex = glGetUniformLocation(program, ("lights[" + number + "].specular").c_str());
		glUniform4fv(uniformIndex, 1, light.specular);
		uniformIndex = glGetUniformLocation(program, ("lights[" + number + "].position").c_str());
		glUniform4fv(uniformIndex, 1, light.position);
	}

	void Shader::setMaterial(const rt3d::materialStruct& material) const
	{
		// pass in material data to Shader 
		int uniformIndex = glGetUniformLocation(program, "material.ambient");
		glUniform4fv(uniformIndex, 1, material.ambient);
		uniformIndex = glGetUniformLocation(program, "material.diffuse");
		glUniform4fv(uniformIndex, 1, material.diffuse);
		uniformIndex = glGetUniformLocation(program, "material.specular");
		glUniform4fv(uniformIndex, 1, material.specular);
		uniformIndex = glGetUniformLocation(program, "material.shininess");
		glUniform1f(uniformIndex, material.shininess);
	}
}
