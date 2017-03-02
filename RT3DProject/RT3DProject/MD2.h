#pragma once
#define WIN32
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
	virtual void SetUpAnimationData() = 0;
	virtual void SetUpAnimationData(const std::vector<int>& frameList) = 0;
public:
	virtual ~MD2() {}
	GLuint ReadMD2Model(const std::string& filename);
	void FreeModel();
	void Animate(const int& animation, const float& dt);
	void Animate(const float& dt) { Animate(1, dt); }
	void setCurrentAnim(const int& n) { currentAnim = n; }
protected:
	md2_model_t mdl;
	int currentAnim;
	int currentFrame;
	int nextFrame;
	float interp;
	std::vector<GLfloat *> vertData;
	GLuint vertDataSize;
	GLfloat *animVerts;
	std::vector<int>animFrameList;
public:
	GLfloat* getAnimVerts() const { return animVerts; }
	GLuint getVertDataSize() const { return vertDataSize; }
	GLuint getVertDataCount() const { return vertDataSize / 3; }
	int getCurrentAnim() const { return currentAnim; }
};