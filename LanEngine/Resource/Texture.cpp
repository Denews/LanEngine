#include "Defines.h"
#include "Texture.h"
#include "../Utils/Utils.h"
#include "../Graphics/GraphicsManager.h"
#include <DirectXTex.h>


namespace Lan
{
	Texture::Texture(void * buffer, tsize len, tstring path, tstring extension) :
		Resource(buffer, len, path, extension),
		m_Buffer(nullptr),
		m_View(nullptr)
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		DirectX::TexMetadata info;
		auto image = std::make_unique<DirectX::ScratchImage>();
		HRESULT result = DirectX::LoadFromWICMemory(buffer, len, 0, &info, *image);

		if (FAILED(result))
		{
			LOG(LogLevel::Error, "이미지 로드 실패");
		}

		DirectX::CreateShaderResourceView(device, image->GetImages(), image->GetImageCount(), image->GetMetadata(), &m_View);
	}

	Texture::~Texture()
	{
		if (m_View)
		{
			m_View->Release();
			m_View = nullptr;
		}

		if (m_Buffer)
		{
			m_Buffer->Release();
			m_Buffer = nullptr;
		}
	}

	int32 Texture::GetWidth() const
	{
		return m_Width;
	}

	int32 Texture::GetHeight() const
	{
		return m_Height;
	}

	int32 Texture::GetChannel() const
	{
		return m_Channel;
	}

	ID3D11Texture2D * Texture::GetTextureBuffer() const
	{
		return m_Buffer;
	}

	ID3D11ShaderResourceView* Texture::GetTextureView() const
	{
		return m_View;
	}
}