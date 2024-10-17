#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include <fstream>

void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model, const Camera& camera)
{
	// Convert point from model space to world space
	for (int i = 0; i < m_vertices.size(); i += 3)
	{
		vertex_t p1 = model * glm::vec4{ m_vertices[0 + i], 1 };
		vertex_t p2 = model * glm::vec4{ m_vertices[1 + i], 1 };
		vertex_t p3 = model * glm::vec4{ m_vertices[2 + i], 1 };

		// Convert point from world space to view space
		p1 = camera.ModelToView(p1);
		p2 = camera.ModelToView(p2);
		p3 = camera.ModelToView(p3);

		// Convert point from view space to projection space
		glm::ivec2 s1 = camera.ToScreen(p1);
		glm::ivec2 s2 = camera.ToScreen(p2);
		glm::ivec2 s3 = camera.ToScreen(p3);

		framebuffer.DrawTriangle(s1.x, s1.y, s2.x, s2.y, s3.x, s3.y, m_color);
	}
}

bool Model::Load(const std::string& filename)
{
	std::ifstream input;
	input.open(filename);

	while (!input.eof())
	{

	}

	return false;
}
