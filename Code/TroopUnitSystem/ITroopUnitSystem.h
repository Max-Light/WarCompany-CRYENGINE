#pragma once

#include "StdAfx.h"

#include "TroopUnit/ITroopUnitBasicTypes.h"
#include "TroopUnit/ITroopUnit.h"
#include "TroopUnit/ITroopUnitClass.h"

struct ITroopUnitSystem
{
	ITroopUnitSystem() = default;
	virtual ~ITroopUnitSystem() = default;

	// Spawns a troop unit
	virtual ITroopUnit* SpawnTroopUnit(const STroopUnitSpawnParams& troopUnitSpawnParams) = 0;

	// Return the troop unit class with the troop unit id
	// Returns nullptr if not found
	virtual ITroopUnitClass* GetTroopUnitClass(TroopId troopId) const = 0;
};