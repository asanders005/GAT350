#include "Actor.h"
#include "Shader.h"

void Actor::Draw(Framebuffer& framebuffer, const Camera& camera)
{
	VertexShader::uniforms.model = m_transform.GetMatrix();
	m_model->Draw(framebuffer, m_transform.GetMatrix(), camera);
}
