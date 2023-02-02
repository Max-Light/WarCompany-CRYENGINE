#pragma once

#include "ISplineControlPoint.h"
#include "AnchorPoint.h"


class CVertexPoint : public ISplineControlPoint
{
public:
	CVertexPoint() = default;
	virtual ~CVertexPoint() override = default;

	// ISplineControlPoint implementation
	virtual Vec3 GetPosition() const override { return m_pEntity->GetPos(); }
	virtual void SetPosition(const Vec3& position) override { m_pEntity->SetPos(position); }
	// ~ISplineControlPoint implementation

	/*Vec3 CalculateCurvePosition(float t);
	static Vec3 CalculateCurvePosition(CVertexPoint& vertex, float t);
	Vec3 CalculateCurveTangent(float t);
	static Vec3 CalculateCurveTangent(CVertexPoint& vertex, float t);*/
private:
	IEntity* m_pEntity = nullptr;
};