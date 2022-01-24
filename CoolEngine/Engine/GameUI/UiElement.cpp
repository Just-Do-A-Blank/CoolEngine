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
	m_layer;
}

const bool& UiElement::IsRenderable()
{
	return m_isRenderable;
}
