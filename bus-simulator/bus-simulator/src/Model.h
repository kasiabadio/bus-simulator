
#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>



class Model
{
private:
	const aiScene* scene;
	const aiNode* root_node;
	std::vector<Mesh> meshes;

public:
	Model(std::string filepath);
	void write_model(std::string filepath);
	void read_model(); // result is tab of meshes
	void read_model_node(aiNode* node);
	void draw_model() const;
	std::vector<Mesh> read_meshes() const { return meshes; }
	
};

#endif