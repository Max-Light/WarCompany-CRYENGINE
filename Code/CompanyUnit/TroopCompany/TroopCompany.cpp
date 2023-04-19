
#include "TroopCompany.h"

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
	if (m_pTroopCompanyClass == nullptr)
	{
		CryLog("WARNING: Troop Company has no class model.");
	}
}

Cry::Entity::EventFlags CTroopCompany::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CTroopCompany::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
	{
		gEnv->pAuxGeomRenderer->DrawAABB(GetAABB(), true, ColorB(0, 0, 25), EBoundingBoxDrawStyle::eBBD_Faceted);
	}
	break;
	}
}

Vec3 CTroopCompany::GetSize() const
{
	float width = m_troopLineCount * m_pTroopCompanyClass->TroopSize().x;
	float height;
	if (m_troopUnits.size() % m_troopLineCount == 0)
	{
		height = (m_troopUnits.size() / m_troopLineCount) * m_pTroopCompanyClass->TroopSize().y;
	}
	else
	{
		height = (int)(m_troopUnits.size() / m_troopLineCount) + 1 * m_pTroopCompanyClass->TroopSize().y;
	}
	return Vec3(width, height, m_pTroopCompanyClass->TroopSize().z);
}

AABB CTroopCompany::GetAABB() const
{
	Vec3 halfSize = GetSize() / 2;
	return AABB(Vec3(-halfSize.x, -halfSize.y, 0), Vec3(halfSize.x, halfSize.y, m_pTroopCompanyClass->TroopSize().z));
}

const CTroopCompany::TTroop* CTroopCompany::GetTroopInGrid(uint y, uint x) const
{
	uint index = y * m_troopLineCount + x;
	if (index >= m_troopUnits.size())
	{
		return nullptr;
	}
	return &m_troopUnits[index];
}

void CTroopCompany::ResizeTroopLine(uint troopCount)
{
	if (troopCount > m_troopUnits.capacity())
	{
		CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_WARNING, "The width input is larger than the troop capacity! Setting width to the capacity amount.");
		m_troopLineCount = m_troopUnits.capacity();
	}
	else
	{
		m_troopLineCount = troopCount;
	}
}

void CTroopCompany::SetTroopCapacity(uint capacity)
{
	m_troopUnits.reserve(capacity);
	if (capacity < m_troopLineCount)
	{
		m_troopLineCount = capacity;
	}
}

