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

	// Returns the position of the formation along the x and y plane
	virtual Vec2 GetPos() const = 0;

	// return the rotation of the formation
	virtual Quat GetRotation() const = 0;

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

	// Return the formation line direction from the starting column to the ending column
	virtual Vec3 GetBattleLineDirection() const = 0;

	// Return the total battle line length
	virtual float GetBattleLineLength() const = 0;

	// Returns true if the formation has no slots
	virtual bool IsEmpty() const = 0;

	// Inserts a new column in the formation containing the inserted unit
	virtual IFormationSlot* InsertColumnAndUnit(uint col, SSlotSpawnParams& slotParams, EColumnShiftType shiftType) = 0;

	// Inserts a unit within the specified column
	virtual IFormationSlot* InsertUnitInColumn(uint col, uint depth, SSlotSpawnParams& slotParams) = 0;

	// Removes the slot at the specified index
	virtual void RemoveSlot(uint x, uint y) = 0;

	// Removes the slot from the formation
	virtual void RemoveSlot(IFormationSlot* pSlot) = 0;

	// Translates a formation Vec2 grid position to a Vec3 local position
	virtual Vec3 CreatePos(const Vec2& pos) = 0;
};