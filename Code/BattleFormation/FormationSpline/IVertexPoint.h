#pragma once

#include "StdAfx.h"

struct IVertexPoint : public IEntityComponent
{
	static void ReflectType(Schematyc::CTypeDesc<IVertexPoint>& desc)
	{
		desc.SetGUID("{75328B15-7614-49BE-A0C9-CBFAA20EBFF0}"_cry_guid);
		desc.SetEditorCategory("Battle Formation/Battle Line Spline");
		desc.SetLabel("Vertex Point");
	}

	// IEntityComponent
	virtual void Initialize() override = 0;
	virtual Cry::Entity::EventFlags GetEventMask() const override = 0;
	virtual void ProcessEvent(const SEntityEvent& event) override = 0;
	// ~IEntityComponent

	virtual Vec3 GetPosition() const = 0;
	virtual void MoveToPosition(const Vec3& pos) = 0;
};