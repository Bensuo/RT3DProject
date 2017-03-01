#include "shader.h"
#include "md2model.h"

namespace rendering
{
	shader::shader(const std::string& vertexPath, const std::string& fragmentPath)
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
			std::cout << "Vertex shader not compiled." << std::endl;
			rt3d::printShaderError(v);
		}

		glCompileShader(f);
		glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			std::cout << "Fragment shader not compiled." << std::endl;
			rt3d::printShaderError(f);
		}

		Program = glCreateProgram();

		glAttachShader(Program, v);
		glAttachShader(Program, f);

		glBindAttribLocation(Program, RT3D_VERTEX, "in_Position");
		glBindAttribLocation(Program, RT3D_COLOUR, "in_Color");
		glBindAttribLocation(Program, RT3D_NORMAL, "in_Normal");
		glBindAttribLocation(Program, RT3D_TEXCOORD, "in_TexCoord");

		glLinkProgram(Program);
		glUseProgram(Program);

		delete[] vs; // dont forget to free allocated memory
		delete[] fs; // we allocated this in the loadFile function...
	}

	char * shader::loadFile(const char * fname, GLint & fSize)
	{
		int size;
		char * memblock;

		// file read based on example in cplusplus.com tutorial
		// ios::ate opens file at the end
		std::ifstream file(fname, std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open()) {
			size = (int)file.tellg(); // get location of file pointer i.e. file size
			fSize = (GLint)size;
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

	// Uses the current shader
	void shader::use() const
	{
		glUseProgram(Program);
	}

	void shader::disable() const
	{
		glUseProgram(0);
	}

	void shader::setUniformMatrix4fv(const char* uniformName, const GLfloat *data) const
	{
		int uniformIndex = glGetUniformLocation(Program, uniformName);
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data);
	}

	// set matrices untested... likely to change - not totally happy with this for now.
	void shader::setMatrices(const GLfloat *proj, const GLfloat *mv, const GLfloat *mvp) const
	{
		int uniformIndex = glGetUniformLocation(Program, "modelview");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, mv);
		uniformIndex = glGetUniformLocation(Program, "projection");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, proj);
		uniformIndex = glGetUniformLocation(Program, "MVP");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, mvp);
		uniformIndex = glGetUniformLocation(Program, "normalmatrix");
	}

	void shader::setLightPos(const GLfloat *lightPos) const
	{
		int uniformIndex = glGetUniformLocation(Program, "lightPosition");
		glUniform4fv(uniformIndex, 1, lightPos);
	}

	void shader::setProjection(const GLfloat *data) const
	{
		int uniformIndex = glGetUniformLocation(Program, "projection");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data);
	}

	void shader::setLight(const rt3d::lightStruct light) const
	{
		// pass in light data to shader
		int uniformIndex = glGetUniformLocation(Program, "light.ambient");
		glUniform4fv(uniformIndex, 1, light.ambient);
		uniformIndex = glGetUniformLocation(Program, "light.diffuse");
		glUniform4fv(uniformIndex, 1, light.diffuse);
		uniformIndex = glGetUniformLocation(Program, "light.specular");
		glUniform4fv(uniformIndex, 1, light.specular);
		uniformIndex = glGetUniformLocation(Program, "lightPosition");
		glUniform4fv(uniformIndex, 1, light.position);
	}


	void shader::setMaterial(const rt3d::materialStruct material) const
	{
		// pass in material data to shader 
		int uniformIndex = glGetUniformLocation(Program, "material.ambient");
		glUniform4fv(uniformIndex, 1, material.ambient);
		uniformIndex = glGetUniformLocation(Program, "material.diffuse");
		glUniform4fv(uniformIndex, 1, material.diffuse);
		uniformIndex = glGetUniformLocation(Program, "material.specular");
		glUniform4fv(uniformIndex, 1, material.specular);
		uniformIndex = glGetUniformLocation(Program, "material.shininess");
		glUniform1f(uniformIndex, material.shininess);
	}
}
