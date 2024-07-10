#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system) : m_DeviceContext(deviceContext), m_System(system) {}

DeviceContext::~DeviceContext()
{
	m_DeviceContext->Release();
}

void DeviceContext::ClearRenderTargetColor(SwapChainPtr swap_chain, float red, float green, float blue, float alpha)
{

	FLOAT clearColor[] = { red, green, blue, alpha };
	m_DeviceContext->ClearRenderTargetView(swap_chain->m_RenderTargetView, clearColor);
	m_DeviceContext->OMSetRenderTargets(1, &swap_chain->m_RenderTargetView, NULL);
}

void DeviceContext::SetVertexBuffer(VertexBufferPtr vertex_buffer)
{
	UINT stride = vertex_buffer->m_SizeVertex;
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, &vertex_buffer->m_Buffer, &stride, &offset);
	m_DeviceContext->IASetInputLayout(vertex_buffer->m_Layout);
}

void DeviceContext::SetIndexBuffer(IndexBufferPtr index_buffer)
{

	m_DeviceContext->IASetIndexBuffer(index_buffer->m_Buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::DrawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::DrawIndexTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->DrawIndexed(index_count, start_index_location, start_vertex_index);
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

void DeviceContext::SetVertexShader(VertexShaderPtr vertex_shader)
{
	m_DeviceContext->VSSetShader(vertex_shader->m_VertexShader, nullptr, 0);
}

void DeviceContext::SetPixelShader(PixelShaderPtr pixel_shader)
{
	m_DeviceContext->PSSetShader(pixel_shader->m_PixelShader, nullptr, 0);
}

void DeviceContext::SetConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer)
{
	m_DeviceContext->VSSetConstantBuffers(0, 1, &buffer->m_Buffer);
}

void DeviceContext::SetConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr buffer)
{
	m_DeviceContext->PSSetConstantBuffers(0, 1, &buffer->m_Buffer);
}
