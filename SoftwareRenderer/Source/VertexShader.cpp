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

#pragma region PointLight
	glm::vec3 vPosition = mv * glm::vec4{ ivertex.position, 1 };
	glm::vec3 lightPos = uniforms.view * glm::vec4{ uniforms.light.position, 1 };
	glm::vec3 lightDir = glm::normalize(lightPos - vPosition);

#pragma endregion
#pragma region DirectionLight
	//glm::vec3 lightDir = uniforms.view * glm::vec4{ uniforms.light.direction, 0 };
	//lightDir = glm::normalize(glm::mat3{ -1 } * lightDir);

#pragma endregion

	float intensity = std::max(0.0f, glm::dot(lightDir, overtex.normal));
	color3_t diffuse = uniforms.light.color * intensity;

	color3_t specular = color3_t{ 0 };
	if (intensity > 0)
	{
		glm::vec3 reflection = glm::reflect(-lightDir, overtex.normal);
		glm::vec3 viewDir = glm::normalize(-vPosition);
		intensity = std::max(glm::dot(reflection, viewDir), 0.0f);
		intensity = std::pow(intensity, uniforms.material.shininess);
		specular = uniforms.material.specular * intensity;
	}

	overtex.color = ((uniforms.ambient + diffuse) * uniforms.material.albedo) + specular;
}
