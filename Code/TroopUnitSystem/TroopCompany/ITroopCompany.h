#pragma once

#include "ITroopCompanyClass.h"
#include "TroopUnitSystem/TroopUnit/ITroopUnit.h"

struct ITroopCompany
{
	ITroopCompany() = default;
	virtual ~ITroopCompany() = default;

	// Return the troop company class
	virtual ITroopCompanyClass* GetTroopCompanyClass() const = 0;

	// Return the troop unit class
	virtual ITroopUnitClass* GetTroopUnitClass() const = 0;

	// Return the world position of the troop company relative to the game world
	virtual Vec3 GetWorldPos() const = 0;

	// Return the total amount of troops in the troop company
	virtual uint GetTroopCount() const = 0;

	// Retrieve the troop line length 
	virtual uint GetLineLength() const = 0;

	// Retrieve the troop unit within the troop company grid
	// Return nullptr if no troop unit is found
	virtual ITroopUnit* GetTroopInGrid(Vec2i gridCoord) const = 0;

	// Return the size dimension of the troop company
	virtual Vec3 GetSize() const = 0;

	// Add the troop to the company grid
	virtual void AddTroop(ITroopUnit* pTroopUnit) = 0;

	// Remove the troop from the company grid
	virtual void RemoveTroop(ITroopUnit* pTroopUnit) = 0;
};