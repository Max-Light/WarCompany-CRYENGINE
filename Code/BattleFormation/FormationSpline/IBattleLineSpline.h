#pragma once

#include "StdAfx.h"
#include "IVertexPoint.h"
#include "IAnchorPoint.h"
#include "ICurveProperties.h"
#include "BattleFormation/IBattleFormation.h"

#include <CryEntitySystem/IEntityComponent.h>

// Interface to return curve points and properties on spline sections
struct ICurveSection
{
	virtual IVertexPoint* GetStartVertex() const = 0;
	virtual IAnchorPoint* GetOutAnchor() const = 0;
	virtual IAnchorPoint* GetInAnchor() const = 0;
	virtual IVertexPoint* GetEndVertex() const = 0;
	virtual ICurveProperties* GetCurveLine() const = 0;
};

// Curve iterator containing reference to curve points within the spline
struct ICurveIterator 
{
	virtual ~ICurveIterator() = default;

	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual const ICurveSection& GetCurve() const = 0;
	virtual void Next() = 0;
	virtual bool IsEnd() const = 0;
	virtual void MoveToFirst() = 0;
};

// Pointer to the curve iterator
typedef _smart_ptr<ICurveIterator> ICurveIteratorPtr;

struct IBattleLineSpline : public IEntityComponent
{
	virtual ~IBattleLineSpline() = default;

	static void ReflectType(Schematyc::CTypeDesc<IBattleLineSpline>& desc)
	{
		desc.SetGUID("{ABAE3702-F302-4122-A470-639ABD50938B}"_cry_guid);
	}

	// Return the pointing battle formation
	virtual IBattleFormation* GetFormation() const = 0;

	// Inserts a new curve into the spline at the specified battle line x position
	virtual void InsertCurve(float battleLineXPos) = 0;

	enum class ECurveDirection : uint8
	{
		Left,
		Right,
		Center
	};
	// Removes the vertex and chosen curve segment from the left or right
	virtual void RemoveCurve(const IVertexPoint* pVertex, const ECurveDirection& dir) = 0;

	// Resets the entire battle line spline to a single curve
	virtual void Reset() = 0;

	// Return a curve iterator pointer to the first element
	virtual ICurveIteratorPtr GetCurveIterator() = 0;

	// Returns the max angular restriction of a vertex between adjacent vertices
	virtual float GetMaxVertexAngularRestriction() const = 0;

	// Returns the max angular restriction of an anchor between the two vertices
	virtual float GetMaxAnchorAngularRestriction() const = 0;

	// Returns the default normalized length of an anchor to its nearest vertex
	virtual float GetDefaultNormalizedAnchorLength() const = 0;
};