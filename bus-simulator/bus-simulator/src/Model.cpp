#include "Model.h"

Assimp::Importer importer;

Input::Input()
{}

Input::Input(float angle_x, float angle_y, glm::mat4 P_scene, glm::mat4 V_scene, glm::mat4 M_scene):
	angle_x(angle_x), angle_y(angle_y), P(P_scene), V(V_scene), M(M_scene)
{}

Input::Input(glm::mat4 P_scene, glm::mat4 V_scene, glm::mat4 M_scene, bool czy_box):
	angle_x(0.0f), angle_y(0.0f), P(P_scene), V(V_scene), M(M_scene), czy_box(czy_box)
{}


Model::Model(const char* model_file, const char* model_texture, const char* model_texture1):
	scene(importer.ReadFile(model_file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes))
{
	std::cout << "Reading ... " << model_file << std::endl;

	// Read texture 
	if (strcmp(model_texture, "null") != 0)
	{
		tex = write_model_texture(model_texture);
	}
	
	if (strcmp(model_texture1, "null") != 0)
	{
		tex1 = write_model_texture(model_texture1);
	}

	if (strcmp(model_file, "res/models/Bus.obj") == 0)
	{
		box = Utility::create_box(model_file);
		std::cout << "bus box created " << std::endl;
		
	}
	else if (strcmp(model_file, "res/models/MapleTreeStem.obj") == 0)
	{
		box = Utility::create_box(model_file);
		std::cout << "tree box created " << std::endl;
	}

	
}


void Model::write_model()
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


Input Model::read_model_matrices()
{
	return Input(P, V, M, false);
}


GLuint Model::write_model_texture(const char* filename)
{

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

// TRANSFORMATIONS OF A RECTANGLE
// needed for SAP
void Model::rotate_around_x_rectangle(float angle, std::vector<struct xyz> &edges)
{
	for (int i = 0; i < edges.size(); i++)
	{
		struct xyz temp_edges = edges[i];
		edges[i] = box.rotate_around_x(angle, temp_edges);
	}
}

void Model::rotate_around_y_rectangle(float angle, std::vector<struct xyz> &edges)
{
	for (int i = 0; i < edges.size(); i++)
	{
		struct xyz temp_edges = edges[i];
		edges[i] = box.rotate_around_y(angle, temp_edges);
	}
}

void Model::rotate_around_z_rectangle(float angle, std::vector<struct xyz> &edges)
{
	for (int i = 0; i < edges.size(); i++)
	{
		struct xyz temp_edges = edges[i];
		edges[i] = box.rotate_around_z(angle, temp_edges);
	}
}

void Model::scale_rectangle(glm::vec3 vector, std::vector<struct xyz> &edges)
{
	for (int i = 0; i < edges.size(); i++)
	{
		struct xyz temp_edges = edges[i];
		edges[i] = box.scale(vector, temp_edges);
	}
}

void Model::translate_rectangle(glm::vec3 vector, std::vector<struct xyz> &edges)
{

	for (int i = 0; i < edges.size(); i++)
	{
		struct xyz temp_edges = edges[i];
		edges[i] = box.translate(vector, temp_edges);
	}
}

void Model::print_rectangle_coords() const
{
	// for each tree (for example) draw it's base coordinates
	for (int i = 0; i < rectangles.size(); i++)
	{
		std::cout << "a: " << rectangles[i][0].x << " " <<
			rectangles[i][0].y << " " <<
			rectangles[i][0].z << " " << std::endl <<

			"b: " << rectangles[i][1].x << " " <<
			rectangles[i][1].y << " " <<
			rectangles[i][1].z << std::endl <<

			"c: " << rectangles[i][2].x << " " <<
			rectangles[i][2].y << " " <<
			rectangles[i][2].z << std::endl <<

			"d: " << rectangles[i][3].x << " " <<
			rectangles[i][3].y << " " <<
			rectangles[i][3].z << std::endl << std::endl;
	}
	std::cout << std::endl;
}

///// END OF TRANSFORMATIONS OF A RECTANGLE

void Model::draw_relative_to_terrain(const Input& in)
{
	this->P = glm::mat4(1.0f);
	this->V = glm::mat4(1.0f);
	this->M = glm::mat4(1.0f);
	P = in.P;
	V = in.V;
	
	// Every move per one object (tree/grass)
	for (int m = 0; m < moves.size(); m++)
	{
		for (int t = 0; t < moves[m].translate_vectors.size(); t++)
		{
			M = glm::translate(M, moves[m].translate_vectors[t]);
		}

		for (int s = 0; s < moves[m].scale_vectors.size(); s++)
		{
			M = glm::scale(M, moves[m].scale_vectors[s]);
		}

		for (int r = 0; r < moves[m].rotate_vectors.size(); r++)
		{
			M = glm::rotate(M, moves[m].rotate_angles[r], moves[m].rotate_vectors[r]);
		}

		// Draw all meshes from meshes vector
		for (int i = 0; i < meshes.size(); i++)
		{
			
			Mesh temp_mesh = meshes[i];
			meshes[i].draw_mesh(P, V, M, temp_mesh, tex);
		}
		
		if (in.czy_box) 
		{
			box.draw_bounding_box(P, V, M);
		}
		
		this->M = glm::mat4(1.0f);
		
	}
}

void Bus::write_model_static_transformations()
{}

void Bus::draw_model(const Input& in)
{
	
	this->P = glm::mat4(1.0f);
	this->V = glm::mat4(1.0f);
	this->M = glm::mat4(1.0f);
	
	P = in.P;
	V = in.V;
	
	M = glm::translate(M, glm::vec3(-1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, in.angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, in.angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(0.009f, 0.009f, 0.009f));
	
	// Draw all meshes from meshes vector
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh temp_mesh = meshes[i];
		meshes[i].draw_mesh(P, V, M, temp_mesh, tex);
	}

	box.draw_bounding_box(P, V, M);

	std::vector<struct xyz> temp_edges_copy = box.edges;
	std::vector<struct xyz> temp_edges = temp_edges_copy;
	translate_rectangle(glm::vec3(-1.0f, 0.0f, 0.0f), temp_edges);
	rotate_around_y_rectangle(in.angle_y, temp_edges);
	rotate_around_x_rectangle(in.angle_x, temp_edges);
	scale_rectangle(glm::vec3(0.009f, 0.009f, 0.009f), temp_edges);

	rectangles.emplace_back(temp_edges);

}

void Tree::draw_model(const Input& in)
{}

void Tree::write_model_static_transformations()
{
	Move temp_move;

	temp_move.translate_vectors.emplace_back(-7.0f, 0.0f, 0.0f);
	temp_move.scale_vectors.emplace_back(0.3f, 0.3f, 0.3f);
	moves.push_back(temp_move);
	clear_Move(temp_move);

	temp_move.translate_vectors.emplace_back(-7.0f, 0.0f, -3.0f);
	temp_move.scale_vectors.emplace_back(0.3f, 0.3f, 0.3f);
	moves.push_back(temp_move);
	clear_Move(temp_move);

	temp_move.translate_vectors.emplace_back(-8.6f, 0.0f, -8.0f);
	temp_move.scale_vectors.emplace_back(0.3f, 0.3f, 0.3f);
	moves.push_back(temp_move);
	clear_Move(temp_move);

	temp_move.translate_vectors.emplace_back(-8.6f, 0.0f, -13.0f);
	temp_move.scale_vectors.emplace_back(0.3f, 0.3f, 0.3f);
	moves.push_back(temp_move);
	clear_Move(temp_move);

	temp_move.translate_vectors.emplace_back(-8.6f, 0.0f, -38.0f);
	temp_move.scale_vectors.emplace_back(0.3f, 0.3f, 0.3f);
	moves.push_back(temp_move);
	clear_Move(temp_move);

	// base of the box static transformation for collision
	std::vector<struct xyz> temp_edges_copy = box.edges;

	for (int m = 0; m < moves.size(); m++)
	{

		std::vector<struct xyz> temp_edges = temp_edges_copy;
		box.edges = temp_edges_copy;
		
		for (int t = 0; t < moves[m].translate_vectors.size(); t++)
		{
			translate_rectangle(moves[m].translate_vectors[t], temp_edges);
		}

		for (int s = 0; s < moves[m].scale_vectors.size(); s++)
		{
			scale_rectangle(moves[m].scale_vectors[s], temp_edges);
		}

		for (int r = 0; r < moves[m].rotate_vectors.size(); r++)
		{
			if (moves[m].rotate_vectors[r].x != 0) rotate_around_x_rectangle(moves[m].rotate_angles[r], temp_edges);
			else if (moves[m].rotate_vectors[r].y != 0) rotate_around_y_rectangle(moves[m].rotate_angles[r], temp_edges);
			else if (moves[m].rotate_vectors[r].z != 0) rotate_around_z_rectangle(moves[m].rotate_angles[r], temp_edges);
		}
		rectangles.emplace_back(temp_edges);
	}
	
	std::cout << "TREES COORDS: " << std::endl;
	print_rectangle_coords();
	
}


void Road::draw_model(const Input& in)
{}


void Road::write_model_static_transformations()
{
	Move temp_move;

	temp_move.translate_vectors.emplace_back(-7.0f, 0.0f, 0.0f);
	temp_move.translate_vectors.emplace_back(0.0f, 0.0f, -35.0f);
	temp_move.rotate_vectors.emplace_back(0.0f, 1.0f, 0.0f);
	temp_move.rotate_angles.emplace_back(glm::radians(90.0f));
	temp_move.scale_vectors.emplace_back(0.01f, 0.01f, 0.01f);

	moves.push_back(temp_move);
	clear_Move(temp_move);
	
	temp_move.translate_vectors.emplace_back(-7.0f, 0.0f, 0.0f);
	temp_move.translate_vectors.emplace_back(0.0f, 0.0f, -20.0f);
	temp_move.rotate_vectors.emplace_back(0.0f, 1.0f, 0.0f);
	temp_move.rotate_angles.emplace_back(glm::radians(90.0f));
	temp_move.scale_vectors.emplace_back(0.01f, 0.01f, 0.01f);

	moves.push_back(temp_move);
	clear_Move(temp_move);
	
	temp_move.translate_vectors.emplace_back(-7.0f, 0.0f, 0.0f);
	temp_move.translate_vectors.emplace_back(0.0f, 0.0f, -4.0f);
	temp_move.rotate_vectors.emplace_back(0.0f, 1.0f, 0.0f);
	temp_move.rotate_angles.emplace_back(glm::radians(90.0f));
	temp_move.scale_vectors.emplace_back(0.01f, 0.01f, 0.01f);

	moves.push_back(temp_move);
	clear_Move(temp_move);
	
	temp_move.translate_vectors.emplace_back(-7.0f, 0.0f, 0.0f);
	temp_move.translate_vectors.emplace_back(0.0f, 0.0f, 5.0f);
	temp_move.rotate_vectors.emplace_back(0.0f, 1.0f, 0.0f);
	temp_move.rotate_angles.emplace_back(glm::radians(90.0f));
	temp_move.scale_vectors.emplace_back(0.01f, 0.01f, 0.01f);

	moves.push_back(temp_move);
	clear_Move(temp_move);
}


void Grass::write_model_static_transformations()
{
	Move temp_move;
	srand(time(NULL));
	float temp_z = -2.0f;
	float temp_x = -8.0f;
	float temp_floating = 0.0f;
	int i;
	
	// Left side of the road
	for (i = 0; i < 30; i++)
	{
		temp_move.translate_vectors.emplace_back(temp_x + temp_floating, 0.0f, temp_z + temp_floating);
		temp_move.scale_vectors.emplace_back(0.015f, 0.015f, 0.015f);

		moves.push_back(temp_move);
		clear_Move(temp_move);
		// range -10.0f to -7.0f
		temp_x = -(rand() % 7 + 7.0f);
		temp_floating = (rand() % 100 + 1) / 100;
		temp_z = -(rand() % 40 + 2.0f);
		//std::cout << temp_x << " " << temp_z << " " << std::endl;
	}

	// Right side of the road
	for (i = 0; i < 110; i++)
	{
		temp_move.translate_vectors.emplace_back(temp_x + temp_floating, 0.0f, temp_z + temp_floating);
		temp_move.scale_vectors.emplace_back(0.015f, 0.015f, 0.015f);
		
		moves.push_back(temp_move);
		clear_Move(temp_move);
		// range -14.0f to -7.0f
		temp_x = rand() % 20 + 3.0f;
		temp_floating = (rand() % 100 + 1) / 100;
		temp_z = -(rand() % 60 - 20.0f);
		//std::cout << temp_x << " " << temp_z << " " << std::endl;
	}	
}


void Grass::draw_model(const Input& in)
{}


void Rock::draw_model(const Input& in)
{
	this->P = glm::mat4(1.0f);
	this->V = glm::mat4(1.0f);
	this->M = glm::mat4(1.0f);

	P = in.P;
	V = in.V;

	M = glm::translate(M, glm::vec3(2.0f, 0.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::scale(M, glm::vec3(0.02f, 0.02f, 0.02f));

	// Draw all meshes from meshes vector
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh temp_mesh = meshes[i];
		meshes[i].draw_mesh_x2(P, V, M, temp_mesh, tex, tex1);
	}

}

void Rock::write_model_static_transformations()
{
	
}

//////////////////////////////

Terrain::Terrain(const char* model_texture, int height, int width):
	terrain_height(height), terrain_width(width)
{
	tex = write_model_texture(model_texture);
	std::cout << "Read terrain texture ..." << std::endl;


	for (int row = 0; row < terrain_height; row++)
	{
		int col;
		// adding a row of vertices
		for (col = 0; col < terrain_width; col++) {
			// x, y, z, 1

			//std::cout << random_num << std::endl;
			terrain_verts.emplace_back(col, row, 0, 1);
			terrain_norms.emplace_back(0, 1, 0, 0);
			//std::cout << col << " " << row << std::endl;
		}

		// adding a row of indices
		for (col = 0; col < terrain_width - 1; col++)
		{
			terrain_indices.emplace_back(col + row * terrain_width);
			terrain_indices.emplace_back(col + row * terrain_width + 1);
			terrain_indices.emplace_back(col + terrain_width * (row + 1));

			/*std::cout << "BLUE INDICES" << std::endl;
			std::cout << col + row * terrain_width << " " <<
				col + row * terrain_width + 1 << " " <<
				col + terrain_width * (row + 1) << std::endl;*/
		}

		for (col = terrain_width - 1; col > 0; col--)
		{
			terrain_indices.emplace_back(col + row * terrain_width);
			terrain_indices.emplace_back(col + terrain_width * (row + 1) - 1);
			terrain_indices.emplace_back(col + terrain_width * (row + 1));


			/*std::cout << "GREEN INDICES" << std::endl;
			std::cout << col + row * terrain_width << " " <<
				col + terrain_width * (row + 1) - 1 << " " <<
				col + terrain_width * (row + 1) << std::endl;*/
		}

		// adding a row of texture coordinates
		if (row % 2 == 0)
		{
			for (col = 0; col < terrain_width; col += 2)
			{
				terrain_texture_coordinates.emplace_back(0, 0);
				terrain_texture_coordinates.emplace_back(1, 0);
			}

		}
		else
		{
			for (col = 0; col < terrain_width; col += 2)
			{
				terrain_texture_coordinates.emplace_back(0, 1);
				terrain_texture_coordinates.emplace_back(1, 1);

			}
		}
	}
	/*std::cout << terrain_verts.size() << std::endl;
	std::cout << terrain_texture_coordinates.size() << std::endl;*/

}


void Terrain::draw_terrain(const Input& in)
{

	this->P = glm::mat4(1.0f);
	this->V = glm::mat4(1.0f);
	this->M = glm::mat4(1.0f);

	P = in.P;
	V = in.V;

	M = glm::translate(M, glm::vec3(-50.0f, 0.0f, -45.0f));
	M = glm::rotate(M, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(10.0f, 10.0f, 10.0f));
	
	spLambertTextured->use();
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glEnableVertexAttribArray(spLambertTextured->a("normal"));

	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));
	
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, terrain_verts.data());
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, terrain_texture_coordinates.data());
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, terrain_norms.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spLambertTextured->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, terrain_indices_count(), GL_UNSIGNED_INT, terrain_indices.data());

	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("color"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));

}

Input Terrain::read_model_matrices()
{
	return Input(P, V, M, false);
}


int Terrain::terrain_indices_count() const
{
	return terrain_height * terrain_width * 3;
}


int Terrain::terrain_vertices_count() const
{
	return (terrain_width * terrain_height) + (terrain_width - 1) * (terrain_height - 2);
}


GLuint Terrain::write_model_texture(const char* filename)
{

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return tex;
}

