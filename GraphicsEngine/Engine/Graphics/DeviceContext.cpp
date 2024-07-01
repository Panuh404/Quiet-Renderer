#include "DeviceContext.h"
#include "SwapChain.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext) : m_DeviceContext(deviceContext) {}

DeviceContext::~DeviceContext()
{
}

bool DeviceContext::ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{

	FLOAT clearColor[] = { red, green, blue, alpha };
	m_DeviceContext->ClearRenderTargetView(swap_chain->m_RenderTargetView, clearColor);
	return true;
}

bool DeviceContext::Release()
{
	m_DeviceContext->Release();
	delete this;
	return true;
}
