
#include "StdAfx.h"
#include "BattleLineSplineComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CryPhysics/IPhysics.h>


namespace
{
    static void RegisterBattleLineSplineComponent(Schematyc::IEnvRegistrar& registrar)
    {
        Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
        {
            Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBattleLineSplineComponent));
        }
    }

    CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBattleLineSplineComponent);
}


void CBattleLineSplineComponent::Initialize()
{
    
}

Cry::Entity::EventFlags CBattleLineSplineComponent::GetEventMask() const
{
    return Cry::Entity::EEvent::Update;
}

void CBattleLineSplineComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case Cry::Entity::EEvent::Update:
    {
        gEnv->pRenderer->GetIRenderAuxGeom()->SetRenderFlags(SAuxGeomRenderFlags());
        gEnv->pRenderer->GetIRenderAuxGeom()->DrawSphere(m_pEntity->GetWorldPos(), 0.5f, ColorB(0, 0, 255), false);
    }
    break;
    }
}

