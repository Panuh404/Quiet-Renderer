#include "Texture.h"
#include <DirectXTex.h>

#include "Engine/GraphicsEngine.h"

Texture::Texture(const wchar_t* full_path) : Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_NONE, nullptr, image_data);
	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(
			GraphicsEngine::Get()->GetRenderSystem()->m_D3DDevice,
			image_data.GetImages(),
			image_data.GetImageCount(),
			image_data.GetMetadata(),
			&m_Texture
		);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::Get()->GetRenderSystem()->m_D3DDevice->CreateShaderResourceView(m_Texture, &desc, &m_ShaderResView);
	}
	else
	{
		throw std::exception("ERROR::Texture - Failed to load texture");
	}
}

Texture::~Texture()
{
	m_ShaderResView->Release();
	m_Texture->Release();
}
