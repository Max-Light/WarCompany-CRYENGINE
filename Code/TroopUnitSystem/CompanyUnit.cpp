#include "StdAfx.h"
#include "CompanyUnit.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>
#include <CryRenderer/IRenderAuxGeom.h>

#include <CryAISystem/IMovementSystem.h>
#include <CryAISystem/MovementBlock.h>
#include <CryAISystem/MovementRequest.h>
#include <CryAISystem/MovementRequestID.h>
#include <CryAISystem/IAISystem.h>

namespace
{
	static void RegisterCompanyUnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCompanyUnit));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCompanyUnitComponent);
}

void CCompanyUnit::Initialize()
{
	m_pNavAgent = m_pEntity->GetComponent<IEntityNavigationComponent>();
}

Cry::Entity::EventFlags CCompanyUnit::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CCompanyUnit::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
	{
		gEnv->pAuxGeomRenderer->SetRenderFlags(SAuxGeomRenderFlags());
		Vec3 halfSize = Vec3(3, 2, 1) / 2;
		AABB boundingBox = AABB(m_pEntity->GetPos() - halfSize, m_pEntity->GetPos() + halfSize);
		gEnv->pAuxGeomRenderer->DrawAABB(boundingBox, true, ColorB(0, 0, 25), EBoundingBoxDrawStyle::eBBD_Faceted);

		m_pNavAgent->NavigateTo(Vec3(500, 500, 32));
		
	}
	break;
	}
}
