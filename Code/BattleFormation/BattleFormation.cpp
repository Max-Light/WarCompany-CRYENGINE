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
	ColumnCollection::iterator colItr = m_formationColumns.begin() + columnIndex;
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

void CBattleFormation::RemoveColumn(uint columnIndex)
{
	ColumnItr colItr = m_formationColumns.begin() + columnIndex;
	CRY_ASSERT(colItr != m_formationColumns.end(), "Column not found in formation!");
	if (colItr == m_formationColumns.end())
	{
		return;
	}
	CFormationColumn* pColumnToRemove = *colItr;

	SBattleFormationEvent formationEvent = SBattleFormationEvent(
		FormationSystem::EFormationEvent::REMOVE_COLUMN,
		reinterpret_cast<intptr_t>(pColumnToRemove),
		0
	);
	NotifyBattleFormationListeners(formationEvent);

	pColumnToRemove->Clear();
	delete pColumnToRemove;
	m_formationColumns.erase(colItr);
}

bool CBattleFormation::RemoveColumn(IFormationColumn* pColumn)
{
	ColumnItr colItr = std::find(m_formationColumns.begin(), m_formationColumns.end(), pColumn);
	CRY_ASSERT(colItr != m_formationColumns.end(), "Column not found in formation!");
	if (colItr == m_formationColumns.end())
	{
		return false;
	}
	CFormationColumn* pColumnToRemove = *colItr;

	SBattleFormationEvent formationEvent = SBattleFormationEvent(
		FormationSystem::EFormationEvent::REMOVE_COLUMN,
		reinterpret_cast<intptr_t>(pColumnToRemove),
		0
	);
	NotifyBattleFormationListeners(formationEvent);

	pColumnToRemove->Clear();
	delete pColumnToRemove;
	m_formationColumns.erase(colItr);
	return true;
}

void CBattleFormation::RemoveSlot(uint columnIndex, uint index)
{
	CRY_ASSERT(columnIndex < GetColumnCount(), "Column index out of range!");
	CFormationColumn* pColumn = m_formationColumns[columnIndex];
	CRY_ASSERT(index < pColumn->GetSlotCount(), "Slot index out of range in Column!");
	CFormationSlot* pSlotToRemove = pColumn->GetSlotAt(index);
	if (!pSlotToRemove)
	{
		CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_ERROR, "Formation Slot does not exist in the formation!");
		return;
	}

	SBattleFormationEvent formationEvent = SBattleFormationEvent(
		FormationSystem::EFormationEvent::ADD_SLOT,
		reinterpret_cast<intptr_t>(pColumn),
		reinterpret_cast<intptr_t>(pSlotToRemove)
	);
	if (pColumn->GetSlotCount() == 1)
	{
		formationEvent.event |= FormationSystem::EFormationEvent::REMOVE_COLUMN;
	}
	NotifyBattleFormationListeners(formationEvent);

	pColumn->RemoveSlot(index);
	pSlotToRemove->Destroy();
	if (pColumn->IsEmpty())
	{
		m_formationColumns.erase(m_formationColumns.begin() + columnIndex);
		delete pColumn;
	}
}

bool CBattleFormation::RemoveSlot(IFormationSlot* pSlot)
{
	uint columnIndex = 0;
	for (CFormationColumn* pColumn : m_formationColumns)
	{
		++columnIndex;
		if (pColumn->IsRanging(pSlot->GetGridPos().x))
		{
			CFormationColumn::SlotItr slotItr = pColumn->FindSlot(pSlot);
			CFormationSlot* pSlotToRemove = *slotItr;
			CRY_ASSERT(pSlotToRemove, "Slot not found in column!");
			if (!pSlotToRemove)
			{
				CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_ERROR, "Formation Slot does not exist in the formation!");
				return false;
			}

			SBattleFormationEvent formationEvent = SBattleFormationEvent(
				FormationSystem::EFormationEvent::ADD_SLOT,
				reinterpret_cast<intptr_t>(pColumn),
				reinterpret_cast<intptr_t>(pSlotToRemove)
			);
			if (pColumn->GetSlotCount() == 1)
			{
				formationEvent.event |= FormationSystem::EFormationEvent::REMOVE_COLUMN;
			}
			NotifyBattleFormationListeners(formationEvent);

			bool result = pColumn->RemoveSlot(slotItr);
			pSlotToRemove->Destroy();
			if (pColumn->IsEmpty())
			{
				m_formationColumns.erase(m_formationColumns.begin() + columnIndex);
				delete pColumn;
			}
			return result;
		}
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

ColumnCollection::iterator CBattleFormation::InsertColumnAt(ColumnItr colItr, float columnWidth, const EColumnShiftType& shiftType)
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

