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

	// Return the position of the slot 
	virtual Vec3 GetPos() const = 0;

	// Returns true if the formation unit is on the slots position
	virtual bool IsFormationReady() const = 0;
};

// Functions that spawn a slot require this parameter
// Contains all properties in order to spawn a slot
struct SSlotSpawnParams
{
	Vec3 slotSize;
	IFormationUnit* pUnit = nullptr;
};