#include "VertexShader.h"

VertexShader::uniforms_t VertexShader::uniforms =
{
	glm::mat4{ 1 }, //Model
	glm::mat4{ 1 }, //View
	glm::mat4{ 1 }, //Projection
	color3_t{ 1 }
};

void VertexShader::Process(const vertex_t& ivertex, vertex_output_t& overtex)
{
	glm::mat4 mvp = uniforms.projection * uniforms.view * uniforms.model;

	overtex.position = mvp * glm::vec4{ ivertex.position, 1 };
	overtex.color = uniforms.ambient;
}
