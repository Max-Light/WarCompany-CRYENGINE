#pragma once

#include "IAnchorPoint.h"
#include "IBattleLineSpline.h"

#include <CryEntitySystem/IEntityComponent.h>


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
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IAnchorPoint
	Vec3 GetPos() const { return GetEntity()->GetPos(); }
	// ~IAnchorPoint

	// Set the position of the anchor
	void SetPos(const Vec2& gridPosition);

	// Spawns an anchor point
	static CAnchorPoint* SpawnAnchor(const SAnchorSpawnParams& anchorParams);
private:
	IBattleLineSpline* m_pSpline = nullptr;
};