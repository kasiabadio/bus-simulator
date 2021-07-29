#include "Model.h"
Assimp::Importer importer;

Model::Model(std::string filepath):
	scene(importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes))
	// root_node(scene->mRootNode)
	
{
	std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;
	tex = readTexture("res/textures/Bus_tex.tga.png");
	//read_model();
	
}


void Model::read_model()
{
	// Read all meshes of a root node of a model and read recursively its nodes
	std::cout << "root_node->mNumChildren: " << root_node->mNumChildren << std::endl;
	std::cout << "root_node->mNumMeshes: " << root_node->mNumMeshes << std::endl;
	for (int i = 0; i < root_node->mNumMeshes; i++)
	{
		Mesh mesh;
		mesh.write_mesh(scene->mMeshes[root_node->mMeshes[i]]);

	}
	std::cout << "Meshes.size: " << meshes.size() << std::endl;
	for (int j = 0; j < root_node->mNumChildren; j++)
	{
		read_model_node(root_node->mChildren[j]);
	}
	
}


void Model::read_model_node(aiNode* model_node)
{
	// Read all meshes of a node of a model and read recursively its nodes
	std::cout << "model_node->mNumMeshes: " << model_node->mNumMeshes << std::endl;
	for (int i = 0; i < model_node->mNumMeshes; i++)
	{
		Mesh mesh;
		mesh.write_mesh(scene->mMeshes[model_node->mMeshes[i]]);
		meshes.push_back(mesh);
	}
	std::cout << "Meshes.size: " << meshes.size() << std::endl;
	
	for (int j = 0; j < model_node->mNumChildren; j++)
	{
		read_model_node(model_node->mChildren[j]);
	}
}

// TEMPORARY
void Model::loadModel()
{
	for (unsigned int l = 0; l < scene->mNumMeshes; l++)
	{
		aiMesh* mesh = scene->mMeshes[l];
		std::cout << scene->mNumMeshes << std::endl;
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			
			aiVector3D vertex = mesh->mVertices[i];
			verts.emplace_back(vertex.x, vertex.y, vertex.z, 1);

			// w=0 bo jest to kierunek, a nie pozycja
			aiVector3D normal = mesh->mNormals[i];
			norms.emplace_back(normal.x, normal.y, normal.z, 0);

			// mamy tutaj akurat tylko 0 zestaw, wspó³rzêdnych dwuwymiarowych
			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			texCoords.emplace_back(texCoord.x, texCoord.y);

			
		}

		// musimy pobraæ tablicê indeksów, ¿eby wiedzieæ co my z tych wierzcho³ków budujemy
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}

		}
	}
	
}


GLuint Model::readTexture(const char* filename) {

	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Read into computers memory
	std::vector<unsigned char> image;   //Allocate memory 
	unsigned width, height;   //Variables for image size
	//Read the image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import to graphics card memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate handle
	//Copy image to graphics cards memory reprezented by the active handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}




void Model::draw_model(float angle_x, float angle_y) const
{

	glm::mat4 M = glm::mat4(1.0f);

	M = glm::rotate(M, angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
	
	M = glm::scale(M, glm::vec3(0.009f, 0.009f, 0.009));
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 100.0f);

	spLambertTextured->use();
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glEnableVertexAttribArray(spLambertTextured->a("normal"));

	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));
	
	/*
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, meshes[0].read_verts().data());
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, meshes[0].read_texture_coordinates().data());
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, meshes[0].read_norms().data());
	 */
	
	
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, verts.data());
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords.data());
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, norms.data());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spLambertTextured->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	//glDrawElements(GL_TRIANGLES, meshes[0].read_indices().size(), GL_UNSIGNED_INT, meshes[0].read_indices().data());

	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("color"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));


}
