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

	// Inserts a new vertex and curve segment 
	virtual void InsertCurve(IFormationColumn& column, float normalVal = 0.5f) = 0;

	enum class ECurveDirection : uint8
	{
		Left,
		Right
	};
	// Removes the vertex and chosen curve segment from the left or right
	virtual void RemoveCurve(IVertexPoint& vertex, const ECurveDirection& dir = ECurveDirection::Right) = 0;

	// Applies the curve on the specified curve segment
	virtual void ApplyCurveMode(ICurveSegment& curveSegment, const ICurveSegment::ECurveMode& mode) = 0;
};