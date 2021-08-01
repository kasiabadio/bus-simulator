#ifndef MESH_H_
#define MESH_H_


#include <vector>
#include <GL/glew.h>

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Mesh
{
public:
	std::vector<glm::vec4> mesh_verts;
	std::vector<glm::vec4> mesh_norms;
	std::vector<glm::vec2> mesh_texture_coordinates;
	std::vector<unsigned int> mesh_indices;

	Mesh();
	void draw_mesh(glm::mat4 P, glm::mat4 V, glm::mat4 M, Mesh& _mesh, const GLuint& _tex) const;

};

#endif