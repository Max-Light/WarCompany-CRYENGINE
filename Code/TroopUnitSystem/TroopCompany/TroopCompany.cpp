
#include "StdAfx.h"
#include "TroopCompany.h"
#include "GamePlugin.h"

#include <CryPhysics/IPhysics.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterTroopCompanyComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CTroopCompany));
		}
	}
	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterTroopCompanyComponent);
}

void CTroopCompany::Initialize()
{
	
}

Cry::Entity::EventFlags CTroopCompany::GetEventMask() const
{
	return 
		Cry::Entity::EEvent::Initialize | 
		Cry::Entity::EEvent::Update;
}

void CTroopCompany::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Initialize:
		break;
	case Cry::Entity::EEvent::Update:
		//gEnv->pAuxGeomRenderer->DrawAABB(GetAABB(), true, ColorB(0, 0, 25), EBoundingBoxDrawStyle::eBBD_Faceted);
		break;
	}
}

ITroopUnit* CTroopCompany::GetTroopInGrid(Vec2i gridCoord) const
{
	uint troopCount = GetTroopCount();
	uint completeLines = troopCount / m_lineLength;
	uint remainder = troopCount % m_lineLength;
	
	CRY_ASSERT(gridCoord.y < completeLines, "y value out of bounds in troop company grid.");
	if (gridCoord.y < completeLines)
	{
		CRY_ASSERT(gridCoord.x < m_lineLength, "x value out of bounds in troop company grid");
		if (gridCoord.x >= m_lineLength)
		{
			return nullptr;
		}
		return m_troopUnits[gridCoord.y * m_lineLength + gridCoord.x];
	}
	else if (gridCoord.y == completeLines && remainder != 0)
	{
		uint lastLineXStart = (m_lineLength - remainder) / 2;
		CRY_ASSERT(gridCoord.x >= lastLineXStart && gridCoord.x < lastLineXStart + remainder, "x value out of bounds in troop company grid");
		if (gridCoord.x < lastLineXStart || gridCoord.x >= lastLineXStart + remainder)
		{
			return nullptr;
		}
		return m_troopUnits[(troopCount - remainder) + (gridCoord.x - lastLineXStart)];
	}
	else
	{
		return nullptr;
	}
}

Vec3 CTroopCompany::GetSize() const
{
	ITroopUnitClass* pUnitClass = GetTroopUnitClass();
	float width = m_lineLength * GetTroopUnitClass()->GetSize().x;
	float height;
	if (m_troopUnits.size() % m_lineLength == 0)
	{
		height = (m_troopUnits.size() / m_lineLength) * pUnitClass->GetSize().y;
	}
	else
	{
		height = (uint)(m_troopUnits.size() / m_lineLength) + 1 * pUnitClass->GetSize().y;
	}
	return Vec3(width, height, pUnitClass->GetSize().z);
}

void CTroopCompany::AddTroop(ITroopUnit* pTroopUnit)
{
}

void CTroopCompany::RemoveTroop(ITroopUnit* pTroopUnit)
{
}

Vec2i CTroopCompany::TranslateToCompanyGridCoord(uint unitIndex) const
{
	CRY_ASSERT(unitIndex < GetTroopCount(), "Unit index is out of bounds!");

	int unitIndexInLine;
	int lineIndex = unitIndex / m_lineLength;
	int lineStartIndex = lineIndex * m_lineLength;
	if (GetTroopCount() >= lineStartIndex + m_lineLength)
	{
		unitIndexInLine = unitIndex - lineStartIndex;
	}
	else
	{
		int remainingTroopsInLine = GetTroopCount() - lineStartIndex;
		int lastLineStartIndex = (m_lineLength - remainingTroopsInLine) / 2;
		unitIndexInLine = lastLineStartIndex + unitIndex - lineStartIndex;
	}
	
	return Vec2_tpl<int>(unitIndexInLine, lineIndex);
}

Vec3 CTroopCompany::CalculateUnitPosition(uint unitIndex) const
{
	Vec3 companyPos = GetWorldPos();
	Vec3 troopDimension = GetTroopUnitClass()->GetSize();
	Vec2i companyGridCoordinate = TranslateToCompanyGridCoord(unitIndex);
	Vec2 companyGridPos = Vec2(troopDimension.x / 2, -troopDimension.y / 2) + Vec2(companyGridCoordinate.x * troopDimension.x, -companyGridCoordinate.y * troopDimension.y);
	Vec2 worldGridPos = companyPos + companyGridPos;
	Vec3 origin = Vec3(worldGridPos.x, worldGridPos.y, 2500);

	ray_hit hit;
	IPhysicalWorld::SRWIParams rayParams;
	rayParams.org = origin;
	rayParams.dir = Vec3(0, 0, -5000);
	rayParams.objtypes = ent_all;
	rayParams.flags = rwi_stop_at_pierceable | rwi_colltype_any;
	rayParams.hits = &hit;
	rayParams.nMaxHits = 1;
	gEnv->pPhysicalWorld->RayWorldIntersection(rayParams);
	
	return hit.pt;
}

void CTroopCompany::SpawnTroops()
{
	for (uint troopCounter = 0; troopCounter < 100; ++troopCounter)
	{
		if (m_troopUnits[troopCounter])
		{
			continue;
		}

		Vec3 unitPos = CalculateUnitPosition(troopCounter);

		STroopUnitSpawnParams troopUnitSpawnParams;
		troopUnitSpawnParams.troopId = m_pTroopUnitClass->GetTroopId();
		troopUnitSpawnParams.position = unitPos;
		ITroopUnit* pTroopUnit = CGamePlugin::GetInstance()->GetTroopUnitSystem()->SpawnTroopUnit(troopUnitSpawnParams);

		m_troopUnits[troopCounter] = pTroopUnit;
	}
}

void CTroopCompany::InitializeTroopCompany(const STroopCompanyParams& troopCompanyParams)
{
	m_pTroopUnitClass = troopCompanyParams.pTroopUnitClass;
	m_lineLength = troopCompanyParams.lineLength;
	m_troopUnits = std::vector<ITroopUnit*>(100);
	SpawnTroops();
}

void CTroopCompany::RepositionTroops()
{
	uint troopCounter = 0;
	for (ITroopUnit* pTroopUnit : m_troopUnits)
	{
		if (!pTroopUnit)
		{
			continue;
		}

		Vec3 unitPos = CalculateUnitPosition(troopCounter);
		pTroopUnit->SetPos(unitPos);
	}
}

