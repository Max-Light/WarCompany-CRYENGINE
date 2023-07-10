#pragma once

#include "StdAfx.h"
#include "ITroopUnitBasicTypes.h"

#include <CryAISystem/Components/IEntityNavigationComponent.h>

struct STroopNavigationProperties
{
	IEntityNavigationComponent::SCollisionAvoidanceProperties navCollisionAvoidance;
	IEntityNavigationComponent::SMovementProperties navMovementProperties;
};

struct STroopUnitClassParams
{
	TroopId troopId;
	ETroopType troopType;
	STroopNavigationProperties navProperties;
};

struct ITroopUnitClass
{
	ITroopUnitClass() = default;
	virtual ~ITroopUnitClass() = default;

	// Return the troop unit identifier
	virtual TroopId GetTroopId() const = 0;

	// Return the dimension size of the troop
	virtual Vec3 GetSize() const = 0;

	// Return the bounding box of the troop unit
	virtual AABB GetAABB() const = 0;

	// Return the navigation properties of the troop unit
	virtual STroopNavigationProperties GetNavProperties() const = 0;
};