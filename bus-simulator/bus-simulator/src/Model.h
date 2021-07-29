
#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include "Shader.h"
#include "Mesh.h"
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


class Model
{
private:
	const aiScene* scene;
	const aiNode* root_node;
	std::vector<Mesh> meshes;
	GLuint tex;
	
	// TEMPORARY
	std::vector<glm::vec4> verts;
	std::vector<glm::vec4> norms;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;


public:
	
	
	Model(std::string filepath);
	void read_model(); // result is tab of meshes
	void read_model_node(aiNode* node);
	GLuint readTexture(const char* filename);

	// TEMPORARY
	void loadModel();
	
	void draw_model(float angle_x, float angle_y) const;
	std::vector<Mesh> read_meshes() const { return meshes; }
	GLuint read_tex() const { return tex; }
};

#endif