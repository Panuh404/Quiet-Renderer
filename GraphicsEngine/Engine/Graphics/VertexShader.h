#pragma once
#include "VertexBuffer.h"
#include "Engine/Prerequisites.h"

class VertexShader
{
public:
	VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
	~VertexShader();

private:
	ID3D11VertexShader* m_VertexShader;
	RenderSystem* m_System = nullptr;

	friend class RenderSystem;
	friend class DeviceContext;
};

