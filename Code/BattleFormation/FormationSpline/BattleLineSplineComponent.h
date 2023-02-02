#pragma once

#include <CryEntitySystem/IEntityComponent.h>

#include "BattleFormation/FormationSlot.h"
#include "BattleFormation/BlockFormation.h"
#include "VertexPoint.h"


class CBattleLineSplineComponent : public IEntityComponent
{
public:
	using TVertex = typename CVertexPoint;
	using TAnchor = typename CAnchorPoint;
public:
	CBattleLineSplineComponent() = default;
	virtual ~CBattleLineSplineComponent() = default;

	static void ReflectType(Schematyc::CTypeDesc<CBattleLineSplineComponent>& desc)
	{
		desc.SetGUID("{ABAE3702-F302-4122-A470-639ABD50938B}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Battle Line Spline");
		desc.SetDescription("Control the shape of splineable battle formations.");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	/*TVertex* operator[](const int index) const noexcept { return GetVertexAt(index); }
	TVertex* Start() const { return m_pStartVertex; }
	TVertex* End() const { return m_pEndVertex; }
	TVertex* GetVertexAt(int index) const;
	int CurveCount() const { return m_curveCount; }*/
private:
	CBlockFormation* m_pFormation = nullptr;
};

