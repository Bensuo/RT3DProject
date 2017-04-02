#pragma once
#include "rt3d.h"
#include <vector>

typedef GLfloat md2vec3[3];

struct md2_header_t
{
	int ident;
	int version;

	int skinwidth;
	int skinheight;

	int framesize;

	int num_skins;
	int num_vertices;
	int num_st;
	int num_tris;
	int num_glcmds;
	int num_frames;

	int offset_skins;
	int offset_st;
	int offset_tris;
	int offset_frames;
	int offset_glcmds;
	int offset_end;
};

struct md2_skin_t
{
	char name[64];
};

struct md2_texCoord_t
{
	short s;
	short t;
};

struct md2_triangle_t
{
	unsigned short vertex[3];
	unsigned short st[3];
};

struct md2_vertex_t
{
	unsigned char v[3];
	unsigned char normalIndex;
};

struct md2_frame_t
{
	md2vec3 scale;
	md2vec3 translate;
	char name[16];
	struct md2_vertex_t *verts;
};

struct md2_glcmd_t
{
	float s;
	float t;
	int index;
};

struct md2_model_t
{
	struct md2_header_t header;

	struct md2_skin_t *skins;
	struct md2_texCoord_t *texcoords;
	struct md2_triangle_t *triangles;
	struct md2_frame_t *frames;
	int *glcmds;

	GLuint tex_id;
};

class MD2
{
public:
	explicit MD2(const std::string& filename);
	MD2();
	virtual ~MD2();
	const GLuint& readMD2Model(const std::string& filename);
	void freeModel();
	void animate(const int& animation, const float& dt);
	void animate(const float& dt) { animate(currentAnim, dt); }
	void setCurrentAnim(const int& n) { currentAnim = n; }
	void setUpAnimationData(const std::vector<int>& frameList) { animFrameList = frameList; }
	void resetAnimation() {
		currentFrame = 0;
	}
	float getInterp() const { return interp; }
protected:
	md2_model_t mdl;
	int currentAnim = 0;
	int currentFrame;
	int nextFrame;
	float interp;
	std::vector<GLfloat *> vertData;
	GLuint vertDataSize;
	GLfloat *animVerts;
	std::vector<int>animFrameList;
public:
	const GLfloat* getAnimVerts() const { return animVerts; }
	const GLuint& getVertDataSize() const { return vertDataSize; }
	const GLuint& getVertDataCount() const { return vertDataSize / 3; }
	const int& getCurrentAnim() const { return currentAnim; }
	const int& getCurrentFrame() const { return currentFrame; }
	const std::vector<int>& GetAnimFrames() const { return animFrameList; }
	const GLfloat* getCurrentVerts() const { return vertData[currentFrame]; }
	const GLfloat* getNextVerts() const { return vertData[nextFrame]; }
};