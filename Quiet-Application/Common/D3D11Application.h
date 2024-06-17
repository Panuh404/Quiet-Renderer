#pragma once

#include "Application.h"

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

class D3D11Application final : public Application
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	D3D11Application(const std::string& title);
	~D3D11Application() override;

protected:
	bool Initialize() override;
	bool Load() override;
	void OnResize(int32_t width, int32_t height) override;
	void Render() override;
	void Update() override;

private:
	bool CreateSwapchainResources();
	void DestroySwapchainResources();

	// Core function for compiling shaders
	bool CompileShader(	
		const std::wstring& fileName,	// Path of the shader file we want to compile
		const std::string& entryPoint,	// name of the function where the shader begins execution
		const std::string& profile,		// HLSL version to use
		ComPtr<ID3DBlob>& shaderBlob)	// buffer where our compiled code will be stored
		const;

	[[nodiscard]] ComPtr<ID3D11VertexShader> CreateVertexShader(const std::wstring& fileName,
																ComPtr<ID3DBlob>& vertexShaderBlob) const;

	[[nodiscard]] ComPtr<ID3D11PixelShader> CreatePixelShader(const std::wstring& fileName) const;

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGIFactory2> _dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain1> _swapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> _renderTarget = nullptr;
	ComPtr<ID3D11Buffer> _triangleVertices = nullptr;
	ComPtr<ID3D11InputLayout> _vertexLayout = nullptr;
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
};

