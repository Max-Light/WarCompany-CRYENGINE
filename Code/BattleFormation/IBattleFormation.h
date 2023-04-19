#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "BattleFormation/IFormationSlot.h"

struct IBattleFormation : public IEntityComponent
{
	static void ReflectType(Schematyc::CTypeDesc<IBattleFormation>& desc)
	{
		desc.SetGUID("{1B7B07DB-DCEA-4A06-96EF-114DF4EE8B29}"_cry_guid);
	}

	// Different shifting of columns within the formation
	enum class EColumnShiftType
	{
		Left,
		Right,
		Center
	};

	// Functions that spawn a slot require this parameter
	// Contains all properties in order to spawn a slot
	struct SSlotSpawnParams
	{
		Vec3 slotSize;
		IFormationUnit* pUnit = nullptr;
	};

	// Retrieves the slot
	virtual IFormationSlot* GetSlot(uint x, uint y) const = 0;

	// Retrieve the total amount of columns
	virtual uint GetColumnCount() const = 0;

	// Retrieve the slot count in the specified column
	virtual uint GetSlotCountInColumn(uint col) const = 0;

	// Retrieve the total amount of slots in the formation
	virtual uint GetSlotCount() const = 0;

	// Inserts a new column in the formation containing the inserted unit
	virtual IFormationSlot* InsertColumnAndUnit(uint col, SSlotSpawnParams& slotParams, EColumnShiftType shiftType) = 0;

	// Inserts a unit within the specified column
	virtual IFormationSlot* InsertUnitInColumn(uint col, uint depth, SSlotSpawnParams& slotParams) = 0;

	// Removes the slot at the specified index
	virtual void RemoveSlot(uint x, uint y) = 0;

	// Removes the slot from the formation
	virtual void RemoveSlot(IFormationSlot* pSlot) = 0;
};