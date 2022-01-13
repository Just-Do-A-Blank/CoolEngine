#pragma once

#include <vector>

struct Frame
{
	float m_frameTime;
	ID3D11ShaderResourceView* m_ptexture;
};

class SpriteAnimation
{
	std::vector<Frame> m_frames;

	bool m_Interpolate = false;

public:
	void SetFrames(std::vector<Frame>& frames);
	void SetInterpolate(bool interpolate);

	ID3D11ShaderResourceView* GetCurrentFrame();
};