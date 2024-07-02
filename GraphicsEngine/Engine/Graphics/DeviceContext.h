#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext);
	~DeviceContext();

	void ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void SetVertexBuffer(VertexBuffer* vertex_buffer);
	void DrawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void SetViewportSize(UINT width, UINT height);

	bool Release();

private:
	ID3D11DeviceContext* m_DeviceContext;
};

