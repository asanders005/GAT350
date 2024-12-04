#include "FragmentShader.h"
#include "Shader.h"

color4_t FragmentShader::Process(const fragment_input_t& fragment)
{
#pragma region PointLight
	//glm::vec3 vPosition = mv * glm::vec4{ fragment.position, 1 };
	glm::vec3 lightPos = Shader::uniforms.view * glm::vec4{ Shader::uniforms.light.position, 1 };
	glm::vec3 lightDir = glm::normalize(lightPos - fragment.position);

#pragma endregion
#pragma region DirectionLight
	//glm::vec3 lightDir = uniforms.view * glm::vec4{ uniforms.light.direction, 0 };
	//lightDir = glm::normalize(glm::mat3{ -1 } * lightDir);

#pragma endregion

	float intensity = std::max(0.0f, glm::dot(lightDir, fragment.normal));
	color3_t diffuse = Shader::uniforms.light.color * intensity;

	color3_t specular = color3_t{ 0 };
	if (intensity > 0)
	{
		glm::vec3 reflection = glm::reflect(-lightDir, fragment.normal);
		glm::vec3 viewDir = glm::normalize(-fragment.position);
		intensity = std::max(glm::dot(reflection, viewDir), 0.0f);
		intensity = std::pow(intensity, Shader::uniforms.material.shininess);
		specular = Shader::uniforms.material.specular * intensity;
	}

	color3_t color = ((Shader::uniforms.ambient + diffuse) * Shader::uniforms.material.albedo) + specular;

	return { color, 1 };
}