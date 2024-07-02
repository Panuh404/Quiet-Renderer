#include "VertexShader.h"
#include "GraphicsEngine.h"

VertexShader::VertexShader() {}

VertexShader::~VertexShader() {}

bool VertexShader::Init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(GraphicsEngine::Get()->m_D3DDevice->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_VertexShader)))
	{
		return false;
	}	
	return true;
}

void VertexShader::Release()
{
	m_VertexShader->Release();
	delete this;
}
