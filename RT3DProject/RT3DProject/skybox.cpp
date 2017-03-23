#define STB_IMAGE_IMPLEMENTATION

#include "Skybox.h"
#include "shader.h"
#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

namespace Rendering
{
	bool Skybox::loadCubeMapSide(GLuint& texture, GLenum side_target, const char* file_name) {

		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		int x, y, n;
		unsigned char*  image_data = stbi_load(file_name, &x, &y, &n, STBI_rgb_alpha);
		if (!image_data) {
			fprintf(stderr, "ERROR: could not load %s\n", file_name);
			return false;
		}
		// non-power-of-2 dimensions check
		if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
			fprintf(stderr,
				"WARNING: image %s is not power-of-2 dimensions\n",
				file_name);
		}

		// copy image data into 'target' side of cube map
		glTexImage2D(
			side_target,
			0,
			GL_RGBA,
			x,
			y,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image_data);
		stbi_image_free(image_data);
		return true;
	}

	Skybox::Skybox(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, const char* vertexPath, const char* fragmentPath) : shader(vertexPath, fragmentPath)
	{
		GLfloat skyboxVertices[] = {
			// Positions          
			-SIZE,  SIZE, -SIZE,
			-SIZE, -SIZE, -SIZE,
			SIZE, -SIZE, -SIZE,
			SIZE, -SIZE, -SIZE,
			SIZE,  SIZE, -SIZE,
			-SIZE,  SIZE, -SIZE,

			-SIZE, -SIZE,  SIZE,
			-SIZE, -SIZE, -SIZE,
			-SIZE,  SIZE, -SIZE,
			-SIZE,  SIZE, -SIZE,
			-SIZE,  SIZE,  SIZE,
			-SIZE, -SIZE,  SIZE,

			SIZE, -SIZE, -SIZE,
			SIZE, -SIZE,  SIZE,
			SIZE,  SIZE,  SIZE,
			SIZE,  SIZE,  SIZE,
			SIZE,  SIZE, -SIZE,
			SIZE, -SIZE, -SIZE,

			-SIZE, -SIZE,  SIZE,
			-SIZE,  SIZE,  SIZE,
			SIZE,  SIZE,  SIZE,
			SIZE,  SIZE,  SIZE,
			SIZE, -SIZE,  SIZE,
			-SIZE, -SIZE,  SIZE,

			-SIZE,  SIZE, -SIZE,
			SIZE,  SIZE, -SIZE,
			SIZE,  SIZE,  SIZE,
			SIZE,  SIZE,  SIZE,
			-SIZE,  SIZE,  SIZE,
			-SIZE,  SIZE, -SIZE,

			-SIZE, -SIZE, -SIZE,
			-SIZE, -SIZE,  SIZE,
			SIZE, -SIZE, -SIZE,
			SIZE, -SIZE, -SIZE,
			-SIZE, -SIZE,  SIZE,
			SIZE, -SIZE,  SIZE
		};

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &skyboxVertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// generate a cube-map texture to hold all the sides	
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		// load each image and copy into a side of the cube-map texture
		loadCubeMapSide(texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
		loadCubeMapSide(texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
		loadCubeMapSide(texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
		loadCubeMapSide(texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
		loadCubeMapSide(texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
		loadCubeMapSide(texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);

		// format cube map texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	Skybox::~Skybox()
	{
	}

	void Skybox::render(Camera& camera) const
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		shader.use();

		auto projection = glm::perspective(static_cast<float>(1), 1600.0f / 900, 0.1f, 2000.0f);
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

	}
}
