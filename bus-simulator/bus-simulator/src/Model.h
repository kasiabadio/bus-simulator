#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include "Shader.h"
#include "lodepng.h"

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


class Mesh;

class Model
{
private:
	const aiScene* scene;
	const aiNode* root_node;
	GLuint tex;


public:

	std::vector<Mesh> meshes;
	Model(std::string filepath);
	void read_model(); // result is vector of meshes
	GLuint readTexture(const char* filename);


	void draw_model(float angle_x, float angle_y) const;
	GLuint read_tex() const { return tex; }
};


class Mesh
{
public:
	std::vector<glm::vec4> mesh_verts;
	std::vector<glm::vec4> mesh_norms;
	std::vector<glm::vec2> mesh_texture_coordinates;
	std::vector<unsigned int> mesh_indices;

public:
	Mesh();
	void draw_mesh(glm::mat4 P, glm::mat4 V, glm::mat4 M, Mesh& _mesh, const GLuint &_tex) const;
	
};




#endif