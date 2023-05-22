#pragma once

#include "StdAfx.h"
#include "IAnchorPoint.h"
#include "IBattleLineSpline.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Physics/SpherePrimitiveComponent.h>


struct SAnchorSpawnParams
{
	SAnchorSpawnParams() = default;

	IBattleLineSpline* pSpline = nullptr;
	Vec2 gridPosition = ZERO;
};


class CAnchorPoint : public IAnchorPoint, public IEntityComponent
{
public:
	CAnchorPoint() = default;
	virtual ~CAnchorPoint() override = default;

	static void ReflectType(Schematyc::CTypeDesc<CAnchorPoint>& desc)
	{
		desc.SetGUID("{89BE39F7-6E69-4B05-8C8B-E61BDB0F01D8}"_cry_guid);
		desc.SetLabel("Anchor Point");
		desc.SetDescription("A anchor point attached to a battle line spline.");
		desc.SetComponentFlags(EntityComponentFlags(
			{
				EEntityComponentFlags::HiddenFromUser,
				EEntityComponentFlags::HideFromInspector
			}
		));
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// ISplinePoint
	virtual Vec3 GetPos() const override { return GetEntity()->GetPos(); }
	virtual void SetPos(const Vec2& gridPosition) override;
	// ~ISplinePoint

	// Spawns an anchor point
	static CAnchorPoint* CreateAnchor(const SAnchorSpawnParams& anchorParams);
private:
	IBattleLineSpline* m_pSpline = nullptr;
	Cry::DefaultComponents::CSpherePrimitiveComponent* m_pSphereCollider = nullptr;
};