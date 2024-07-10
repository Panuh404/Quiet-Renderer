#pragma once
#include "VertexBuffer.h"
#include "Engine/Prerequisites.h"

class PixelShader
{
public:
	PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
	~PixelShader();

private:
	ID3D11PixelShader* m_PixelShader;
	RenderSystem* m_System = nullptr;

	friend class RenderSystem;
	friend class DeviceContext;
};

