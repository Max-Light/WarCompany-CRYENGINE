#pragma once

#include <CryEntitySystem/IEntityComponent.h>

#include "IFormationSlot.h"
#include "IFormationColumn.h"

class CColumnIterator;

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

	// Retrieves the slot at the index
	virtual IFormationSlot* GetSlot(uint x, uint y) const = 0;

	// Retrieves the column at the index
	virtual IFormationColumn* GetColumn(uint x) const = 0;

	// Return the first iterator element of the column collection
	virtual CColumnIterator GetBeginColumn() = 0;

	// Return the last iterator element of the column collection
	virtual CColumnIterator GetEndColumn() = 0;

	// Retrieve the total amount of columns
	virtual uint GetColumnCount() const = 0;

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