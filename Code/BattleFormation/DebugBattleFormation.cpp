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
	if (!m_pFormation)
	{
		m_pFormation = GetEntity()->GetOrCreateComponent<CBattleFormation>();
	}
	UpdateFormationParams();
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

void CDebugBattleFormation::UpdateFormationParams()
{
	for (uint columnIndex = 0; columnIndex < m_formationParams.columnParams.Size(); ++columnIndex)
	{
		IFormationColumn* pColumn = m_formationParams.columnParams.At(columnIndex).pColumn;
		if (!pColumn)
		{
			m_pFormation->InsertColumnAndUnit(columnIndex, nullptr, IBattleFormation::EColumnShiftType::Right);
			if (m_formationParams.columnParams.At(columnIndex).slotParams.Size() == 0)
			{
				m_formationParams.columnParams.At(columnIndex).slotParams.PushBack(SSlotParams());
			}
			m_formationParams.columnParams.At(columnIndex).pColumn = m_pFormation->GetColumn(columnIndex);
			m_formationParams.columnParams.At(columnIndex).slotParams.At(0).pSlot = m_pFormation->GetSlot(columnIndex, 0);
		}
		else if (pColumn != m_pFormation->GetColumn(columnIndex))
		{
			m_pFormation->RemoveColumn(columnIndex);
			--columnIndex;
			continue;
		}
		else if (m_formationParams.columnParams.At(columnIndex).slotParams.Size() == 0)
		{
			if (m_pFormation->GetSlotCountInColumn(columnIndex) == 1)
			{
				m_pFormation->RemoveSlot(columnIndex, 0);
			}
			m_pFormation->RemoveColumn(columnIndex);
			for (int columnShiftIndex = columnIndex + 1; columnShiftIndex < m_formationParams.columnParams.Size(); ++columnShiftIndex)
			{
				m_formationParams.columnParams.At(columnShiftIndex - 1) = m_formationParams.columnParams.At(columnShiftIndex);
			}
			m_formationParams.columnParams.PopBack();
			--columnIndex;
			continue;
		}

		for (uint slotIndex = 0; slotIndex < m_formationParams.columnParams.At(columnIndex).slotParams.Size(); ++slotIndex)
		{
			IFormationSlot* pSlot = m_formationParams.columnParams.At(columnIndex).slotParams.At(slotIndex).pSlot;
			if (!pSlot)
			{
				m_pFormation->InsertUnitInColumn(columnIndex, slotIndex, nullptr);
				m_formationParams.columnParams.At(columnIndex).slotParams.At(slotIndex).pSlot = m_pFormation->GetSlot(columnIndex, slotIndex);
			}
			else if (pSlot != m_pFormation->GetSlot(columnIndex, slotIndex))
			{
				m_pFormation->RemoveSlot(columnIndex, slotIndex);
				--slotIndex;
			}
		}
	}

	for (uint columnIndex = 0; columnIndex < m_pFormation->GetColumnCount(); ++columnIndex)
	{
		uint slotDifference = m_pFormation->GetSlotCountInColumn(columnIndex) - m_formationParams.columnParams.At(columnIndex).slotParams.Size();
		while (slotDifference != 0)
		{
			m_pFormation->RemoveSlot(columnIndex, m_pFormation->GetSlotCountInColumn(columnIndex) - slotDifference);
			--slotDifference;
		}
		if (m_pFormation->IsColumnEmpty(columnIndex))
		{
			m_pFormation->RemoveColumn(columnIndex);
		}
	}
	uint columnDifference = m_pFormation->GetColumnCount() - m_formationParams.columnParams.Size();
	while (columnDifference != 0)
	{
		m_pFormation->RemoveColumn(m_pFormation->GetColumnCount() - columnDifference);
		--columnDifference;
	}
}

