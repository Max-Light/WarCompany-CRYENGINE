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

	InsertColumnAndUnit(0, slotParams);
	InsertColumnAndUnit(0, slotParams);
	InsertColumnAndUnit(0, slotParams);
	InsertColumnAndUnit(0, slotParams);
	InsertUnitInColumn(1, 1, slotParams);
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
		slotCount += colItr->GetSlots()->size();
	}
	return slotCount;
}

IFormationSlot* CBattleFormation::InsertColumnAndUnit(uint col, SSlotSpawnParams& slotParams, EColumnShiftType shiftType)
{
	CRY_ASSERT(col <= GetColumnCount(), "Column is outside of formation insertion range!");

	TColumnCollection::iterator colItr = m_formationColumns.begin() + col;
	colItr = InsertColumnAt(colItr, slotParams.slotSize.x, shiftType);
	SBattleFormationIterator formItr = SBattleFormationIterator(colItr);
	CFormationSlot* pSlot = SpawnSlot(slotParams, QuerySlotPosition(formItr, slotParams));

	formItr.colItr->GetSlots()->insert(formItr.slotItr, pSlot);
	return pSlot;
}

IFormationSlot* CBattleFormation::InsertUnitInColumn(uint col, uint depth, SSlotSpawnParams& slotParams)
{
	CRY_ASSERT(col < GetColumnCount(), "Column is outside of formation range!");
	CRY_ASSERT(depth <= GetSlotCountInColumn(col), "Depth is outside of formation insertion range!");

	SBattleFormationIterator formItr = SBattleFormationIterator(m_formationColumns.begin() + col);
	formItr.slotItr += depth;
	ShiftSlotsInColumn(formItr.slotItr, formItr.colItr->GetSlots()->end(), slotParams.slotSize.y);
	CFormationSlot* pSlot = SpawnSlot(slotParams, QuerySlotPosition(formItr, slotParams));

	formItr.colItr->GetSlots()->insert(formItr.slotItr, pSlot);
	return pSlot;
}

void CBattleFormation::RemoveSlot(uint col, uint depth)
{
	
}

void CBattleFormation::RemoveSlot(IFormationSlot* pSlot)
{
	
}

CFormationSlot* CBattleFormation::SpawnSlot(const SSlotSpawnParams& slotParams, const Vec3& pos)
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams.sName = "Formation Slot";
	spawnParams.pParent = m_pEntity;
	spawnParams.vPosition = pos;

	IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams);
	CFormationSlot* pSlot = pEntity->GetOrCreateComponent<CFormationSlot>();
	pSlot->AssignUnit(slotParams.pUnit);
	pSlot->SetSize(slotParams.slotSize);

	return pSlot;
}

Vec3 CBattleFormation::QuerySlotPosition(const SBattleFormationIterator& itr, const SSlotSpawnParams& slotParams) const
{
	float x = itr.colItr->GetXPos();
	float y = GetColumnOffset(&*itr.colItr) - (slotParams.slotSize.y / 2);
	if (itr.slotItr != (*itr.colItr->GetSlots()).begin())
	{
		TSlotCollection::iterator prevSlotItr = itr.slotItr - 1;
		y += (*prevSlotItr)->GetPos().y - ((*prevSlotItr)->GetSize().y / 2);
	}
	return GetTerrainPosition(Vec2(x, y));
}

CBattleFormation::TColumnCollection::iterator CBattleFormation::InsertColumnAt(TColumnCollection::iterator colItr, float columnWidth, const EColumnShiftType& shiftType)
{
	colItr = m_formationColumns.emplace(colItr);

	float xPos = columnWidth / 2;
	if (colItr != m_formationColumns.begin())
	{
		xPos += (colItr - 1)->GetXPos() + (colItr - 1)->GetWidth() / 2;
	}

	colItr->SetXPos(xPos);
	colItr->SetWidth(columnWidth);
	ShiftColumnsAt(colItr + 1, columnWidth, shiftType);
	return colItr;
}

void CBattleFormation::ShiftColumnsAt(const TColumnCollection::iterator& colItr, float offset, const EColumnShiftType& shiftType)
{
	for (auto colCopyItr = colItr; colCopyItr != m_formationColumns.end(); ++colCopyItr)
	{
		colCopyItr->SetXPos(colItr->GetXPos() + offset);
	}

	switch (shiftType)
	{
	case EColumnShiftType::Left:
		m_pEntity->SetPos(m_pEntity->GetPos() - m_pEntity->GetRightDir() * offset);
		ShiftSlotsInColumnRange(m_formationColumns.begin(), colItr, offset);
		break;
	case EColumnShiftType::Right:
		ShiftSlotsInColumnRange(colItr, m_formationColumns.end(), offset);
		break;
	case EColumnShiftType::Center:
		m_pEntity->SetPos(m_pEntity->GetPos() - m_pEntity->GetRightDir() * colItr->GetWidth() / 2);
		ShiftSlotsInColumnRange(m_formationColumns.begin(), m_formationColumns.end(), offset);
		break;
	}
}

void CBattleFormation::ShiftSlotsInColumnRange(TColumnCollection::iterator startItr, const TColumnCollection::iterator& endItr, float offset)
{
	for (startItr; startItr != endItr; ++startItr)
	{
		for (auto slotItr = startItr->GetSlots()->begin(); slotItr != startItr->GetSlots()->end(); ++slotItr)
		{
			CFormationSlot* pSlot = (*slotItr);
			Vec2 gridPosition = pSlot->GetPos();
			gridPosition.x += offset;
			pSlot->SetPos(GetTerrainPosition(gridPosition));
		}
	}
}

void CBattleFormation::ShiftSlotsInColumn(TSlotCollection::iterator startItr, const TSlotCollection::iterator& endItr, float offset)
{
	for (startItr; startItr != endItr; ++startItr)
	{
		(*startItr)->SetPos(GetTerrainPosition((*startItr)->GetPos() + Vec3(0, offset, 0)));
	}
}

float CBattleFormation::GetColumnOffset(IBattleFormationColumn* pColumn) const
{
	float verticalOffset = 0;
	for (IColumnVerticalityHandler* pHandler : m_columnVerticalHandlers)
	{
		verticalOffset += pHandler->GetColumnVerticalOffset(pColumn);
	}
	return verticalOffset;
}

Vec3 CBattleFormation::GetTerrainPosition(Vec2 gridPosition) const
{
	Vec3 formationWorldPos = m_pEntity->GetWorldPos();
	gridPosition.x += formationWorldPos.x;
	gridPosition.y += formationWorldPos.y;
	float elevation = gEnv->p3DEngine->GetTerrainElevation(gridPosition.x, gridPosition.y);
	Vec3 terrainPos = Vec3(gridPosition.x, gridPosition.y, elevation);

	IPersistantDebug* pDebug = gEnv->pGameFramework->GetIPersistantDebug();
	pDebug->Begin("Formation Slot Terrain Point", false);
	pDebug->AddSphere(terrainPos, 1, ColorF(1, 0, 0), 5.0f);

	return terrainPos - formationWorldPos;
}

void CBattleFormation::IterateSlots(std::function<void(CFormationSlot&)> func) const
{
	for (auto colItr = m_formationColumns.begin(); colItr != m_formationColumns.end(); ++colItr)
	{
		for (auto slotItr = colItr->GetSlots()->begin(); slotItr != colItr->GetSlots()->end(); ++slotItr)
		{
			func(**slotItr);
		}
	}
}

void CBattleFormation::UpdateAllSlotPos() const
{
	static auto slotPosUpdate = [this](CFormationSlot& slot)
	{
		slot.SetPos(GetTerrainPosition(slot.GetPos()));
	};
	IterateSlots(slotPosUpdate);
}