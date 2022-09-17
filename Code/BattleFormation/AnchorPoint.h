#pragma once

#include "ISplineControlPoint.h"


class CVertexPoint;

class CAnchorPoint : public ISplineControlPoint
{
public:
	enum EAnchorType : uint8
	{
		eInAnchor,
		eOutAnchor
	};
public:
	CAnchorPoint(CVertexPoint* vertexPoint, EAnchorType type)
		: m_pVertex(vertexPoint)
		, m_type(type)
	{
	}
	~CAnchorPoint() = default;

	// ISplineControlPoint implementation
	virtual Vec3 GetPosition() const override { return m_position; }
	virtual void SetPosition(const Vec3& position) override { m_position = position; }
	// ~ISplineControlPoint implementation

	CVertexPoint* Vertex() const { return m_pVertex; }
public:
	static constexpr float s_anchorArmScaleLength = 0.33f;
	static constexpr float s_maxAnchorAngle = 45;
private:
	CVertexPoint* const m_pVertex = nullptr;
	EAnchorType m_type;
	Vec3 m_position;
};