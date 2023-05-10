#pragma once

#include "IVertexPoint.h"
#include "IBattleLineSpline.h"

#include <CryEntitySystem/IEntityComponent.h>

struct SVertexSpawnParams
{
	SVertexSpawnParams() = default;

	IBattleLineSpline* pSpline = nullptr;
	Vec2 gridPosition = ZERO;
};

class CVertexPoint : public IVertexPoint, public IEntityComponent
{
public:
	CVertexPoint() = default;
	virtual ~CVertexPoint() override = default;

	static void ReflectType(Schematyc::CTypeDesc<CVertexPoint>& desc)
	{
		desc.SetGUID("{FF349F10-73BF-4653-965B-1C27F47CA957}"_cry_guid);
		desc.SetLabel("Vertex Point");
		desc.SetDescription("A vertex point attached to a battle line spline.");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IVertexPoint
	virtual Vec3 GetPos() const override { return GetEntity()->GetPos(); }
	virtual float GetBattleLineXPos() const override { return GetEntity()->GetPos().x; }
	// ~IVertexPoint
	
	// Set the position of the the vertex
	void SetPos(const Vec2& gridPosition);

	// Spawns a vertex point
	static CVertexPoint* SpawnVertex(SVertexSpawnParams& vertexParams);
private:
	IBattleLineSpline* m_pSpline = nullptr;
};