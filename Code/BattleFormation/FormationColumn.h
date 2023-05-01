#pragma once

#include "IFormationColumn.h"
#include "FormationSlot.h"

class CFormationColumn : public IFormationColumn
{
public:
	typedef std::vector<CFormationSlot*> SlotCollection;
public:
	CFormationColumn() = default;
	virtual ~CFormationColumn() = default;

	CFormationSlot* operator[](uint i) const { return m_slots[i]; }

	// IFormationColumn
	virtual float GetXPos() const override { return m_xPos; }
	virtual float GetWidth() const override { return m_width; }
	// ~IFormationColumn

	// Return the beginning iterator
	SlotCollection::const_iterator GetBeginSlot() { return m_slots.begin(); }

	// Return the ending iterator
	SlotCollection::const_iterator GetEndSlot() { return m_slots.end(); }

	// Return the number of slots
	uint GetSlotCount() const { return m_slots.size(); }

	// Set the x position of the column
	void SetXPos(float xPos);

	// Set the width of the column
	void SetWidth(float width);

	// Insert the slot into the formation column
	void InsertSlot(SlotCollection::iterator& slotItr, CFormationSlot* slot);

	// Remove the slot from the formation column
	CFormationSlot* RemoveSlot(const SlotCollection::iterator& slotItr);

	// Return value of the QuerySlotPos function
	struct SSlotPos
	{
		SlotCollection::iterator slotItr;
		Vec2 gridPos;
	};
	SSlotPos QuerySlotPos(uint depth, const SSlotSpawnParams& slotParams) const;

	// Shift Slots up and down the column by an offset
	void ShiftSlotsAt(SlotCollection::iterator& slotItr, float offset);

	// Iterate over all slots in the column
	void IterateSlots(std::function<void(CFormationSlot&)> func) const;
protected:
	SlotCollection m_slots;
	float m_xPos = 0;
	float m_width = 0;
};