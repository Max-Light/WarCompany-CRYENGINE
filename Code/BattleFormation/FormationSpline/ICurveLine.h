#pragma once

#include "StdAfx.h"
#include "IAnchorPoint.h"

// Curve types 
enum class ECurveMode : uint8
{
	Linear = 0,
	Quadratic = 1,
	Cubic = 2
};

struct ICurveLine
{
	ICurveLine() = default;
	virtual ~ICurveLine() = default;

	// Return the curve mode of the line
	virtual ECurveMode GetCurveMode() const = 0;

	// Aplies a curve type mode to the line
	virtual void ApplyCurveMode(const ECurveMode& mode) = 0;
};