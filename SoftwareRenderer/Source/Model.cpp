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
			std::istringstream sstream{ line.substr(3) };
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
				if (index[0])
				{
					vertex_t vertex;
					vertex.position = vertices[index[0] - 1];
					vertex.normal = (index[1]) ? normals[index[1] - 1] : glm::vec3{ 1 };
					//vertex.uv = uvs[index[1] - 1];

					m_vb.push_back(vertex);
				}
			}
		}
	}

	input.close();
	return true;
}

void Model::Draw()
{
	Shader::Draw(m_vb);
}