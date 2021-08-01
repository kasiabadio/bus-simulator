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

class Input {
public:
	float angle_x;
	float angle_y;
	glm::mat4 P_scene;
	glm::mat4 V_scene;
	glm::mat4 M_scene;
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
	Model(const char* model_file, const char* model_texture);
	void read_model(); // The result is vector of meshes
	GLuint read_model_texture(const char* filename);
	
	virtual void draw_model(const Input& in) = 0;
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


#endif