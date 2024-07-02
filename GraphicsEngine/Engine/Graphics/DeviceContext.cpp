#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext) : m_DeviceContext(deviceContext) {}

DeviceContext::~DeviceContext()
{
}

void DeviceContext::ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{

	FLOAT clearColor[] = { red, green, blue, alpha };
	m_DeviceContext->ClearRenderTargetView(swap_chain->m_RenderTargetView, clearColor);
	m_DeviceContext->OMSetRenderTargets(1, &swap_chain->m_RenderTargetView, NULL);
}

void DeviceContext::SetVertexBuffer(VertexBuffer* vertex_buffer)
{
	UINT stride = vertex_buffer->m_SizeVertex;
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, &vertex_buffer->m_Buffer, &stride, &offset);
	m_DeviceContext->IASetInputLayout(vertex_buffer->m_Layout);
}

void DeviceContext::DrawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_DeviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::SetViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	
	m_DeviceContext->RSSetViewports(1, &viewport);
}

void DeviceContext::SetVertexShader(VertexShader* vertex_shader)
{
	m_DeviceContext->VSSetShader(vertex_shader->m_VertexShader, nullptr, 0);
}

void DeviceContext::SetPixelShader(PixelShader* pixel_shader)
{
	m_DeviceContext->PSSetShader(pixel_shader->m_PixelShader, nullptr, 0);
}

bool DeviceContext::Release()
{
	m_DeviceContext->Release();
	delete this;
	return true;
}
