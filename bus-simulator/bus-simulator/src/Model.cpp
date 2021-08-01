#include "Model.h"

#include <functional>
Assimp::Importer importer;


Input::Input(float angle_x, float angle_y, glm::mat4 P_scene, glm::mat4 V_scene, glm::mat4 M_scene):
	angle_x(angle_x), angle_y(angle_y), P_scene(P_scene), V_scene(V_scene), M_scene(M_scene)
{}


Input::Input(glm::mat4 P_scene, glm::mat4 V_scene, glm::mat4 M_scene):
	angle_x(0.0f), angle_y(0.0f), P_scene(P_scene), V_scene(V_scene), M_scene(M_scene)
{}


Model::Model(const char* model_file, const char* model_texture) :
	scene(importer.ReadFile(model_file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes))

{
	std::cout << "Reading ... " << model_file << std::endl;
	// Read texture (one for each model for now)
	tex = read_model_texture(model_texture);
}


void Model::read_model()
{

	std::cout << "Number of meshes in a model: " << scene->mNumMeshes << std::endl;
	for (unsigned int l = 0; l < scene->mNumMeshes; l++)
	{
		aiMesh* mesh = scene->mMeshes[l];
		Mesh temp_mesh;

		// Read each meshes data

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			// w = 1, because it's position
			aiVector3D vertex = mesh->mVertices[i];
			temp_mesh.mesh_verts.emplace_back(vertex.x, vertex.y, vertex.z, 1);

			// w = 0, because it's direction
			aiVector3D normal = mesh->mNormals[i];
			temp_mesh.mesh_norms.emplace_back(normal.x, normal.y, normal.z, 0);


			aiVector3D texture_coords = mesh->mTextureCoords[0][i];
			temp_mesh.mesh_texture_coordinates.emplace_back(texture_coords.x, texture_coords.y);

		}


		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; j++)
			{
				temp_mesh.mesh_indices.push_back(face.mIndices[j]);
			}

		}

		meshes.push_back(temp_mesh);
	}

	std::cout << "Number of meshes which were read into vector: " << meshes.size() << std::endl << std::endl;
}


GLuint Model::read_model_texture(const char* filename) {

	GLuint tex;
	// Activate texture 0
	glActiveTexture(GL_TEXTURE0);

	// Read into computers memory
	std::vector<unsigned char> image;
	unsigned width, height;

	// Read the image
	unsigned error = lodepng::decode(image, width, height, filename);

	// Import to graphics card memory
	glGenTextures(1, &tex);						//Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate handle

	// Copy image to graphics cards memory represented by the active handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}


void Bus::draw_model(const Input& in)
{
	glm::mat4 P = glm::mat4(1.0f);
	glm::mat4 V = glm::mat4(1.0f);
	glm::mat4 M = glm::mat4(1.0f);
	
	P = P * in.P_scene;
	V = V * in.V_scene;
	
	
	M = glm::rotate(M, in.angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, in.angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(0.009f, 0.009f, 0.009));
	
	// Draw all meshes from meshes vector
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh temp_mesh = meshes[i];
		meshes[i].draw_mesh(P, V, M, temp_mesh, tex);
	}

}


void Grass::draw_model(const Input &in)
{
	glm::mat4 P = glm::mat4(1.0f);
	glm::mat4 V = glm::mat4(1.0f);
	glm::mat4 M = glm::mat4(1.0f);
	
	P = P * in.P_scene;
	V = V * in.V_scene;
	

	M = glm::translate(M, glm::vec3(0.0f, -1.0f, 0.0f));
	M = glm::rotate(M, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(0.1f, 0.1f, 0.1));
	
	// Draw all meshes from meshes vector
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh temp_mesh = meshes[i];
		meshes[i].draw_mesh(P, V, M, temp_mesh, tex);
	}
}

