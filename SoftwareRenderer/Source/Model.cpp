#include "Model.h"
#include "Framebuffer.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool Model::Load(const std::string& filename)
{
	std::ifstream input(filename);

	if (!input.is_open())
	{
		std::cerr << "Error opening " << filename << std::endl;
		return false;
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	//std::vector<glm::vec2> uvs;
	std::string line;

	while (std::getline(input, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;
			
			vertices.push_back(position);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 normal;
			sstream >> normal.x;
			sstream >> normal.y;
			sstream >> normal.z;

			normals.push_back(normal);
		}
		/*if (line.substr(0, 3) == "vt ")
		{
			std::istringstream sstream{ line.substr(2) };
			glm::vec2 uv;
			sstream >> uv.x;
			sstream >> uv.y;

			uvs.push_back(uv);
		}*/
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream sstream{ line.substr(2) };
			std::string str;
			while (std::getline(sstream, str, ' '))
			{
				std::istringstream sstream(str);
				std::string indexString;

				size_t i = 0;
				unsigned int index[3] = { 0, 0, 0 };

				while (std::getline(sstream, indexString, '/'))
				{
					if (!indexString.empty())
					{
						std::istringstream indexStream{ indexString };
						indexStream >> index[i];
					}
					i++;
				}
				if (index[0] && index[1])
				{
					vertex_t vertex;
					vertex.position = vertices[index[0] - 1];
					vertex.normal = normals[index[1] - 1];
					//vertex.uv = uvs[index[1] - 1];

					m_vb.push_back(vertex);
				}
			}
		}
	}

	input.close();
	return true;
}

void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model, const Camera& camera)
{
	Shader::Draw(m_vb);

	// Convert point from model space to world space
	//for (int i = 0; i < m_vb.size(); i += 3)
	//{
	//	vertex_t p1 = model * glm::vec4{ m_vb[0 + i], 1 };
	//	vertex_t p2 = model * glm::vec4{ m_vb[1 + i], 1 };
	//	vertex_t p3 = model * glm::vec4{ m_vb[2 + i], 1 };

	//	// Convert point from world space to view space
	//	p1 = camera.ModelToView(p1);
	//	p2 = camera.ModelToView(p2);
	//	p3 = camera.ModelToView(p3);

	//	// Convert point from view space to projection space
	//	glm::ivec2 s1 = camera.ToScreen(p1);
	//	glm::ivec2 s2 = camera.ToScreen(p2);
	//	glm::ivec2 s3 = camera.ToScreen(p3);
	//	
	//	if (s1.x == -1 || s1.y == -1 || s2.x == -1 || s2.y == -1 || s3.x == -1 || s3.y == -1)
	//	{
	//		continue;
	//	}

	//	framebuffer.DrawTriangle(s1.x, s1.y, s2.x, s2.y, s3.x, s3.y, Color::ColorConvert(m_color));
	//}
}