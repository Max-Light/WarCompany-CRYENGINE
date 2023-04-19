#pragma once

#include "IBattleFormationColumn.h"
#include "FormationSlot.h"

class CBattleFormationColumn : public IBattleFormationColumn
{
public:
	using TSlotCollection = std::vector<CFormationSlot*>;
public:
	CBattleFormationColumn() = default;
	virtual ~CBattleFormationColumn() = default;

	// IFormationColumn
	virtual float GetXPos() const override { return m_xPosition; }
	virtual float GetWidth() const override { return m_width; }
	// ~IFormationColumn

	// Return a pointer to the collection of slots in the column
	const TSlotCollection* GetSlots() const { return &m_slots; }
	TSlotCollection* GetSlots() { return &m_slots; }

	// Set the x position of the column in the formation
	void SetXPos(float xPos) { m_xPosition = xPos; }

	// Set the width of the column
	void SetWidth(float width) { m_width = width; }
protected:
	TSlotCollection m_slots;
	float m_xPosition = 0;
	float m_width = 0;
};