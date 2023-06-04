#include "StdAfx.h"
#include "BattleFormation.h"
#include "CompanyUnit/TroopCompany/TroopCompany.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>
#include <CryGame/IGameFramework.h>

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
	
}

Cry::Entity::EventFlags CBattleFormation::GetEventMask() const
{
	return Cry::Entity::EEvent::Initialize 
		| Cry::Entity::EEvent::Update 
		| Cry::Entity::EEvent::TransformChanged
		| Cry::Entity::EEvent::EditorPropertyChanged;
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
		
		break;
	case Cry::Entity::EEvent::EditorPropertyChanged:
		Initialize();
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

float CBattleFormation::GetBattleLineLength() const
{
	if (m_formationColumns.empty())
	{
		return 0;
	}
	else 
	{
		return (*(--m_formationColumns.end()))->GetMaxXPos();
	}
}

IFormationSlot* CBattleFormation::InsertColumnAndUnit(uint columnIndex, IFormationUnit* pUnit, EColumnShiftType shiftType)
{
	CRY_ASSERT(columnIndex <= GetColumnCount(), "Column is outside of formation insertion range!");
	Vec3 slotSize = Vec3(5, 3, 2);
	ColumnItr colItr = m_formationColumns.begin() + columnIndex;
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

	SBattleFormationEvent formationEvent = SBattleFormationEvent(
		FormationSystem::EFormationEvent::ADD_COLUMN | 
		FormationSystem::EFormationEvent::ADD_SLOT, 
		reinterpret_cast<intptr_t>(pColumn),
		reinterpret_cast<intptr_t>(pSlot)
	);
	NotifyBattleFormationListeners(formationEvent);
	return pSlot;
}

IFormationSlot* CBattleFormation::InsertUnitInColumn(uint columnIndex, uint depth, IFormationUnit* pUnit)
{
	CRY_ASSERT(columnIndex < GetColumnCount(), "Column is outside of formation range!");
	ColumnItr colItr = m_formationColumns.begin() + columnIndex;
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

	SBattleFormationEvent formationEvent = SBattleFormationEvent(
		FormationSystem::EFormationEvent::ADD_SLOT,
		reinterpret_cast<intptr_t>(pColumn),
		reinterpret_cast<intptr_t>(pSlot)
	);
	NotifyBattleFormationListeners(formationEvent);
	return pSlot;
}

void CBattleFormation::RemoveColumn(uint columnIndex, EColumnShiftType shiftType)
{
	ColumnItr columnItr = m_formationColumns.begin() + columnIndex;
	CRY_ASSERT(columnItr != m_formationColumns.end(), "Column not found in formation!");
	if (columnItr == m_formationColumns.end())
	{
		CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_ERROR, "Column Index is out of formation range!");
		return;
	}
	RemoveColumn(columnItr, shiftType);
}

bool CBattleFormation::RemoveColumn(IFormationColumn* pColumn, EColumnShiftType shiftType)
{
	ColumnItr columnItr = std::find(m_formationColumns.begin(), m_formationColumns.end(), pColumn);
	CRY_ASSERT(columnItr != m_formationColumns.end(), "Column not found in formation!");
	if (columnItr == m_formationColumns.end())
	{
		return false;
	}
	RemoveColumn(columnItr, shiftType);
	return true;
}

void CBattleFormation::RemoveSlot(uint columnIndex, uint slotIndex)
{
	FormationItr formationItr = GetFormationItr(columnIndex, slotIndex);
	RemoveSlot(formationItr);
}

bool CBattleFormation::RemoveSlot(IFormationSlot* pSlot)
{
	FormationItr formationItr = GetBeginFormationItr();
	while (formationItr.GetColumnItr() != m_formationColumns.end())
	{
		if (formationItr.GetColumn()->IsRanging(pSlot->GetGridPos().x))
		{
			if (formationItr.GoToSlot(pSlot))
			{
				RemoveSlot(formationItr);
				return true;
			}
			else
			{
				return false;
			}
		}
		formationItr.NextColumn();
	}
	return false;
}

void CBattleFormation::ClearSlots()
{
	for (CFormationColumn* pColumn : m_formationColumns)
	{
		pColumn->Clear();
		delete pColumn;
	}
	m_formationColumns.erase(m_formationColumns.begin(), m_formationColumns.end());
}

Vec3 CBattleFormation::CreatePos(const Vec2& pos)
{
	Vec2 worldPos = GetPos() + (Vec3)pos * GetRotation();
	float elevation = gEnv->p3DEngine->GetTerrainElevation(worldPos.x, worldPos.y);
	float entityElevation = GetEntity()->GetWorldPos().z;
	Vec3 newPos = pos + Vec3(0, 0, elevation - entityElevation);
	return newPos;
}

void CBattleFormation::RemoveColumn(ColumnItr& columnItr, EColumnShiftType shiftType)
{
	CFormationColumn* pColumnToRemove = *columnItr;

	SBattleFormationEvent formationEvent = SBattleFormationEvent(
		FormationSystem::EFormationEvent::REMOVE_COLUMN,
		reinterpret_cast<intptr_t>(pColumnToRemove),
		0
	);
	NotifyBattleFormationListeners(formationEvent);

	float columnWidth = pColumnToRemove->GetWidth();
	pColumnToRemove->Clear();
	delete pColumnToRemove;
	columnItr = m_formationColumns.erase(columnItr);

	ShiftColumnsAt(columnItr, -columnWidth, shiftType);
}

void CBattleFormation::RemoveSlot(const FormationItr& formationItr)
{
	CFormationColumn* pColumn = formationItr.GetColumn();
	CFormationSlot* pSlotToRemove = formationItr.GetSlot();

	SBattleFormationEvent formationEvent = SBattleFormationEvent(
		FormationSystem::EFormationEvent::REMOVE_SLOT,
		reinterpret_cast<intptr_t>(pColumn),
		reinterpret_cast<intptr_t>(pSlotToRemove)
	);
	NotifyBattleFormationListeners(formationEvent);

	pColumn->RemoveSlot(formationItr.GetSlotItr());
	pSlotToRemove->Destroy();
}

CBattleFormation::SFormationProperties CBattleFormation::GetFormationProperties() const
{
	SFormationProperties formationProperties;
	formationProperties.columnProperties = std::vector<FormationColumn::SColumnProperties>(m_formationColumns.size());
	for (FormationColumn* pColumn : m_formationColumns)
	{
		formationProperties.columnProperties.push_back(pColumn->GetColumnProperties());
	}
	return formationProperties;
}

CBattleFormation::ColumnItr CBattleFormation::InsertColumnAt(ColumnItr colItr, float columnWidth, const EColumnShiftType& shiftType)
{
	ShiftColumnsAt(colItr, columnWidth, shiftType);

	CFormationColumn* pColumn = new CFormationColumn();
	colItr = m_formationColumns.insert(colItr, pColumn);

	float xPos = columnWidth / 2;
	if (colItr != m_formationColumns.begin())
	{
		xPos += (*(colItr - 1))->GetXPos() + (*(colItr - 1))->GetWidth() / 2;
	}

	pColumn->SetXPos(xPos);
	pColumn->SetWidth(columnWidth);
	return colItr;
}

void CBattleFormation::ShiftColumnsAt(const ColumnItr& colItr, float offset, const EColumnShiftType& shiftType)
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

void CBattleFormation::ShiftColumnRange(ColumnItr startItr, const ColumnItr& endItr, float offset)
{
	for (startItr; startItr != endItr; ++startItr)
	{
		CFormationColumn* pColumn = *startItr;
		pColumn->SetXPos(pColumn->GetXPos() + offset);
	}
}

void CBattleFormation::UpdateSlotsInColumnRange(ColumnItr startItr, const ColumnItr& endItr)
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

