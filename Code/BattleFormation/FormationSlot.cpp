#include "StdAfx.h"
#include "BattleFormation/FormationSlot.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>
#include <CryRenderer/IRenderAuxGeom.h>

namespace
{
	static void RegisterFormationSlotComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CFormationSlot));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterFormationSlotComponent);
}

void CFormationSlot::Initialize()
{
	CryLog("Slot Entity Created");
}

Cry::Entity::EventFlags CFormationSlot::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CFormationSlot::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
	{
		gEnv->pAuxGeomRenderer->SetRenderFlags(SAuxGeomRenderFlags());
		gEnv->pAuxGeomRenderer->DrawAABB(m_boundingBox, m_pEntity->GetWorldTM(), true, ColorB(0, 255, 255), EBoundingBoxDrawStyle::eBBD_Faceted);
	}
	break;
	}
}

bool CFormationSlot::IsFormationReady() const
{
	return false;
}

void CFormationSlot::SetPos(const Vec3& pos)
{
	m_pEntity->SetPos(pos); 
}

void CFormationSlot::SetSize(const Vec3& size)
{
	Vec3 halfSize = size / 2;
	m_boundingBox = AABB(Vec3(-halfSize.x, -halfSize.y, 0), Vec3(halfSize.x, halfSize.y, size.z));
}
