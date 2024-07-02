#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext);
	~DeviceContext();

	void ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void SetVertexBuffer(VertexBuffer* vertex_buffer);
	void DrawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void SetViewportSize(UINT width, UINT height);
	void SetVertexShader(VertexShader* vertex_shader);
	void SetPixelShader(PixelShader* pixel_shader);

	void SetConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
	void SetConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer);

	bool Release();

private:
	ID3D11DeviceContext* m_DeviceContext;

	friend class ConstantBuffer;
};

