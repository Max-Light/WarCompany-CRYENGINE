#pragma once

#include "StdAfx.h"
#include "BattleFormation/IFormationUnit.h"

struct IFormationSlot
{
	virtual ~IFormationSlot() = default;

	// Return the formation unit belonging to this slot
	virtual IFormationUnit* GetUnit() const = 0;

	// Return the size of the slot
	virtual Vec3 GetSize() const = 0;

	// Return the position of the slot in local space
	virtual Vec3 GetPos() const = 0;

	// Returns true if the formation unit is on the slots position
	virtual bool IsFormationReady() const = 0;
};