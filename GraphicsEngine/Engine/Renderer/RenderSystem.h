#pragma once
#include <d3d11.h>
#include "Engine/Prerequisites.h"

class RenderSystem
{
public:
	// Initialize Engine and DirectX 11 Device
	RenderSystem();
	// Release all resources loaded
	~RenderSystem();

	SwapChainPtr		CreateSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContextPtr	GetImmediateDeviceContext();

	VertexBufferPtr		CreateVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);
	IndexBufferPtr		CreateIndexBuffer(void* list_indices, UINT size_list);
	ConstantBufferPtr	CreateConstantBuffer(void* buffer, UINT size_buffer);

	VertexShaderPtr		CreateVertexShader(const void* shader_byte_code, size_t byte_Code_Size);
	PixelShaderPtr		CreatePixelShader(const void* shader_byte_code, size_t byte_Code_Size);

	bool CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void ReleaseCompiledShaders();

private:
	DeviceContextPtr m_IMMDeviceContext;

	ID3D11Device* m_D3DDevice;
	D3D_FEATURE_LEVEL m_FeatureLevel;

	IDXGIDevice* m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;
	ID3D11DeviceContext* m_IMMContext;

	ID3DBlob* m_Blob = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class Texture;
};