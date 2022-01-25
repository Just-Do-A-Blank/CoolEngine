#pragma once
class EditorUIComponent
{
public:
#if EDITOR
	virtual void CreateEngineUI() = 0;
#endif
protected:

private:

};

