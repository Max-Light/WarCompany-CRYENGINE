
#include "VertexPoint.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <DefaultComponents/Physics/SpherePrimitiveComponent.h>

namespace
{
	static void RegisterVertexPointComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CVertexPoint));
		}
	}
	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterVertexPointComponent);
}

void CVertexPoint::Initialize()
{
	Cry::DefaultComponents::CSpherePrimitiveComponent* pCollider = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CSpherePrimitiveComponent>();
	pCollider->m_radius = 2.5f;
}

Cry::Entity::EventFlags CVertexPoint::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CVertexPoint::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
		gEnv->pAuxGeomRenderer->DrawSphere(GetEntity()->GetWorldPos(), 0.5f, ColorF(1, 0, 0), false);
		break;
	}
}

void CVertexPoint::SetPos(const Vec2& gridPosition)
{
	Vec3 pos = m_pSpline->GetFormation()->CreatePos(gridPosition);
	GetEntity()->SetPos(pos);
}

CVertexPoint* CVertexPoint::CreateVertex(SVertexSpawnParams& vertexParams)
{
	CRY_ASSERT(vertexParams.pSpline != nullptr, "Vertex point must have a reference to a battle line spline!");

	SEntitySpawnParams spawnParams;
	spawnParams.vPosition = vertexParams.pSpline->GetFormation()->CreatePos(vertexParams.gridPosition);
	spawnParams.sName = "Vertex Point";
	spawnParams.pParent = vertexParams.pSpline->GetEntity();

	IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams);
	CVertexPoint* pVertex = pEntity->GetOrCreateComponent<CVertexPoint>();
	pVertex->m_pSpline = vertexParams.pSpline;
	return pVertex;
}

