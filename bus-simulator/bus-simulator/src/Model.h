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




class Model
{
private:
	const aiScene* scene;
	GLuint tex;


public:

	std::vector<Mesh> meshes;
	Model(const char* model_file, const char* model_texture);
	void read_model(); // The result is vector of meshes
	GLuint read_model_texture(const char* filename);


	void draw_model(float angle_x, float angle_y) const;
	GLuint read_tex() const { return tex; }
};


#endif