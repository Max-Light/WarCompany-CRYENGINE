#include "StdAfx.h"
#include "BattleFormation.h"
#include "CompanyUnit/TroopCompany/TroopCompany.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>
#include <CryGame/IGameFramework.h>

#include <CrySystem/IConsole.h>

namespace
{
	static void RegisterBattleFormationComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBattleFormation));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBattleFormationComponent);
}

void CBattleFormation::Initialize()
{
	SSlotSpawnParams slotParams;
	slotParams.pUnit = nullptr;
	slotParams.slotSize = Vec3(5, 3, 2);

	InsertColumnAndUnit(0, nullptr);
	InsertColumnAndUnit(0, nullptr);
	InsertColumnAndUnit(0, nullptr);
	InsertColumnAndUnit(0, nullptr);
	InsertUnitInColumn(1, 1, nullptr);
	InsertUnitInColumn(3, 1, nullptr);
	InsertColumnAndUnit(1, nullptr);
}

Cry::Entity::EventFlags CBattleFormation::GetEventMask() const
{
	return Cry::Entity::EEvent::Initialize 
		| Cry::Entity::EEvent::Update 
		| Cry::Entity::EEvent::TransformChanged;
}

void CBattleFormation::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Initialize:
		
		break;
	case Cry::Entity::EEvent::Update:
		
		break;
	case Cry::Entity::EEvent::TransformChanged:
		UpdateAllSlotPos();
		break;
	}
}

uint CBattleFormation::GetSlotCount() const
{
	uint slotCount = 0;
	for (auto colItr = m_formationColumns.begin(); colItr != m_formationColumns.end(); ++colItr)
	{
		slotCount += (*colItr)->GetSlotCount();
	}
	return slotCount;
}

IFormationSlot* CBattleFormation::InsertColumnAndUnit(uint col, IFormationUnit* pUnit, EColumnShiftType shiftType)
{
	CRY_ASSERT(col <= GetColumnCount(), "Column is outside of formation insertion range!");
	Vec3 slotSize = Vec3(5, 3, 2);

	ColumnCollection::iterator colItr = m_formationColumns.begin() + col;
	colItr = InsertColumnAt(colItr, slotSize.x, shiftType);
	CFormationColumn* pColumn = *colItr;
	Vec2 slotGridPos = pColumn->QuerySlotGridPos(0, slotSize);
	
	SSlotSpawnParams slotParams;
	slotParams.pFormation = this;
	slotParams.pUnit = pUnit;
	slotParams.gridPosition = slotGridPos;
	slotParams.slotSize = slotSize;

	CFormationSlot* pSlot = CFormationSlot::CreateSlot(slotParams);
	pColumn->InsertSlot(0, pSlot);
	return pSlot;
}

IFormationSlot* CBattleFormation::InsertUnitInColumn(uint col, uint depth, IFormationUnit* pUnit)
{
	CRY_ASSERT(col < GetColumnCount(), "Column is outside of formation range!");
	ColumnCollection::iterator colItr = m_formationColumns.begin() + col;
	CFormationColumn* pColumn = *colItr;
	CRY_ASSERT(depth <= pColumn->GetSlotCount(), "Depth is outside of formation insertion range!");

	Vec3 slotSize = Vec3(5, 3, 2);
	Vec2 slotGridPos = pColumn->QuerySlotGridPos(depth, slotSize);

	SSlotSpawnParams slotParams;
	slotParams.pFormation = this;
	slotParams.pUnit = pUnit;
	slotParams.gridPosition = slotGridPos;
	slotParams.slotSize = slotSize;

	CFormationSlot* pSlot = CFormationSlot::CreateSlot(slotParams);
	pColumn->InsertSlot(depth, pSlot);
	return pSlot;
}

void CBattleFormation::RemoveSlot(uint col, uint depth)
{
	
}

void CBattleFormation::RemoveSlot(IFormationSlot* pSlot)
{
	
}

Vec3 CBattleFormation::CreatePos(const Vec2& pos)
{
	Vec2 worldPos = GetPos() + (Vec3)pos * GetRotation();
	float elevation = gEnv->p3DEngine->GetTerrainElevation(worldPos.x, worldPos.y);
	float entityElevation = GetEntity()->GetWorldPos().z;
	Vec3 newPos = pos + Vec3(0, 0, elevation - entityElevation);
	return newPos;
}

ColumnCollection::iterator CBattleFormation::InsertColumnAt(ColumnCollection::iterator colItr, float columnWidth, const EColumnShiftType& shiftType)
{
	ShiftColumnsAt(colItr, columnWidth, shiftType);

	CFormationColumn* pColumn = new CFormationColumn();
	colItr = m_formationColumns.insert(colItr, pColumn);

	float xPos = columnWidth / 2;
	if (colItr != m_formationColumns.begin())
	{
		xPos += (*(colItr - 1))->GetXPos() + (*(colItr - 1))->GetWidth() / 2;
	}

	(*colItr)->SetXPos(xPos);
	(*colItr)->SetWidth(columnWidth);
	return colItr;
}

void CBattleFormation::ShiftColumnsAt(const ColumnCollection::iterator& colItr, float offset, const EColumnShiftType& shiftType)
{
	switch (shiftType)
	{
	case EColumnShiftType::Left:
		GetEntity()->SetPos(m_pEntity->GetPos() - m_pEntity->GetRightDir() * offset);
		ShiftColumnRange(colItr, m_formationColumns.end(), offset);
		UpdateSlotsInColumnRange(m_formationColumns.begin(), colItr);
		break;
	case EColumnShiftType::Right:
		ShiftColumnRange(colItr, m_formationColumns.end(), offset);
		UpdateSlotsInColumnRange(colItr, m_formationColumns.end());
		break;
	case EColumnShiftType::Center:
		GetEntity()->SetPos(m_pEntity->GetPos() - m_pEntity->GetRightDir() * (*colItr)->GetWidth() / 2);
		ShiftColumnRange(colItr, m_formationColumns.end(), offset);
		UpdateSlotsInColumnRange(m_formationColumns.begin(), m_formationColumns.end());
		break;
	}
}

void CBattleFormation::ShiftColumnRange(ColumnCollection::iterator startItr, const ColumnCollection::iterator& endItr, float offset)
{
	for (startItr; startItr != endItr; ++startItr)
	{
		CFormationColumn* pColumn = *startItr;
		pColumn->SetXPos(pColumn->GetXPos() + offset);
	}
}

void CBattleFormation::UpdateSlotsInColumnRange(ColumnCollection::iterator startItr, const ColumnCollection::iterator& endItr) 
{
	for (startItr; startItr != endItr; ++startItr)
	{
		CFormationColumn* pColumn = *startItr;
		static auto slotPosUpdate = [this](CFormationSlot& slot)
		{
			Vec2 gridPos = slot.GetGridPos();
			slot.SetPos(gridPos);
		};
		pColumn->IterateSlots(slotPosUpdate);
	}
}

void CBattleFormation::IterateSlots(std::function<void(CFormationSlot&)> func) const
{
	for (auto pColumn : m_formationColumns)
	{
		pColumn->IterateSlots(func);
	}
}

void CBattleFormation::UpdateAllSlotPos() 
{
	UpdateSlotsInColumnRange(m_formationColumns.begin(), m_formationColumns.end());
}
