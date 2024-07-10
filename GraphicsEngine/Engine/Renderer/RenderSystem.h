#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	bool Init();		// Initialize Engine and DirectX 11 Device
	bool Release();		// Release all resources loaded

	SwapChain* CreateSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContext* GetImmediateDeviceContext();

	VertexBuffer* CreateVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);
	IndexBuffer* CreateIndexBuffer(void* list_indices, UINT size_list);
	ConstantBuffer* CreateConstantBuffer(void* buffer, UINT size_buffer);

	VertexShader* CreateVertexShader(const void* shader_byte_code, size_t byte_Code_Size);
	PixelShader* CreatePixelShader(const void* shader_byte_code, size_t byte_Code_Size);

	bool CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void ReleaseCompiledShaders();

private:
	DeviceContext* m_IMMDeviceContext;

	ID3D11Device* m_D3DDevice;
	D3D_FEATURE_LEVEL m_FeatureLevel;

	IDXGIDevice* m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;
	ID3D11DeviceContext* m_IMMContext;

	ID3DBlob* m_Blob = nullptr;

	// Temp
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11PixelShader* m_ps = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
};