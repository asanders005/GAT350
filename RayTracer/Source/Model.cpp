#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Triangle.h"
#include "Sphere.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Model::Update()
{
	for (size_t i = 0; i < m_local_vertices.size(); i++)
	{
		m_vb[i] = m_transform * glm::vec4{ m_local_vertices[i], 1 };
	}

	m_center = glm::vec3{ 0 };
	for (auto& vertex : m_vb)
	{
		m_center += vertex;
	}
	m_center /= (float)m_vb.size();

	m_radius = 0;
	for (auto& vertex : m_vb)
	{
		float radius = glm::length(vertex - m_center);
		m_radius = glm::max(radius, m_radius);
	}
}

bool Model::Load(const std::string& filename)
{
	std::ifstream input(filename);

	if (!input.is_open())
	{
		std::cerr << "Error opening " << filename << std::endl;
		return false;
	}

	vertexbuffer_t vertices;
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
					m_local_vertices.push_back(position);
				}
			}
		}
	}

	m_vb.resize(m_local_vertices.size());
	input.close();
	return true;
}

bool Model::Hit(const ray_t& ray, rayCastHit_t& rayCastHit, float minDistance, float maxDistance)
{
	//check for bounding sphere raycast

	// check cast ray with mesh triangles
	for (size_t i = 0; i < m_vb.size(); i += 3)
	{
		float t;
		if (!Sphere::Raycast(ray, m_center, m_radius, minDistance, maxDistance, t)) return false;
		if (Triangle::Raycast(ray, m_vb[i], m_vb[i + 1], m_vb[i + 2], minDistance, maxDistance, t))
		{
			rayCastHit.distance = t;
			rayCastHit.point = ray.At(t);
			glm::vec3 edge1 = m_vb[i + 1] - m_vb[i];
			glm::vec3 edge2 = m_vb[i + 2] - m_vb[i];
			rayCastHit.normal = glm::normalize(glm::cross(edge1, edge2));
			rayCastHit.material = GetMaterial();
			return true;
		}
	}

    return false;
}