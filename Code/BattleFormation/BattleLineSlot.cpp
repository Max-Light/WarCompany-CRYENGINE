
#include "StdAfx.h"
#include "BattleLineSlot.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CryPhysics/IPhysics.h>


namespace
{
	static void RegisterBattleLineSlotComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBattleLineSlot));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBattleLineSlotComponent);
}

void CBattleLineSlot::Initialize()
{
	
}

Cry::Entity::EventFlags CBattleLineSlot::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CBattleLineSlot::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
	{
		gEnv->pRenderer->GetIRenderAuxGeom()->SetRenderFlags(SAuxGeomRenderFlags());
		gEnv->pRenderer->GetIRenderAuxGeom()->DrawSphere(m_pEntity->GetWorldPos(), 1, ColorB(0, 128, 128), false);
	}
	break;
	}
}

void CBattleLineSlot::LookDirection(const Vec3& position)
{
}

void CBattleLineSlot::SetRadius(float radius)
{
}
