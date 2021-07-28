#include "Model.h"



#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Model::Model(std::string filepath)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes);
	std::cout << "Number of meshes in a model: " << scene->mNumMeshes << std::endl;
	std::cout << importer.GetErrorString() << std::endl;
	root_node = scene->mRootNode;
	read_model();
}


void Model::write_model(std::string filepath)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes);
	std::cout << importer.GetErrorString() << std::endl;
	root_node = scene->mRootNode;
}


void Model::read_model()
{
	// Read all meshes of a root node of a model and read recursively its nodes
	for (int i = 0; i < root_node->mNumMeshes; i++)
	{
		Mesh mesh;
		mesh.write_mesh(scene->mMeshes[root_node->mMeshes[i]]);
		meshes.push_back(mesh);
	}

	for (int j = 0; j < root_node->mNumChildren; j++)
	{
		read_model_node(root_node->mChildren[j]);
	}
	
}


void Model::read_model_node(aiNode* model_node)
{
	// Read all meshes of a node of a model and read recursively its nodes
	for (int i = 0; i < model_node->mNumMeshes; i++)
	{
		Mesh mesh;
		mesh.write_mesh(scene->mMeshes[model_node->mMeshes[i]]);
		meshes.push_back(mesh);
	}

	for (int j = 0; j < model_node->mNumChildren; j++)
	{
		read_model_node(model_node->mChildren[j]);
	}
}


void Model::draw_model() const
{
	std::cout << "Number of meshes in a model: " << meshes.size() << std::endl;
}
