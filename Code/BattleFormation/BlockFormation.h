#pragma once

#include "BattleFormation/FormationSlot.h"
#include "BattleFormation/IFormationUnit.h"


using TSlot = typename CFormationSlot;
using TFormationGrid = typename std::vector<std::vector<TSlot*>>;

class CBlockFormation 
{
public:
	CBlockFormation() = default;
	virtual ~CBlockFormation() = default;

	// retrieves the slot at the specified index on the 2D array
	TSlot* GetSlot(int row, int column) const { return m_slots[row][column]; }

	// retrieves the slot at the specified row index and depth in the battle line row
	// returns nullptr if a slot is not found within the battle line depth
	TSlot* GetSlot(int row, float depth) const;

	// returns the battle line depth of the slot located in the collection of slots
	float GetSlotDepth(int row, int column) const;

	// returns the indexed column of the slot located on the specified row and length from the first slot located on the same row
	// returns -1 if no slot was found
	int GetSlotColumn(int row, float length) const;

	// returns the battle line length from the first slot to the last slot
	float GetBattleLineLength(int row = 0) const;

	// returns the sum total lengths of specified battle line row
	float GetBattleLineDepth(int row = 0) const;

	// returns the slot with the specified formation unit
	TSlot* GetSlotOfUnit(const IFormationUnit* unit);

	// Constructs a slot for the unit and reformats the formation
	virtual void AddUnit(const IFormationUnit* unit, const Vec2& position);

	// Removes the slot containing the unit from the formation and reconstructs the formation
	virtual void RemoveUnit(const IFormationUnit* unit);

	// Removes the slot from the formation and reconstructs the formation
	void RemoveSlot(const TSlot* slot);
	void RemoveSlotInRow(const TSlot* slot, int row);
private:
	TFormationGrid m_slots;
	IEntity* m_pEntity = nullptr;
};