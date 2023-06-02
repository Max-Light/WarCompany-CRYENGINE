
#include "FormationColumn.h"

void CFormationColumn::SetColumnVerticalOffset(const IColumnVerticalOffsetEffect& verticalEffect)
{

}

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

void CFormationColumn::InsertSlot(uint index, CFormationSlot* pSlot)
{
    SlotCollection::iterator slotItr = m_slots.begin() + index;
    ShiftSlotsAt(slotItr, -pSlot->GetSize().y);
    m_slots.insert(slotItr, pSlot);
}

bool CFormationColumn::RemoveSlot(const SlotItr& slotItr)
{
    CRY_ASSERT(slotItr != m_slots.end(), "Slot not found in column!");
    if (slotItr == m_slots.end())
    {
        return false;
    }
    m_slots.erase(slotItr);
    return true;
}

bool CFormationColumn::RemoveSlot(const IFormationSlot* pSlot)
{
    SlotCollection::iterator slotItr = std::find(m_slots.begin(), m_slots.end(), pSlot);
    return RemoveSlot(slotItr);
}

CFormationSlot* CFormationColumn::RemoveSlot(const uint index)
{
    CRY_ASSERT(index < m_slots.size(), "Depth out of range in column!");
    if (index >= m_slots.size())
    {
        CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_ERROR, "Depth out of range in column!");
        return nullptr;
    }
    SlotCollection::iterator slotItr = m_slots.begin() + index;
    CFormationSlot* pSlot = *slotItr;
    m_slots.erase(slotItr);
    return pSlot;
}

void CFormationColumn::Clear()
{
    for (CFormationSlot* pSlot : m_slots)
    {
        pSlot->Destroy();
    }
    m_slots.erase(m_slots.begin(), m_slots.end());
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

CFormationColumn::SColumnProperties CFormationColumn::GetColumnProperties() const
{
    SColumnProperties columnProperties;
    columnProperties.slotProperties = std::vector<Slot::SSlotProperties>(m_slots.size());
    for (Slot* pSlot : m_slots)
    {
        columnProperties.slotProperties.push_back(pSlot->GetSlotProperties());
    }
    columnProperties.xPos = m_xPos;
    columnProperties.width = m_width;
    return columnProperties;
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

