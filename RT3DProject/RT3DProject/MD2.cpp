#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glm/glm.hpp>
#include <vector>
#include "rt3d.h"
#include "MD2.h"

MD2::MD2(const std::string& filename)
{
	readMD2Model(filename);
	currentAnim = 0;
	currentFrame = 0;
	nextFrame = 1;
	interp = 0.0f;
}

MD2::MD2(): vertDataSize(0), animVerts(nullptr)
{
	currentAnim = 0;
	currentFrame = 0;
	nextFrame = 1;
	interp = 0.0f;
}

MD2::~MD2()
{
	freeModel();
	int i;
	for (i = 0; i<mdl.header.num_frames; ++i) {
		delete[] vertData[i];
	}
	delete[] animVerts;
}

const GLuint& MD2::readMD2Model(const std::string& filename)
{
	FILE *fp;
	int i;

	fp = fopen(filename.c_str(), "rb");
	if (!fp)
	{
		fprintf(stderr, "Error: couldn't open \"%s\"!\n", filename.c_str());
		return 0;
	}

	/* Read header */
	fread(&mdl.header, 1, sizeof(struct md2_header_t), fp);

	if ((mdl.header.ident != 844121161) ||
		(mdl.header.version != 8))
	{
		/* Error! */
		fprintf(stderr, "Error: bad version or identifier\n");
		fclose(fp);
		return 0;
	}

	/* Memory allocations */
	mdl.skins = static_cast<struct md2_skin_t *>(malloc(sizeof(struct md2_skin_t) * mdl.header.num_skins));
	mdl.texcoords = static_cast<struct md2_texCoord_t *>(malloc(sizeof(struct md2_texCoord_t) * mdl.header.num_st));
	mdl.triangles = static_cast<struct md2_triangle_t *>(malloc(sizeof(struct md2_triangle_t) * mdl.header.num_tris));
	mdl.frames = static_cast<struct md2_frame_t *>(malloc(sizeof(struct md2_frame_t) * mdl.header.num_frames));
	mdl.glcmds = static_cast<int *>(malloc(sizeof(int) * mdl.header.num_glcmds));

	/* Read m_MD2 data */
	fseek(fp, mdl.header.offset_skins, SEEK_SET);
	fread(mdl.skins, sizeof(struct md2_skin_t),
		mdl.header.num_skins, fp);

	fseek(fp, mdl.header.offset_st, SEEK_SET);
	fread(mdl.texcoords, sizeof(struct md2_texCoord_t),
		mdl.header.num_st, fp);

	fseek(fp, mdl.header.offset_tris, SEEK_SET);
	fread(mdl.triangles, sizeof(struct md2_triangle_t),
		mdl.header.num_tris, fp);

	fseek(fp, mdl.header.offset_glcmds, SEEK_SET);
	fread(mdl.glcmds, sizeof(int), mdl.header.num_glcmds, fp);

	/* Read frames */
	fseek(fp, mdl.header.offset_frames, SEEK_SET);
	for (i = 0; i < mdl.header.num_frames; ++i)
	{
		/* Memory allocation for vertices of this frame */
		mdl.frames[i].verts = static_cast<struct md2_vertex_t *>(malloc(sizeof(struct md2_vertex_t) * mdl.header.num_vertices));

		/* Read frame data */
		fread(mdl.frames[i].scale, sizeof(md2vec3), 1, fp);
		fread(mdl.frames[i].translate, sizeof(md2vec3), 1, fp);
		fread(mdl.frames[i].name, sizeof(char), 16, fp);
		fread(mdl.frames[i].verts, sizeof(struct md2_vertex_t),
			mdl.header.num_vertices, fp);
	}

	fclose(fp);

	// now generate VBO data and create mesh
	// then save the data we actually need and free all the stuff we no longer need
	// this is required to allow the correct generation of normals etc

	int j;
	struct md2_frame_t *pframe;
	struct md2_vertex_t *pvert;

	//std::vector<GLfloat> verts;
	// these automatic variables will be created on stack and automatically deleted when this
	// function ends - no need to delete
	std::vector<GLfloat> tex_coords;
	std::vector<GLfloat> norms;
	std::vector<GLuint> indices;

	pframe = &mdl.frames[0]; // first frame
							 // For each triangle 
							 // The bad news for MD2 is that we need to expand the triangles to get the data
							 // The number of tex coords need not match the number of vertices, and the number of normals
							 // is different again. So we need to calculate each vertex
							 // Is it possible to use indexing and remove duplicates though...
							 // But I'll not bother doing that for this aged format that I'm definately dumping next year
							 // (about 8 years overdue...! :-D )
	for (i = 0; i < mdl.header.num_tris; ++i)
	{
		// For each vertex 
		for (j = 0; j < 3; ++j)
		{
			// Get texture coordinates 
			tex_coords.push_back(static_cast<GLfloat>(mdl.texcoords[mdl.triangles[i].st[j]].s) / mdl.header.skinwidth);
			tex_coords.push_back(static_cast<GLfloat>(mdl.texcoords[mdl.triangles[i].st[j]].t) / mdl.header.skinheight);

			// get current vertex
			pvert = &pframe->verts[mdl.triangles[i].vertex[j]];

			// Get normals 
			/* Table of precalculated normals */
			md2vec3 anorms_table[162] = {
			#include "anorms.h"
			};
			norms.push_back(anorms_table[pvert->normalIndex][0]);
			norms.push_back(anorms_table[pvert->normalIndex][1]);
			norms.push_back(anorms_table[pvert->normalIndex][2]);

		}
	}
	// now repeat for each frame...
	int k;
	GLfloat *verts;
	vertDataSize = mdl.header.num_tris * 9;
	for (k = 0; k<mdl.header.num_frames; ++k) {
		verts = new GLfloat[vertDataSize];
		pframe = &mdl.frames[k]; // first frame
		for (i = 0; i < mdl.header.num_tris; ++i)
		{
			// For each vertex 
			for (j = 0; j < 3; ++j)
			{
				// get current vertex
				pvert = &pframe->verts[mdl.triangles[i].vertex[j]];
				verts[(i * 3 + j) * 3] = GLfloat(pframe->scale[0] * pvert->v[0] + pframe->translate[0]);
				verts[(i * 3 + j) * 3 + 1] = GLfloat(pframe->scale[1] * pvert->v[1] + pframe->translate[1]);
				verts[(i * 3 + j) * 3 + 2] = GLfloat(pframe->scale[2] * pvert->v[2] + pframe->translate[2]);
			}
		}
		vertData.push_back(verts);
	}

	// initialise animVerts with frame 0 data
	animVerts = new GLfloat[vertDataSize];
	memcpy(animVerts, vertData[0], vertDataSize * sizeof(float));


	GLuint VAO;
	VAO = rt3d::createInterpolatedMesh(mdl.header.num_tris * 3, vertData[0], vertData[0], nullptr, norms.data(), tex_coords.data(), 0, nullptr);

	// actually have all the data we need, so call FreeModel
	this->freeModel();

	return VAO;
}

void MD2::freeModel()
{
	int i;

	if (mdl.skins)
	{
		free(mdl.skins);
		mdl.skins = nullptr;
	}

	if (mdl.texcoords)
	{
		free(mdl.texcoords);
		mdl.texcoords = nullptr;
	}

	if (mdl.triangles)
	{
		free(mdl.triangles);
		mdl.triangles = nullptr;
	}

	if (mdl.glcmds)
	{
		free(mdl.glcmds);
		mdl.glcmds = nullptr;
	}

	if (mdl.frames)
	{
		for (i = 0; i < mdl.header.num_frames; ++i)
		{
			free(mdl.frames[i].verts);
			mdl.frames[i].verts = nullptr;
		}

		free(mdl.frames);
		mdl.frames = nullptr;
	}
}

void MD2::animate(const int& animation, const float& dt)
{
	auto start = animFrameList[animation * 2];
	auto end = animFrameList[animation * 2 + 1];
	if ((currentFrame < start) || (currentFrame > end))
	{
		currentFrame = start;
		nextFrame = start + 1;
	}
	interp += dt;

	if (interp >= 1.0f)
	{
		interp = 0.0f;
		currentFrame = nextFrame;
		nextFrame++;

		if (nextFrame >= end + 1)
			nextFrame = start;
	}
}