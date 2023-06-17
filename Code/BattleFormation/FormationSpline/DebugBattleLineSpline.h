#pragma once

#include "IBattleLineSpline.h"

#include <DefaultComponents/Audio/TriggerComponent.h>
#include <DefaultComponents/Physics/Vehicles/VehicleComponent.h>


class CDebugBattleLineSpline : public IEntityComponent
{
public:
	CDebugBattleLineSpline() = default;
	virtual ~CDebugBattleLineSpline()
	{
		delete m_pCurveItr;
	}

	static void ReflectType(Schematyc::CTypeDesc<CDebugBattleLineSpline>& desc)
	{
		desc.SetGUID("{4C3056EE-572F-4394-9599-7543E9E083A4}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Debug Battle Line Spline");
		desc.SetDescription("Debug the battle line spline component");
		desc.AddMember(&CDebugBattleLineSpline::m_splinePoints, 'spli', "Spline_Points", "Curve Points", "Set additional points within the battle line spline.", Schematyc::CArray<Vec2>());
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

private:
	void UpdateSpline();
private:
	Schematyc::CArray<Vec2> m_splinePoints;
	IBattleLineSpline* m_pSpline = nullptr;
	ICurveIterator* m_pCurveItr = nullptr;
	Serialization::FunctorActionButton<std::function<void()>> m_button;
};