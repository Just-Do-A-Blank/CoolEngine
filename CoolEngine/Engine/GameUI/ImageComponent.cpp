#include "ImageComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Managers/GraphicsManager.h"

ImageComponent::ImageComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation) : GameUIComponent(identifier, position, scale, rotation)
{
	
}

void ImageComponent::Init(wstring textureFilePath)
{
	ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(textureFilePath);

	if (psRV == nullptr)
	{
		cout << "Failed to set the albedo SRV as one with that name doesn't exist!" << endl;

		return;
	}
	m_ptexture = psRV;
}