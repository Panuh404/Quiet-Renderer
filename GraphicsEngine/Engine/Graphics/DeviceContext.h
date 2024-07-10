#pragma once
#include <d3d11.h>
#include "Engine/Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system);
	~DeviceContext();

	void ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void DrawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void DrawIndexTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void SetViewportSize(UINT width, UINT height);
	void SetVertexShader(VertexShader* vertex_shader);
	void SetPixelShader(PixelShader* pixel_shader);

	void SetVertexBuffer(VertexBuffer* vertex_buffer);
	void SetIndexBuffer(IndexBuffer* index_buffer);
	void SetConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
	void SetConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer);

private:
	ID3D11DeviceContext* m_DeviceContext;
	RenderSystem* m_System = nullptr;

	friend class ConstantBuffer;
};

