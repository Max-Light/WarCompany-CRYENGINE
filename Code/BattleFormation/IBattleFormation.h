#pragma once

#include "IFormationSlot.h"
#include "IFormationColumn.h"
#include <CryEntitySystem/IEntityComponent.h>

struct IBattleFormation;

namespace FormationSystem
{
	enum class EFormationEvent : uint8
	{
		ADD_SLOT = BIT8(1),
		REMOVE_SLOT = BIT8(2),
		ADD_COLUMN = BIT8(3),
		REMOVE_COLUMN = BIT(4)
	};
	typedef CEnumFlags<EFormationEvent> FormationEventFlags;
	CRY_CREATE_ENUM_FLAG_OPERATORS(EFormationEvent);
}

using EBattleFormationEvent = FormationSystem::EFormationEvent;
struct SBattleFormationEvent
{
	SBattleFormationEvent(
		const EBattleFormationEvent _event,
		const intptr_t n0,
		const intptr_t n1)
		: event(_event)
	{
		nParam[0] = n0;
		nParam[1] = n1;
	}

	explicit SBattleFormationEvent(const EBattleFormationEvent _event)
		: event(_event)
	{
		nParam[0] = nParam[1] = 0;
	}

	EBattleFormationEvent event;
	intptr_t nParam[2];
};

struct IBattleFormationListener
{
	IBattleFormationListener() = default;
	virtual ~IBattleFormationListener() = default;

	inline void operator() (const SBattleFormationEvent& event) { this->OnBattleFormationEvent(event); }

	// Sent to the listener when an event has occured
	virtual void OnBattleFormationEvent(const SBattleFormationEvent& event) = 0;
};

struct IColumnIterator
{
	IColumnIterator() = default;
	virtual ~IColumnIterator() = default;

	virtual void Next() = 0;
	virtual IFormationColumn* GetColumn() const = 0;
};

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
	virtual IFormationSlot* GetSlot(uint columnIndex, uint slotIndex) const = 0;

	// Retrieves the column at the index
	virtual IFormationColumn* GetColumn(uint columnIndex) const = 0;

	// Retrieve the total amount of slots in the formation
	virtual uint GetSlotCount() const = 0;

	// Retrieve the total amount of columns
	virtual uint GetColumnCount() const = 0;

	// Retrieve the total amount of slots within the column
	virtual uint GetSlotCountInColumn(uint columnIndex) const = 0;

	// Return the formation line direction from the starting column to the ending column
	virtual Vec3 GetBattleLineDirection() const = 0;

	// Return the total battle line length
	virtual float GetBattleLineLength() const = 0;

	// Returns true if the formation has no slots
	virtual bool IsEmpty() const = 0;

	// Inserts a new column in the formation containing the inserted unit
	virtual IFormationSlot* InsertColumnAndUnit(uint column, IFormationUnit* pUnit, EColumnShiftType shiftType) = 0;

	// Inserts a unit within the specified column
	virtual IFormationSlot* InsertUnitInColumn(uint columnIndex, uint slotIndex, IFormationUnit* pUnit) = 0;

	// Remove the column and its containing slots at the specified index
	virtual void RemoveColumn(uint columnIndex) = 0;

	// Remove the column and its containing slots
	virtual bool RemoveColumn(IFormationColumn* pColumn) = 0;

	// Removes the slot at the specified index
	virtual void RemoveSlot(uint columnIndex, uint slotIndex) = 0;

	// Removes the slot from the formation
	virtual bool RemoveSlot(IFormationSlot* pSlot) = 0;

	// Clear the formation slots
	virtual void ClearSlots() = 0;

	// Translates a formation Vec2 grid position to a Vec3 local position
	virtual Vec3 CreatePos(const Vec2& gridPos) = 0;

	// Add a listener that can receive event data from this battle formation
	virtual void AddBattleFormationListener(IBattleFormationListener* listener) = 0;

	// Removes an existing listener from this battle formation
	virtual void RemoveBattleFormationListener(IBattleFormationListener* listener) = 0;
};