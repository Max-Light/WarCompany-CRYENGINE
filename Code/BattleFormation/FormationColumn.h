#pragma once

#include "IFormationColumn.h"
#include "FormationSlot.h"

typedef CFormationSlot Slot;
typedef std::vector<Slot*> SlotCollection;

class CFormationColumn : public IFormationColumn
{
public:
	struct SColumnProperties
	{
		void Serialize(Serialization::IArchive& archive)
		{
			archive(slotProperties, "Slots", "Slots");
			archive(xPos, "XPosition", "X Position");
			archive(width, "Width", "Width");
		}

		std::vector<Slot::SSlotProperties> slotProperties;
		float xPos;
		float width;
	};

	typedef SlotCollection::const_iterator SlotItr;
public:
	CFormationColumn() = default;
	CFormationColumn(const SlotCollection& slots, float xPos, float width)
		: m_slots(slots)
		, m_xPos(xPos)
		, m_width(width)
	{
	}
	virtual ~CFormationColumn() override = default;

	CFormationSlot* operator[](uint i) const { return m_slots[i]; }

	// IFormationColumn
	inline virtual float GetXPos() const override { return m_xPos; }
	inline virtual float GetWidth() const override { return m_width; }
	inline virtual float GetYPos() const override { return 0; }
	virtual void SetColumnVerticalOffset(const IColumnVerticalOffsetEffect& verticalEffect) override;
	// ~IFormationColumn

	// Return the starting slot iterator
	inline SlotItr GetStartSlot() const { return m_slots.begin(); }

	// Return the ending slot iterator
	inline SlotItr GetEndSlot() const { return m_slots.end(); }

	// Return the corresponding slot iterator 
	inline SlotItr FindSlot(IFormationSlot* slot) const { return std::find(GetStartSlot(), GetEndSlot(), slot); }

	// Return the slot at the specified index
	inline CFormationSlot* GetSlotAt(const uint index) const { return m_slots[index]; }

	// Return the number of slots
	inline uint GetSlotCount() const { return m_slots.size(); }

	// Checks if the column is empty
	inline bool IsEmpty() const { return m_slots.empty(); }

	// Checks if the X horizontal position is within the specifiec range of the column [minX, maxX)
	inline bool IsRanging(float x) const { return x >= GetMinXPos() && x < GetMaxXPos(); }

	// Set the x position of the column
	void SetXPos(float xPos);

	// Set the width of the column
	void SetWidth(float width);

	// Insert the slot into the formation column
	inline void InsertSlot(uint index, CFormationSlot* slot);

	// Remove the slot at the slot iterator position
	inline bool RemoveSlot(const SlotItr& slotItr);

	// Remove the slot from the column
	inline bool RemoveSlot(const IFormationSlot* pSlot);

	// Remove the slot at the specified depth from the column
	inline CFormationSlot* RemoveSlot(const uint index);

	// Clear all slots in the column
	inline void Clear();

	// Return the relative position given the slot parameters
	Vec2 QuerySlotGridPos(uint depth, const Vec3& slotSize) const;

	// Iterate over all slots in the column
	void IterateSlots(std::function<void(CFormationSlot&)> func) const;

	// Return the column properties
	SColumnProperties GetColumnProperties() const;
protected:
	// Shift Slots up and down the column by an offset
	void ShiftSlotsAt(SlotCollection::const_iterator& slotItr, float offset);
protected:
	SlotCollection m_slots;
	float m_xPos = 0;
	float m_width = 0;
};