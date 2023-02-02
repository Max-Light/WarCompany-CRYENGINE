#pragma once

#include "ISplineControlPoint.h"


class CVertexPoint;

class CAnchorPoint : public ISplineControlPoint
{
public:
	CAnchorPoint() = default;
	virtual ~CAnchorPoint() = default;

	// ISplineControlPoint implementation
	virtual Vec3 GetPosition() const override { return m_position; }
	virtual void SetPosition(const Vec3& position) override { m_position = position; }
	// ~ISplineControlPoint implementation

public:
	static constexpr float s_anchorArmScaleLength = 0.33f;
	static constexpr float s_maxAnchorAngle = 45;
private:
	Vec3 m_position;
};