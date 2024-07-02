#pragma once
#include "VertexBuffer.h"

class GraphicsEngine;
class DeviceContext;

class VertexShader
{
public:
	VertexShader();
	~VertexShader();

	void Release();

private:
	bool Init(const void* shader_byte_code, size_t byte_code_size);

	ID3D11VertexShader* m_VertexShader;

	friend class GraphicsEngine;
	friend class DeviceContext;
};

