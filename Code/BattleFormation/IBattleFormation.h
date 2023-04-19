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
	struct SSlotSpawnBaseParam
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

	// Parameters to be passed to the InsertColumnAndUnit function
	struct SColumnUnitInsertionParam : public SSlotSpawnBaseParam
	{
		uint col;
		EColumnShiftType shiftType = EColumnShiftType::Right;
	};
	// Inserts a new column in the formation containing the inserted unit
	virtual IFormationSlot* InsertColumnAndUnit(SColumnUnitInsertionParam& insertionParams) = 0;

	struct SUnitInsertionParam : public SSlotSpawnBaseParam
	{
		uint col;
		uint depth;
	};
	// Inserts a unit within the specified column
	virtual IFormationSlot* InsertUnitInColumn(SUnitInsertionParam& insertionParams) = 0;

	// Removes the slot at the specified index
	virtual void RemoveSlot(uint x, uint y) = 0;

	// Removes the slot from the formation
	virtual void RemoveSlot(IFormationSlot* pSlot) = 0;
};