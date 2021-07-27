#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::write_model(std::string filepath)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes);
	std::cout << importer.GetErrorString() << std::endl;


	
}


