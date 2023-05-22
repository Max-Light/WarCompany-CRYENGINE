
#include "AnchorPoint.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <DefaultComponents/Physics/SpherePrimitiveComponent.h>

namespace
{
    static void RegisterAnchorPointComponent(Schematyc::IEnvRegistrar& registrar)
    {
        Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
        {
            Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAnchorPoint));
        }
    }
    CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAnchorPointComponent);
}

void CAnchorPoint::Initialize()
{

}

Cry::Entity::EventFlags CAnchorPoint::GetEventMask() const
{
    return Cry::Entity::EEvent::Update;
}

void CAnchorPoint::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case Cry::Entity::EEvent::Update:
        gEnv->pAuxGeomRenderer->DrawSphere(GetEntity()->GetWorldPos(), 0.2f, ColorF(0, 0.5f, 0.5f), false);
        break;
    }
}

void CAnchorPoint::SetPos(const Vec2& gridPosition)
{
    Vec3 pos = m_pSpline->GetFormation()->CreatePos(gridPosition);
    GetEntity()->SetPos(pos);
}

CAnchorPoint* CAnchorPoint::CreateAnchor(const SAnchorSpawnParams& anchorParams)
{
    CRY_ASSERT(anchorParams.pSpline != nullptr, "Anchor Point must have a refernece to a battle line spline!");

    SEntitySpawnParams spawnParams;
    spawnParams.vPosition = anchorParams.pSpline->GetFormation()->CreatePos(anchorParams.gridPosition);
    spawnParams.sName = "Anchor Point";
    spawnParams.pParent = anchorParams.pSpline->GetEntity();
    spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();

    IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams);
    CAnchorPoint* pAnchor = pEntity->GetOrCreateComponent<CAnchorPoint>();
    pAnchor->m_pSpline = anchorParams.pSpline;

    return pAnchor;
}
