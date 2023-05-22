#pragma once

#include "StdAfx.h"

struct ISplinePoint
{
	// Return the position of the spline point local to the spline object
	virtual Vec3 GetPos() const = 0;

	// Return the grid position of the spline point local to the spline object
	virtual Vec2 GetGridPos() const { return GetPos(); }

	// Set the position of the spline point given a local grid position
	virtual void SetPos(const Vec2& gridPosition) = 0;
};