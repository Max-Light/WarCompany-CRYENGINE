#pragma once

#include "BattleFormation/IFormationColumn.h"

struct IVertexPoint 
{
	IVertexPoint() = default;
	virtual ~IVertexPoint() = default;

	// Return the position of the vertex
	virtual Vec3 GetPos() const = 0;

	// Return the battle line x position local to the battle formation
	virtual float GetBattleLineXPos() const = 0;
};