#include "Model.h"


Assimp::Importer importer;

Model::Model(std::string filepath) :
	scene(importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes))
	// root_node(scene->mRootNode)

{
	//std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;
	tex = readTexture("res/textures/Sting_Base_Color.png");
	//read_model();

}


void Model::read_model()
{
	for (unsigned int l = 0; l < scene->mNumMeshes; l++)
	{
		aiMesh* mesh = scene->mMeshes[l];
		Mesh temp_mesh;
		
		std::cout << scene->mNumMeshes << std::endl;
		for (int i = 0; i < mesh->mNumVertices; i++)
		{

			aiVector3D vertex = mesh->mVertices[i];

			temp_mesh.mesh_verts.emplace_back(vertex.x, vertex.y, vertex.z, 1);

			// w=0 bo jest to kierunek, a nie pozycja
			aiVector3D normal = mesh->mNormals[i];
			temp_mesh.mesh_norms.emplace_back(normal.x, normal.y, normal.z, 0);

			// mamy tutaj akurat tylko 0 zestaw, współrzędnych dwuwymiarowych
			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			temp_mesh.mesh_texture_coordinates.emplace_back(texCoord.x, texCoord.y);


		}

		// musimy pobrać tablicę indeksów, żeby wiedzieć co my z tych wierzchołków budujemy
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
	std::cout << "Number of meshes: " << meshes.size() << std::endl;
}


Mesh::Mesh()
{}

void Mesh::draw_mesh(glm::mat4 P, glm::mat4 V, glm::mat4 M, Mesh &_mesh, const GLuint &_tex) const
{
	spLambertTextured->use();
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glEnableVertexAttribArray(spLambertTextured->a("normal"));

	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));


	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, _mesh.mesh_verts.data());
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, _mesh.mesh_norms.data());
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, _mesh.mesh_texture_coordinates.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex);
	glUniform1i(spLambertTextured->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, _mesh.mesh_indices.size(), GL_UNSIGNED_INT, _mesh.mesh_indices.data());

	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("color"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));

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
	M = glm::scale(M, glm::vec3(0.09f, 0.09f, 0.09));
	
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 100.0f);

	
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh temp_mesh = meshes[i];
		meshes[i].draw_mesh(P, V, M, temp_mesh, tex);
	}

}
