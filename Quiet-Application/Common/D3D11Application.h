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

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGIFactory2> _dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain1> _swapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> _renderTarget = nullptr;
};

