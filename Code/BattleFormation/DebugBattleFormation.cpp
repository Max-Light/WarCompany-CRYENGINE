#include "DebugBattleFormation.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>
#include <CryGame/IGameFramework.h>

namespace
{
	static void RegisterDebugBattleFormationComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CDebugBattleFormation));
		}
	}
	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterDebugBattleFormationComponent);
}

void CDebugBattleFormation::Initialize()
{
	if (!m_formation)
	{
		m_formation = GetEntity()->GetOrCreateComponent<CBattleFormation>();
	}
	UpdateFormation();
}

Cry::Entity::EventFlags CDebugBattleFormation::GetEventMask() const
{
	return Cry::Entity::EEvent::Initialize
		| Cry::Entity::EEvent::Update
		| Cry::Entity::EEvent::EditorPropertyChanged;
}

void CDebugBattleFormation::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Initialize:
		break;
	case Cry::Entity::EEvent::Update:
		break;
	case Cry::Entity::EEvent::EditorPropertyChanged:
		Initialize();
		break;
	}
}

void CDebugBattleFormation::UpdateFormation()
{
	if (m_formationParams.columnParams.Size() == 0)
	{
		m_formation->ClearSlots();
	}

	for (uint columnIndex = 0; columnIndex < m_formationParams.columnParams.Size(); ++columnIndex)
	{
		IFormationColumn* pColumn = m_formationParams.columnParams.At(columnIndex).pColumn;
		if (!pColumn)
		{
			InsertNewColumnAndUnit(columnIndex);
		}
		else if (pColumn != m_formation->GetColumn(columnIndex))
		{
			RemoveColumn(columnIndex);
		}

		for (uint slotIndex = 0; slotIndex < m_formationParams.columnParams.At(columnIndex).slotParams.Size(); ++slotIndex)
		{
			IFormationSlot* pSlot = m_formationParams.columnParams.At(columnIndex).slotParams.At(slotIndex).pSlot;
			if (!pSlot)
			{
				InsertNewUnitInColumn(columnIndex, slotIndex);
			}
			else if (pSlot != m_formation->GetSlot(columnIndex, slotIndex))
			{
				RemoveSlotInColumn(columnIndex, slotIndex);
			}
		}
	}

	for (uint columnIndex = 0; columnIndex < m_formationParams.columnParams.Size(); ++columnIndex)
	{
		for (uint lastSlotIndex = m_formation->GetSlotCountInColumn(columnIndex) - 1; lastSlotIndex >= m_formationParams.columnParams.At(columnIndex).slotParams.Size(); --lastSlotIndex)
		{
			RemoveSlotInColumn(columnIndex, lastSlotIndex);
		}
	}
	for (uint lastColumnIndex = m_formation->GetColumnCount() - 1; lastColumnIndex >= m_formationParams.columnParams.Size(); --lastColumnIndex)
	{
		RemoveColumn(lastColumnIndex);
	}
}

void CDebugBattleFormation::InsertNewColumnAndUnit(uint columnIndex)
{
	m_formation->InsertColumnAndUnit(columnIndex, nullptr, IBattleFormation::EColumnShiftType::Right);
	if (m_formationParams.columnParams.At(columnIndex).slotParams.Size() == 0)
	{
		m_formationParams.columnParams.At(columnIndex).slotParams.PushBack(SSlotParams());
	}
	m_formationParams.columnParams.At(columnIndex).pColumn = m_formation->GetColumn(columnIndex);
	m_formationParams.columnParams.At(columnIndex).slotParams.At(0).pSlot = m_formation->GetSlot(columnIndex, 0);
}

void CDebugBattleFormation::InsertNewUnitInColumn(uint columnIndex, uint slotIndex)
{
	m_formation->InsertUnitInColumn(columnIndex, slotIndex, nullptr);
	m_formationParams.columnParams.At(columnIndex).slotParams.At(slotIndex).pSlot = m_formation->GetSlot(columnIndex, slotIndex);
}

void CDebugBattleFormation::RemoveColumn(uint columnIndex)
{
	m_formation->RemoveColumn(columnIndex);
}

void CDebugBattleFormation::RemoveSlotInColumn(uint columnIndex, uint slotIndex)
{
	m_formation->RemoveSlot(columnIndex, slotIndex);
}
