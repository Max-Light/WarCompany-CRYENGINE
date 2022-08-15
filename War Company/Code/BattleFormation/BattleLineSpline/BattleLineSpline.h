#pragma once

#include "IControlPoint.h"
#include "VertexPoint.h"
#include "AnchorPoint.h"
#include "StdAfx.h"

#include <BattleFormation/BattleFormationSlot.h>

class CBattleLineSpline final
{
public:
	friend class CBattleLineSlot;
	friend class CVertexPoint;
	friend class CAnchorPoint;

	struct Curve
	{
		const CVertexPoint* start;
		const CVertexPoint* end;
		const float length;
		const int count;
	};
public:
	CBattleLineSpline() {}
	~CBattleLineSpline() = default;
	
	Vec3 CalculateCurvePosition(const CVertexPoint& start, float interpolatedValue) const;
	Vec3 CalculateCurveTangent(const CVertexPoint& start, float interpolatedValue) const;
	CVertexPoint* GetStartVertexPoint() const { return m_startVertexPoint; }
	CVertexPoint* GetEndVertexPoint() const { return m_endVertexPoint; }
	CVertexPoint* GetVertexPointAt(int index) const;
	const Curve& GetCurve(const CVertexPoint& start) const;
	void InsertVertexPointAtEnd();
	void InsertVertexPointAtStart();
	void InsertVertexPointAt(int index);
	void RemoveVertexPointAtStart();
	void RemoveVertexPointAtEnd();
	void RemoveVertexPointAt(int index);
protected:
	void RestrictVertexDistance(CVertexPoint& vertex);
	void RestrictAnchorDistance(CAnchorPoint& anchor);
	void RestrictVertexAngleRange(CVertexPoint& vertex);
	void RestrictAnchorAngleRange(CAnchorPoint& anchor);
	void CompressCurve(CVertexPoint& startVertexPoint);
	void ReflectCurve(CVertexPoint& reflectionPoint);
private:
	CVertexPoint* m_startVertexPoint = nullptr;
	CVertexPoint* m_endVertexPoint = nullptr;
	int m_curveCount = 0;

	static constexpr float s_anchorArmScaleLength = 0.33f;
	static constexpr float s_maxAnchorAngle = 45;
	static constexpr float s_maxVertexAngle = 30;
};