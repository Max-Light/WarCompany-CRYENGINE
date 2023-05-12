
#include "FormationColumn.h"

void CFormationColumn::SetXPos(float xPos)
{
    float diff = xPos - m_xPos;
    for (auto pSlot : m_slots)
    {
        Vec2 gridPos = pSlot->GetGridPos();
        gridPos.x += diff;
        pSlot->SetPos(gridPos);
    }
    m_xPos = xPos; 
}

void CFormationColumn::SetWidth(float width)
{
    m_width = width;
}

void CFormationColumn::InsertSlot(uint depth, CFormationSlot* pSlot)
{
    auto slotItr = m_slots.begin() + depth;
    ShiftSlotsAt(slotItr, -pSlot->GetSize().y);
    m_slots.insert(slotItr, pSlot);
}

CFormationSlot* CFormationColumn::RemoveSlot(const SlotCollection::iterator& slotItr)
{
    CFormationSlot* pSlot = *slotItr;
    m_slots.erase(slotItr);
    return pSlot;
}

Vec2 CFormationColumn::QuerySlotGridPos(uint depth, const Vec3& slotSize) const
{
    auto slotItr = m_slots.begin() + depth;
    Vec2 gridPos = Vec2(m_xPos, -(slotSize.y / 2));
    if (slotItr != m_slots.begin())
    {
        auto prevSlotItr = slotItr - 1;
        gridPos.y += (*prevSlotItr)->GetGridPos().y - ((*prevSlotItr)->GetSize().y / 2);
    }
    return gridPos;
}

void CFormationColumn::IterateSlots(std::function<void(CFormationSlot& slot)> func) const
{
    for (auto pSlot : m_slots)
    {
        func(*pSlot);
    }
}

void CFormationColumn::ShiftSlotsAt(SlotCollection::const_iterator& slotItr, float offset)
{
    for (slotItr; slotItr != m_slots.end(); ++slotItr)
    {
        auto pSlot = (*slotItr);
        Vec2 gridPos = pSlot->GetGridPos();
        gridPos.y += offset;
        pSlot->SetPos(gridPos);
    }
}

