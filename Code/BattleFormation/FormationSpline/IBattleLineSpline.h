#pragma once

#include "IVertexPoint.h"
#include "IAnchorPoint.h"
#include "ICurveSegment.h"

#include <CryEntitySystem/IEntityComponent.h>

struct IBattleLineSpline : public IEntityComponent
{
	static void ReflectType(Schematyc::CTypeDesc<IBattleLineSpline>& desc)
	{
		desc.SetGUID("{ABAE3702-F302-4122-A470-639ABD50938B}"_cry_guid);
	}

	// Return the pointing battle formation
	virtual IBattleFormation* GetFormation() const = 0;

	// Inserts a new curve into the spline at the specified column and its normalized offset value [-1, 1]
	virtual void InsertCurve(IFormationColumn* pColumn, float normalVal = 0.0f) = 0;
	virtual void InsertCurve(float battleLineXPos) = 0;

	enum class ECurveDirection : uint8
	{
		Left,
		Right
	};
	// Removes the vertex and chosen curve segment from the left or right
	virtual void RemoveCurve(const IVertexPoint* pVertex, const ECurveDirection& dir = ECurveDirection::Right) = 0;

	// Applies the curve on the specified curve segment
	virtual void ApplyCurveMode(ICurveSegment* pCurveSegment, const ICurveSegment::ECurveMode& mode) = 0;

	// Move the vertex located on the spline
	virtual void MoveVertex(IVertexPoint* pVertex, const Vec2& pos) = 0;

	// Move the anchor located on the spline
	virtual void MoveAnchor(IAnchorPoint* pAnchor, const Vec2& pos) = 0;
};