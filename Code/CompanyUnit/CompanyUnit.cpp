#include "StdAfx.h"
#include "CompanyUnit.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>
#include <CryRenderer/IRenderAuxGeom.h>


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
		AABB boundingBox = AABB(m_pEntity->GetPos(), m_pEntity->GetPos() + Vec3(5, 3, 2));
		gEnv->pAuxGeomRenderer->DrawAABB(boundingBox, true, ColorB(0, 0, 255), EBoundingBoxDrawStyle::eBBD_Faceted);
	}
	break;
	}
}
