#include "UiElement.h"

void UiElement::Render(RenderStruct& renderStruct)
{
}

void UiElement::Update()
{
}

void UiElement::SetIsRenderable(bool& condition)
{
	m_isRenderable = condition;
}

int& UiElement::GetLayer()
{
	return m_layer;
}

const bool& UiElement::IsRenderable()
{
	return m_isRenderable;
}

Transform* UiElement::GetTransform()
{
	return m_transform;
}

const string& UiElement::GetIdentifier()
{
	return m_uiElementIdentifier;
}
