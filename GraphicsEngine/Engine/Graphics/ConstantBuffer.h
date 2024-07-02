#pragma once
#include "VertexBuffer.h"

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	bool Load(void* buffer, UINT size_buffer);
	void Update(DeviceContext* context, void* buffer);
	bool Release();

private:
	ID3D11Buffer* m_Buffer;

	friend class DeviceContext;
};

