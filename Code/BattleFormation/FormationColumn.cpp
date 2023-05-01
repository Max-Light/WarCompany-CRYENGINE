
#include "FormationColumn.h"

void CFormationColumn::SetXPos(float xPos)
{
    float diff = xPos - m_xPos;
    for (auto pSlot : m_slots)
    {
        pSlot->OffsetPos(Vec2(diff, 0));
    }
    m_xPos = xPos; 
}

void CFormationColumn::SetWidth(float width)
{
    m_width = width;
}

void CFormationColumn::InsertSlot(SlotCollection::iterator& slotItr, CFormationSlot* slot)
{
    m_slots.insert(slotItr, slot);
}

CFormationSlot* CFormationColumn::RemoveSlot(const SlotCollection::iterator& slotItr)
{
    CFormationSlot* pSlot = *slotItr;
    m_slots.erase(slotItr);
    return pSlot;
}

CFormationColumn::SSlotPos CFormationColumn::QuerySlotPos(uint depth, const SSlotSpawnParams& slotParams) const
{
    SlotCollection::iterator slotItr;
    std::advance(slotItr, std::distance<SlotCollection::const_iterator>(slotItr, m_slots.begin()));
    slotItr += depth;

    Vec2 gridPos;
    gridPos.x = m_xPos;
    gridPos.y = -(slotParams.slotSize.y / 2);
    if (slotItr != m_slots.begin())
    {
        SlotCollection::iterator prevSlotItr = slotItr - 1;
        gridPos.y += (*prevSlotItr)->GetPos().y - ((*prevSlotItr)->GetSize().y / 2);
    }

    SSlotPos slotPos;
    slotPos.slotItr = slotItr;
    slotPos.gridPos = gridPos;
    return slotPos;
}

void CFormationColumn::ShiftSlotsAt(SlotCollection::iterator& slotItr, float offset)
{
    for (slotItr; slotItr != m_slots.end(); ++slotItr)
    {
        (*slotItr)->OffsetPos(Vec2(0, offset));
    }
}

void CFormationColumn::IterateSlots(std::function<void(CFormationSlot& slot)> func) const
{
    for (auto pSlot : m_slots)
    {
        func(*pSlot);
    }
}

