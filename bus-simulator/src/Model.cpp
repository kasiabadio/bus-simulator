#include "Model.h"

void write_model(std::string filepath)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes);
	std::cout << importer.GetErrorString() << std::endl;


}
