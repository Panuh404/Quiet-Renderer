#pragma once
#include <d3d11.h>
#include "windows.h"
#include "Engine/Prerequisites.h"

class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);
	~IndexBuffer();

	UINT GetSizeIndexList();

private:
	UINT m_SizeList;
	ID3D11Buffer* m_Buffer;
	RenderSystem* m_System = nullptr;

	friend class DeviceContext;
};

