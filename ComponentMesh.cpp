#include "ComponentMesh.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include <string>

ComponentMesh::ComponentMesh() : Component(componentType_Mesh)
{
}

ComponentMesh::ComponentMesh(GameObject* argparent) : Component(componentType_Mesh, argparent)
{
}

ComponentMesh::ComponentMesh(componentType argtype, GameObject * argparent) : Component(argtype, argparent)
{
}

void ComponentMesh::Update(float dt)
{
	if (visible) {
		ComponentMaterial* mat = (ComponentMaterial*)this->GetParent()->FindComponents(componentType_Material)[0];
		Draw(mat);
	}

}

void ComponentMesh::Draw(const ComponentMaterial * mat)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	if (mat != nullptr) {
		//Apply UV if exist
		if (num_UV != 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, id_UV);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}

		//glEnable(GL_TEXTURE_2D);
		if (mat->GetTextureChannel(texType_Diffuse) != nullptr)
			glBindTexture(GL_TEXTURE_2D, (GLuint)mat->GetTextureChannel(texType_Diffuse)->id);

	}

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void ComponentMesh::DrawInspectorPanel()
{
		std::string str = "Triangles: " + std::to_string(this->num_triangles);
		ImGui::Text(str.c_str());
		str = "Vertices: " + std::to_string(this->num_vertices);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Indices: " + std::to_string(this->num_indices);
		ImGui::Text(str.c_str());
		ImGui::Text("Scale:");
		str = "X: " + std::to_string(this->transform.scale.x);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Y: " + std::to_string(this->transform.scale.y);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Z: " + std::to_string(this->transform.scale.z);
		ImGui::Text(str.c_str());
}
