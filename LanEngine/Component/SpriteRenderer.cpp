#include "Defines.h"
#include "SpriteRenderer.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"
#include "../Graphics/GraphicsManager.h"

namespace Lan
{
	SpriteRenderer::SpriteRenderer() :
		m_Sprite(nullptr),
		m_TexCoordBuffer(nullptr)
	{
		ID3D11Device* device = GraphicsManager::getInstance().getDevice();
		HRESULT result;

		fvec2 vertices[4] =
		{
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		D3D11_BUFFER_DESC vbd;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.ByteWidth = sizeof(fvec2) * 4;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		vbd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA vrd;
		vrd.pSysMem = vertices;
		vrd.SysMemPitch = 0;
		vrd.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&vbd, &vrd, &m_TexCoordBuffer);
		if (FAILED(result))
		{
			LOG_WITH_TAG(LogLevel::Error, "DirectX", "Create TexCoordBuffer Error");
		}
	}

	SpriteRenderer::SpriteRenderer(tstring textureName) :
		m_Sprite(dynamic_cast<Texture *>(ResourceManager::getInstance().getResource(textureName)))
	{
		ID3D11Device* device = GraphicsManager::getInstance().getDevice();
		HRESULT result;

		fvec2 vertices[4] =
		{
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		D3D11_BUFFER_DESC vbd;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.ByteWidth = sizeof(fvec2) * 4;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		vbd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA vrd;
		vrd.pSysMem = vertices;
		vrd.SysMemPitch = 0;
		vrd.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&vbd, &vrd, &m_TexCoordBuffer);
		if (FAILED(result))
		{
			LOG_WITH_TAG(LogLevel::Error, "DirectX", "Create TexCoordBuffer Error");
		}
	}

	SpriteRenderer::~SpriteRenderer()
	{
		if (m_TexCoordBuffer)
		{
			m_TexCoordBuffer->Release();
			m_TexCoordBuffer = nullptr;
		}
	}

	void SpriteRenderer::onDraw()
	{
		ID3D11DeviceContext* deviceContext = GraphicsManager::getInstance().getDeviceContext();

		UINT stride = sizeof(fvec2);
		UINT offset = 0;

		if (m_Sprite)
		{
			ID3D11ShaderResourceView* tex = m_Sprite->getTextureView();
			deviceContext->PSSetShaderResources(0, 1, &tex);
		}

		deviceContext->IASetVertexBuffers(1, 1, &m_TexCoordBuffer, &stride, &offset);
	}

	void SpriteRenderer::setTexture(tstring name)
	{
		ResourceManager::getInstance().getResource(name);
	}

	Texture* SpriteRenderer::getTexture() const
	{
		return m_Sprite;
	}
}