#pragma once
#include "Engine/GameObjects/Gameplay/Player/EPlayerMovementState.h"

class PlayerMovementState
{
public:

	virtual bool Update(float timeDelta) = 0;

	virtual EPLAYERMOVEMENTSTATE NextState() = 0;

};