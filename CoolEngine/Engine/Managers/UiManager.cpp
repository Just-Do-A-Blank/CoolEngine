#include "UiManager.h"
#include "Engine/GameUI/UiElement.h"
#include "Engine/GameUI/UiCanvas.h"

void UiManager::CreateCanvas(string canvasIdentifier)
{
	UiCanvas* canvas = new UiCanvas(canvasIdentifier);
	m_uiCanvasMap.insert(pair<string, UiCanvas*>(canvasIdentifier, canvas));
}

void UiManager::CreateImage()
{
}

void UiManager::CreateTextBox()
{
}

void UiManager::CreateButton()
{
}
