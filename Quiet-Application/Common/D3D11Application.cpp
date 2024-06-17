#include "D3D11Application.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

using Position = DirectX::XMFLOAT3;
using Color = DirectX::XMFLOAT3;

struct VertexPositionColor
{
	Position position;
	Color color;
};

D3D11Application::D3D11Application(const std::string& title) : Application(title) {}

D3D11Application::~D3D11Application()
{
	_deviceContext->Flush();
	_pixelShader.Reset();
	_vertexShader.Reset();
	_vertexLayout.Reset();
	_triangleVertices.Reset();
	DestroySwapchainResources();
	_swapChain.Reset();
	_dxgiFactory.Reset();
	_deviceContext.Reset();
	_device.Reset();
}

bool D3D11Application::Initialize()
{
	// Initializes GLFW and creates a Window
	if (!Application::Initialize())
	{
		return false;
	}
	// Initializes DirectX's devices and SwapChain
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory))))
	{
		std::cerr << "DXGI: Failed to create factory\n";
		return false;
	}

	constexpr D3D_FEATURE_LEVEL deviceFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDevice(
		nullptr,					// Display Adapter Interface
		D3D_DRIVER_TYPE_HARDWARE,		// Driver Type
		nullptr,					// Software
		0,							// Runtime Flags
		&deviceFeatureLevel,			// Out Feature level
		1,					// Feature levels to attempt to create
		D3D11_SDK_VERSION,				// SDK Version
		&_device,						// Out Device created
		nullptr,				// Out Feature level
		&_deviceContext)))			// Out Device Context
	{
		std::cout << "D3D11: Failed to create device and device context\n";
		return false;
	}

	DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor = {};
	swapChainDescriptor.Width = GetWindowWidth();
	swapChainDescriptor.Height = GetWindowHeight();
	swapChainDescriptor.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDescriptor.SampleDesc.Count = 1;
	swapChainDescriptor.SampleDesc.Quality = 0;
	swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescriptor.BufferCount = 2;
	swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDescriptor.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
	swapChainDescriptor.Flags = {};

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDescriptor = {};
	swapChainFullscreenDescriptor.Windowed = true;

	if (FAILED(_dxgiFactory->CreateSwapChainForHwnd(
		_device.Get(),
		glfwGetWin32Window(GetWindow()),
		&swapChainDescriptor,
		&swapChainFullscreenDescriptor,
		nullptr,
		&_swapChain)))
	{
		std::cout << "DXGI: Failed to create SwapChain\n";
		return false;
	}

	if (!CreateSwapchainResources())
	{
		return false;
	}

	return true;
}

void D3D11Application::OnResize(int32_t width, int32_t height)
{
	Application::OnResize(width, height);
	_deviceContext->Flush();

	DestroySwapchainResources();
	if (FAILED(_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, 0)))
	{
		std::cout << "D3D11: Failed to recreate SwapChain buffers\n";
		return;
	}
	CreateSwapchainResources();
}

void D3D11Application::Render()
{
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(GetWindowWidth());
	viewport.Height = static_cast<float>(GetWindowHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	constexpr float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	constexpr UINT vertexStride = sizeof(VertexPositionColor);
	constexpr UINT vertexOffset = 0;

	_deviceContext->ClearRenderTargetView(_renderTarget.Get(), clearColor);
	_deviceContext->IASetInputLayout(_vertexLayout.Get());
	_deviceContext->IASetVertexBuffers(0, 1, _triangleVertices.GetAddressOf(), &vertexStride, &vertexOffset);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	_deviceContext->RSSetViewports(1, &viewport);
	_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
	_deviceContext->OMSetRenderTargets(1, _renderTarget.GetAddressOf(), nullptr);
	_deviceContext->Draw(3, 0);
	_swapChain->Present(1, 0);
}

void D3D11Application::Update()
{
	Application::Update();
}

bool D3D11Application::Load()
{
	ComPtr<ID3DBlob> vertexShaderBlob = nullptr;

	// VERTEX SHADER
	_vertexShader = CreateVertexShader(L"Assets/Shaders/main_vs.hlsl", vertexShaderBlob);
	if (_vertexShader == nullptr) return false;

	// PIXEL SHADER
	_pixelShader = CreatePixelShader(L"Assets/Shaders/main_ps.hlsl");
	if (_pixelShader == nullptr) return false;

	// INPUT LAYOUT DESCRIPTION
	constexpr D3D11_INPUT_ELEMENT_DESC vertexInputLayoutInfo[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			offsetof(VertexPositionColor, position),
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			offsetof(VertexPositionColor, color),
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	// INPUT LAYOUT
	if (FAILED(_device->CreateInputLayout(
		vertexInputLayoutInfo,
		_countof(vertexInputLayoutInfo),
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		&_vertexLayout
	)))
	{
		std::cerr << "D3D11: Failed to create default vertex input layout\n";
		return false;
	}

	// VERTEX BUFFER
	constexpr VertexPositionColor vertices[] = {
		{ Position{  0.0f,  0.5f, 0.0f }, Color{ 0.9f, 0.2f, 0.3f } },
		{ Position{  0.5f, -0.5f, 0.0f }, Color{ 0.3f, 0.9f, 0.2f } },
		{ Position{ -0.5f, -0.5f, 0.0f }, Color{ 0.2f, 0.3f, 0.9f } },
	};

	// BUFFER INFO DESCRIPTOR
	D3D11_BUFFER_DESC bufferInfo = {};
	bufferInfo.ByteWidth = sizeof(vertices);
	bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	// Save vertices to memory
	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = vertices;

	if (FAILED(_device->CreateBuffer(
		&bufferInfo,
		&resourceData,
		&_triangleVertices
	)))
	{
		std::cerr << "D3D11: Failed to create triangle vertex buffer\n";
		return false;
	}

	return true;
}

bool D3D11Application::CreateSwapchainResources()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	if (FAILED(_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))))
	{
		std::cerr << "D3D11: Failed to get Back Buffer from the SwapChain\n";
		return false;
	}
	if (FAILED(_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &_renderTarget)))
	{
		std::cerr << "D3D11: Failed to create RTV from Back Buffer\n";
		return false;
	}
	return true;
}

void D3D11Application::DestroySwapchainResources()
{
	_renderTarget.Reset();
}

bool D3D11Application::CompileShader(const std::wstring& fileName, const std::string& entryPoint,
	const std::string& profile, ComPtr<ID3DBlob>& shaderBlob) const
{
	constexpr UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ComPtr<ID3DBlob> tempShaderBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	if (FAILED(D3DCompileFromFile(
		fileName.data(),						// Name of the shader we want to compile
		nullptr,							// array of macros that we want to define
		D3D_COMPILE_STANDARD_FILE_INCLUDE,		// pointer to a ID3DInclude object, specify how to handle #include directives in shaders
		entryPoint.data(),			// contains the name of the main function in the shader
		profile.data(),					// Shader Model version to use for this shader
		compileFlags,						// flags that change how to compile our shaders
		0,								// other flag, ignored
		&tempShaderBlob,				// output, this is where our compiled code will be stored
		&errorBlob					// pointer to a ID3DBlob* that store our errors
	)))
	{
		std::cerr << "D3D11: Failed to read shader from file\n";
		if (errorBlob != nullptr)
		{
			std::cerr << "D3D11: With message: " << static_cast<const char*>(errorBlob->GetBufferPointer()) << "\n";
		}
		return false;
	}
	shaderBlob = std::move(tempShaderBlob);
	return true;
	
}

D3D11Application::ComPtr<ID3D11VertexShader> D3D11Application::CreateVertexShader(const std::wstring& fileName,
	ComPtr<ID3DBlob>& vertexShaderBlob) const
{
	if (!CompileShader(fileName, "main", "vs_5_0", vertexShaderBlob))
	{
		return nullptr;
	}

	ComPtr<ID3D11VertexShader> vertexShader;
	if (FAILED(_device->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		nullptr,
		&vertexShader
	)))
	{
		std::cerr << "D3D11: Failed to compile vertex shader\n";
		return nullptr;
	}
	return vertexShader;
}

D3D11Application::ComPtr<ID3D11PixelShader> D3D11Application::CreatePixelShader(const std::wstring& fileName) const
{
	ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
	if (!CompileShader(fileName, "main", "ps_5_0", pixelShaderBlob))
	{
		return nullptr;
	}

	ComPtr<ID3D11PixelShader> pixelShader;
	if (FAILED(_device->CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize(),
		nullptr,
		&pixelShader
	)))
	{
		std::cerr << "D3D11: Failed to compile pixel shader\n";
		return nullptr;
	}
	return pixelShader;
}
