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

	glm::mat4 mv = uniforms.view * uniforms.model;
	overtex.normal = glm::normalize(glm::mat3{ mv } * ivertex.normal);

	glm::vec3 lightPosition{ 3, 3, -3 };
	glm::vec3 vPosition = mv * glm::vec4{ ivertex.position, 1 };
	glm::vec3 lightDir = lightPosition - vPosition;

	float intensity = std::max(0.0f, glm::dot(lightDir, overtex.normal));
	color3_t lightColor = { 0.8f, 0.1f, 0.4f };
	color3_t diffuse = lightColor * intensity;

	overtex.color = diffuse;
}
