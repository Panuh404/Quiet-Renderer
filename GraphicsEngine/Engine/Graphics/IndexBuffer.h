#pragma once
#include <d3d11.h>
#include "windows.h"

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	bool Load(void* list_indices, UINT size_list);
	bool Release();

	UINT GetSizeIndexList();

private:
	UINT m_SizeList;

	ID3D11Buffer* m_Buffer;

	friend class DeviceContext;
};

