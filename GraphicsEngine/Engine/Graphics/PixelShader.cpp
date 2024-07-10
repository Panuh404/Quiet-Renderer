#include "PixelShader.h"
#include <exception>
#include "Engine/Renderer/RenderSystem.h"

PixelShader::PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system) : m_System(system)
{
	if (!SUCCEEDED(m_System->m_D3DDevice->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_PixelShader)))
	{
		throw std::exception("ERROR::Pixel Shader - Creation Failed");
	}
}

PixelShader::~PixelShader()
{
	m_PixelShader->Release();
}