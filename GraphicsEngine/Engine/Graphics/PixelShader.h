#pragma once
#include "VertexBuffer.h"

class GraphicsEngine;
class DeviceContext;

class PixelShader
{
public:
	PixelShader();
	~PixelShader();

	void Release();

private:
	bool Init(const void* shader_byte_code, size_t byte_code_size);

	ID3D11PixelShader* m_PixelShader;

	friend class GraphicsEngine;
	friend class DeviceContext;
};

