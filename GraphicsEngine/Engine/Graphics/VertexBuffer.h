#pragma once
#include <d3d11.h>
#include "windows.h"

class DeviceContext;

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	bool Load(void* list_vertices,UINT size_vertex,UINT size_list, void* shader_byte_code, UINT size_byte_shader);
	bool Release();

	UINT GetSizeVertexList();

private:
	UINT m_SizeVertex;
	UINT m_SizeList;

	ID3D11Buffer* m_Buffer;
	ID3D11InputLayout* m_Layout;

	friend class DeviceContext;
};
