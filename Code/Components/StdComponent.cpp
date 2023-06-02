
#include "StdAfx.h"
#include "StdComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryRenderer/IRenderAuxGeom.h>

namespace
{
	static void RegisterStdComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CStdComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterStdComponent);
}

void CStdComponent::Initialize()
{
    
}

Cry::Entity::EventFlags CStdComponent::GetEventMask() const
{
    return Cry::Entity::EEvent::Update
        | Cry::Entity::EEvent::EditorPropertyChanged;
}

void CStdComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case Cry::Entity::EEvent::Update:
        gEnv->pRenderer->GetIRenderAuxGeom()->SetRenderFlags(SAuxGeomRenderFlags());
        gEnv->pRenderer->GetIRenderAuxGeom()->DrawSphere(m_pEntity->GetPos(), 1, ColorB(0, 128, 128), false);
        break;
    case Cry::Entity::EEvent::EditorPropertyChanged:
        CryLog("%i %i %i %i", event.nParam[0], event.nParam[1], event.nParam[2], event.nParam[3]);
        break;
    }
}