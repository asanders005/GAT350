#include "Actor.h"
#include "Shader.h"
#include "Material.h"

void Actor::Draw()
{
	VertexShader::uniforms.model = m_transform.GetMatrix();
	VertexShader::uniforms.material = *m_material;
	m_model->Draw();
}
