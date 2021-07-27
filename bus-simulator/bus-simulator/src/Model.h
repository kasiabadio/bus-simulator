
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
	std::vector<Mesh> meshes;

public:
	void write_model(std::string filepath);
	void read_model();
	void write_meshes(); 
	void read_meshes();
	void draw_model();
};

#endif