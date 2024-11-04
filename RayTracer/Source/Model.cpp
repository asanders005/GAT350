#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Triangle.h"
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

	vertices_t vertices;
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
					glm::vec3 position = vertices[index[0] - 1];
					m_vertices.push_back(position);
				}
			}
		}
	}

	input.close();
	return true;
}

bool Model::Hit(const ray_t& ray, rayCastHit_t& rayCastHit, float minDistance, float maxDistance)
{
	for (size_t i = 0; i < m_vertices.size(); i += 3)
	{
		Triangle triangle{ m_vertices[i + 0], m_vertices[i + 1], m_vertices[i + 2], m_material};
		if (triangle.Hit(ray, rayCastHit, minDistance, maxDistance))
		{
			return true;
		}
	}

    return false;
}