
#include "TroopUnit.h"

#include "GamePlugin.h"

#include <CryRenderer/IRenderAuxGeom.h>

namespace
{
	static void RegisterTroopUnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CTroopUnit));
		}
	}
	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterTroopUnitComponent);
}

void CTroopUnit::Initialize()
{
	m_pTroopUnitClass = CGamePlugin::GetInstance()->GetTroopUnitSystem()->GetTroopUnitClass((TroopId)m_troopType);
	if (!m_pTroopUnitClass)
	{
		CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_ERROR, "Troop Unit requires a class type");
		return;
	}

	m_pCollider = GetEntity()->GetOrCreateComponent<Cry::DefaultComponents::CCylinderPrimitiveComponent>();

	m_pNavigationComponent = GetEntity()->GetComponent<IEntityNavigationComponent>();
	if (!m_pNavigationComponent)
	{
		m_pNavigationComponent = GetEntity()->CreateComponent<IEntityNavigationComponent>();
		m_pNavigationComponent->SetNavigationAgentType("TroopUnit");
		m_pNavigationComponent->SetCollisionAvoidanceProperties(m_pTroopUnitClass->GetNavProperties().navCollisionAvoidance);
		m_pNavigationComponent->SetMovementProperties(m_pTroopUnitClass->GetNavProperties().navMovementProperties);
	}
}

Cry::Entity::EventFlags CTroopUnit::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CTroopUnit::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
		gEnv->pAuxGeomRenderer->DrawCylinder(GetWorldPos() + Vec3(0, 0, m_pCollider->m_height / 2), Vec3(0, 0, 1), m_pCollider->m_radius, m_pCollider->m_height, ColorF(0, 1, 0), false);
		break;
	}
}

