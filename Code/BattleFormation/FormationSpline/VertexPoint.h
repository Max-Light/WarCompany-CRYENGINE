#pragma once

#include "StdAfx.h"
#include "IVertexPoint.h"

class CVertexPoint : public IVertexPoint
{
public:
	CVertexPoint() = default;
	virtual ~CVertexPoint() override = default;

	static void ReflectType(Schematyc::CTypeDesc<CVertexPoint>& desc)
	{
		desc.SetGUID("{75328B15-7614-49BE-A0C9-CBFAA20EBFF0}"_cry_guid);
		desc.SetEditorCategory("Battle Formation/Battle Line Spline");
		desc.SetLabel("Vertex Point");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IVertexPoint
	virtual Vec3 GetPosition() const override { return m_position; }
	virtual IFormationColumn* GetColumn() const { return m_pColumnRef; }
	virtual void MoveToPosition(const Vec3& gridPos) override;
	// ~IVertexPoint

	float GetNormalizedOffset() const { return m_normalizedOffset; }
	void SetNormalizedOffset(float normalizedVal) { m_normalizedOffset = crymath::clamp(normalizedVal, 0.0f, 1.0f); }
	void SetPos(const Vec3& gridPos) { m_position = gridPos; }
private:
	IFormationColumn* m_pColumnRef = nullptr;
	float m_normalizedOffset;
	Vec3 m_position;
};