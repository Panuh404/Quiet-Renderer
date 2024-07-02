#include "PixelShader.h"
#include "GraphicsEngine.h"

PixelShader::PixelShader() {}

PixelShader::~PixelShader() {}

bool PixelShader::Init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(GraphicsEngine::Get()->m_D3DDevice->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_PixelShader)))
	{
		return false;
	}
	return true;
}

void PixelShader::Release()
{
	m_PixelShader->Release();
	delete this;
}
