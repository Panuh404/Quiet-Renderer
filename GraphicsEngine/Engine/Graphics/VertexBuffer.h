#pragma once
#include <d3d11.h>
#include "windows.h"
#include "Engine/Prerequisites.h"

class VertexBuffer
{
public:
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader, RenderSystem* system);
	~VertexBuffer();

	UINT GetSizeVertexList();

private:
	UINT m_SizeVertex;
	UINT m_SizeList;

	ID3D11Buffer* m_Buffer;
	ID3D11InputLayout* m_Layout;
	RenderSystem* m_System = nullptr;

	friend class DeviceContext;
};

