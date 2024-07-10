#pragma once
#include "VertexBuffer.h"
#include "Engine/Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system);
	~ConstantBuffer();

	void Update(DeviceContextPtr context, void* buffer);

private:
	ID3D11Buffer* m_Buffer;
	RenderSystem* m_System = nullptr;

	friend class DeviceContext;
};

