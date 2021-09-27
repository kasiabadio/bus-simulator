
#include "Mesh.h"

Mesh::Mesh()
{}


void Mesh::draw_mesh(glm::mat4 P, glm::mat4 V, glm::mat4 M, Mesh& _mesh, const GLuint& _tex) const
{
	spLambertTextured->use();
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glEnableVertexAttribArray(spLambertTextured->a("normal"));

	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));
	
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, _mesh.mesh_verts.data());
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, _mesh.mesh_texture_coordinates.data());
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, _mesh.mesh_norms.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex);
	glUniform1i(spLambertTextured->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, _mesh.mesh_indices.size(), GL_UNSIGNED_INT, _mesh.mesh_indices.data());

	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("color"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	
}


void Mesh::draw_mesh_x2(glm::mat4 P, glm::mat4 V, glm::mat4 M, Mesh& _mesh, const GLuint& _tex, const GLuint& _tex1) const
{
	spLambertMultiTextured->use();
	glUniformMatrix4fv(spLambertMultiTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertMultiTextured->u("V"), 1, false, glm::value_ptr(V));
	glEnableVertexAttribArray(spLambertMultiTextured->a("vertex"));
	glEnableVertexAttribArray(spLambertMultiTextured->a("texCoord"));
	glEnableVertexAttribArray(spLambertMultiTextured->a("normal"));

	glUniformMatrix4fv(spLambertMultiTextured->u("M"), 1, false, glm::value_ptr(M));

	glVertexAttribPointer(spLambertMultiTextured->a("vertex"), 4, GL_FLOAT, false, 0, _mesh.mesh_verts.data());
	glVertexAttribPointer(spLambertMultiTextured->a("texCoord"), 2, GL_FLOAT, false, 0, _mesh.mesh_texture_coordinates.data());
	glVertexAttribPointer(spLambertMultiTextured->a("normal"), 4, GL_FLOAT, false, 0, _mesh.mesh_norms.data());

	glUniform1i(spLambertMultiTextured->u("tex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex);
	

	glUniform1i(spLambertMultiTextured->u("tex1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _tex1);
	

	glDrawElements(GL_TRIANGLES, _mesh.mesh_indices.size(), GL_UNSIGNED_INT, _mesh.mesh_indices.data());

	glDisableVertexAttribArray(spLambertMultiTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertMultiTextured->a("color"));
	glDisableVertexAttribArray(spLambertMultiTextured->a("normal"));

}
