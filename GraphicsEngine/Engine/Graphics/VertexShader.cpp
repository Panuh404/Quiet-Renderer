#include "VertexShader.h"
#include <exception>
#include "Engine/Renderer/RenderSystem.h"

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system) : m_System(system)
{
	if (!SUCCEEDED(m_System->m_D3DDevice->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_VertexShader)))
	{
		throw std::exception("ERROR::Vertex Shader - Creation Failed");
	}
}

VertexShader::~VertexShader()
{
	m_VertexShader->Release();
}
