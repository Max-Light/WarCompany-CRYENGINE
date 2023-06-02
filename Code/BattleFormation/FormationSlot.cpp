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
		Vec3 halfSize = m_slotSize / 2;
		AABB box = AABB(Vec3(-halfSize.x, -halfSize.y, 0), Vec3(halfSize.x, halfSize.y, m_slotSize.z));
		gEnv->pAuxGeomRenderer->SetRenderFlags(SAuxGeomRenderFlags());
		gEnv->pAuxGeomRenderer->DrawAABB(box, m_pEntity->GetWorldTM(), true, ColorB(0, 255, 255), EBoundingBoxDrawStyle::eBBD_Faceted);
	}
	break;
	}
}

bool CFormationSlot::IsFormationReady() const
{
	return false;
}

void CFormationSlot::SetPos(const Vec2& gridPos)
{
	Vec3 pos = m_pFormation->CreatePos(gridPos);
	GetEntity()->SetPos(pos);
}

void CFormationSlot::SetSize(const Vec3& size)
{
	m_slotSize = size;
}

CFormationSlot* CFormationSlot::CreateSlot(const SSlotSpawnParams& slotParams)
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams.sName = "Formation Slot";
	spawnParams.pParent = slotParams.pFormation->GetEntity();
	spawnParams.vPosition = slotParams.pFormation->CreatePos(slotParams.gridPosition);

	IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams);
	CFormationSlot* pSlot = pEntity->GetOrCreateComponent<CFormationSlot>();
	pSlot->AssignUnit(slotParams.pUnit);
	pSlot->SetSize(slotParams.slotSize);
	pSlot->m_pFormation = slotParams.pFormation;
	return pSlot;
}

CFormationSlot::SSlotProperties CFormationSlot::GetSlotProperties() const
{
	SSlotProperties slotProperties;
	slotProperties.unitId = 0;
	slotProperties.gridPos = GetGridPos();
	return slotProperties;
}
