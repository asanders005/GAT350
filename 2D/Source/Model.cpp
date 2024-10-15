#include "Model.h"
#include "Framebuffer.h"

void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model)
{
	for (int i = 0; i < m_vertices.size(); i += 3)
	{
		vertex_t p1 = model * glm::vec4{ m_vertices[0 + i], 1 };
		vertex_t p2 = model * glm::vec4{ m_vertices[1 + i], 1 };
		vertex_t p3 = model * glm::vec4{ m_vertices[2 + i], 1 };

		framebuffer.DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, m_color);
	}
}
