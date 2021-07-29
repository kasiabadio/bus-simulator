#ifndef MESH_H_
#define MESH_H_
#include <vector>
#include <assimp/mesh.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


class Mesh
{
private:
	std::vector<glm::vec4> verts;
	std::vector<glm::vec4> norms;
	std::vector<glm::vec2> texture_coordinates;
	std::vector<unsigned int> indices;

public:
	Mesh();
	void write_mesh(aiMesh* _mesh);
	std::vector<glm::vec4> read_verts() const { return verts;  }
	std::vector<glm::vec4> read_norms() const { return norms; }
	std::vector<glm::vec2> read_texture_coordinates() const { return texture_coordinates; }
	std::vector<unsigned int> read_indices() const { return indices; }

};



#endif
