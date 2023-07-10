#pragma once

#include "StdAfx.h"
#include "ITroopUnitClass.h"

struct STroopUnitSpawnParams
{
	TroopId troopId;
	Vec3 position;
	uint health;
};

struct ITroopUnit 
{
	ITroopUnit() = default;
	virtual ~ITroopUnit() = default;

	// Retrieve the troop ID
	virtual TroopId GetTroopId() const = 0;

	// Return the position of the troop unit relative to the game world
	virtual Vec3 GetWorldPos() const = 0;

	// Retrieve the dimension size of the troop unit
	virtual Vec3 GetSize() const = 0;

	// Return the amount of health the troop unit has
	virtual uint GetHealth() const = 0;

	// Set the health of the troop unit
	virtual void SetHealth(uint health) = 0;

	// Set the local position of the troop unit
	virtual void SetPos(Vec3 position) = 0;

	// Heal the troop unit by the amount of health
	virtual void Heal(uint healthAmount) = 0;

	// Damage the troop unit by the amount of damage
	virtual void Damage(uint damageAmount) = 0;

	// Navigate the troop unit to the designated position
	virtual void MoveToPosition(Vec3 position) = 0;
};