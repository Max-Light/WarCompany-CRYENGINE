#pragma once

#include "ISplinePoint.h"

struct IVertexPoint : public ISplinePoint
{
	IVertexPoint() = default;
	virtual ~IVertexPoint() = default;

	// Return the battle line x position local to the battle formation
	virtual float GetBattleLineXPos() const = 0;
};