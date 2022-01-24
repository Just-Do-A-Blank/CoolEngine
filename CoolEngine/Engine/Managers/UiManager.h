#pragma once
#include "Engine/Structure/Singleton.h"

class UiCanvas;

class UiManager :
    public Singleton<UiManager>
{
private:
    unordered_map<string, UiCanvas*> m_uiCanvasMap;
public:
    void CreateCanvas(string canvasIdentifier);
    void CreateImage();
    void CreateTextBox();
    void CreateButton();

};

