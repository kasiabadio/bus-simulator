#include "Mesh.h"


void Mesh::write_mesh(aiMesh *_mesh)
{
	// Process vertices, normals and texture coordinates for a mesh
	for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
	{
		verts.emplace_back(glm::vec4(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z, 1));
		norms.emplace_back(glm::vec4(_mesh->mNormals[i].x, _mesh->mNormals[i].y, _mesh->mNormals[i].z, 0));
		texture_coordinates.emplace_back(glm::vec2(_mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y));
	}

	for (unsigned int j = 0; j < _mesh->mNumFaces; j++)
	{
		aiFace& face = _mesh->mFaces[j];
		
		for (int f = 0; f < face.mNumIndices; f++)
		{
			indices.push_back(face.mIndices[f]);
		}
	}
	
}


