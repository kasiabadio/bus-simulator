#ifndef MODEL_H_
#define MODEL_H_

#include <vector>

#include "lodepng.h"
#include "Mesh.h"

#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


class Input
{
public:
	float angle_x;
	float angle_y;
	glm::mat4 P;
	glm::mat4 V;
	glm::mat4 M;
	
	Input(float angle_x, float angle_y, glm::mat4 P_scene, glm::mat4 V_scene, glm::mat4 M_scene);
	Input(glm::mat4 P_scene, glm::mat4 V_scene, glm::mat4 M_scene);
	
};


class Model
{
protected:
	const aiScene* scene;
	GLuint tex;

public:

	std::vector<Mesh> meshes;
	glm::mat4 M;
	glm::mat4 V;
	glm::mat4 P;
	
	Model(const char* model_file, const char* model_texture);
	void write_model(); 
	virtual void draw_model(const Input& in) = 0;
	Input read_model_matrices();
	GLuint write_model_texture(const char* filename);
	GLuint read_tex() const { return tex; }
};


class Bus: public Model
{
public:
	using Model::Model; // Constructor inheritance
	void draw_model(const Input& in) override;
	
};


class Grass: public Model
{
public:
	using Model::Model;
	void draw_model(const Input& in) override;
};


//////////////////////////////
class Terrain
{
public:
	const aiScene* scene;
	Terrain(const char* model_texture, int height, int width);
	GLuint tex;
	glm::mat4 M;
	glm::mat4 V;
	glm::mat4 P;

	int terrain_height;
	int terrain_width;
	std::vector<glm::vec4> terrain_verts;
	std::vector<glm::vec4> terrain_norms;
	std::vector<glm::vec2> terrain_texture_coordinates;
	std::vector<unsigned int> terrain_indices;

	
	void draw_terrain(const Input& in);
	Input read_model_matrices();
	int terrain_indices_count() const;
	int terrain_vertices_count() const;
	GLuint write_model_texture(const char* filename);
	GLuint read_tex() const { return tex; }
};


#endif